#ifndef LUMADI_DETAIL_THREADPOOL_STATICTHREADPOOL_H
#define LUMADI_DETAIL_THREADPOOL_STATICTHREADPOOL_H

#include "IThreadPool.h"
#include "IQueue.h"
#include "workStealingQueue.h"
#include "concepts.h"
#include "task.h"
#include "roundRobinStealingStrategy.h"

namespace Lumadi
{

  /**
   * @brief RAII guard to increment/decrement the active thread counter.
   */
  struct ActiveThreadCounter
  {
    std::atomic<size_t> &counter;
    /**
     * @brief Increments the counter on construction.
     * @param c Reference to the atomic counter.
     */
    ActiveThreadCounter(std::atomic<size_t> &c) : counter(c)
    {
      ++counter;
    }
    /**
     * @brief Decrements the counter on destruction.
     */
    ~ActiveThreadCounter()
    {
      --counter;
    }
  };

  template <WorkStealingQueueConcept QueueType>
  class StaticThreadPool
  {
  public:
    /**
     * @brief Constructs a static thread pool with a fixed number of threads.
     * @param numThreads Number of worker threads to create.
     */
    StaticThreadPool(size_t numThreads) : running_(true)
    {
      threads_.reserve(numThreads);
      for (size_t i = 0; i < numThreads; i++)
      {
        localQueues_.push_back(std::make_unique<QueueType>());
        threads_.emplace_back([this, i]
                              { WorkerLoop(i); });
      }
    }

    /**
     * @brief Destructor. Shuts down the thread pool and joins all threads.
     */
    ~StaticThreadPool()
    {
      running_ = false;
      globalQueue_.Stop();
      for (auto &q : localQueues_)
        q->Stop();

      cv_.notify_all();
      for (auto &t : threads_)
        if (t.joinable())
          t.join();
    }

    /**
     * @brief Adds a new task to the thread pool.
     *
     * The task will be enqueued to a local or global queue depending on the calling thread.
     * @tparam Func Callable type.
     * @tparam Args Argument types.
     * @param f Callable to execute.
     * @param args Arguments to pass to the callable.
     * @return Task<ReturnType> representing the result of the task.
     */
    template <typename Func, typename... Args>
    auto AddTask(Func &&f, Args &&...args) -> Task<std::invoke_result_t<Func, Args...>>
    {
      using ReturnType = std::invoke_result_t<Func, Args...>;
      auto task = std::make_shared<std::packaged_task<ReturnType()>>(
          std::bind(std::forward<Func>(f), std::forward<Args>(args)...));
      std::shared_future<ReturnType> future = task->get_future().share();

      auto wrapper = [task]()
      {
        (*task)();
      };

      if (thread_index_ >= 0 && static_cast<size_t>(thread_index_) < localQueues_.size())
      {
        localQueues_[thread_index_]->Enqueue(wrapper);
      }
      else
      {
        globalQueue_.Enqueue(wrapper);
      }

      cv_.notify_one();
      return Task<ReturnType>(std::move(future));
    }

    /**
     * @brief Returns the number of currently active (executing) threads.
     * @return Number of active threads.
     */
    size_t GetActiveThreadCount() const
    {
      return amountOfActiveThreads_;
    }

    /**
     * @brief Changes the work stealing strategy at runtime.
     * @tparam Strategy The new strategy class template (must implement IStealingStrategy).
     */
    template <template <typename> class Strategy>
    void ChangeStealingStrategy()
    {
      stealingStrategy_ = std::make_unique<Strategy<QueueType>>();
    }

  private:
    QueueType globalQueue_;
    std::vector<std::unique_ptr<QueueType>> localQueues_;
    std::vector<std::thread> threads_;
    std::atomic<bool> running_{false};
    std::mutex mutex_;
    std::condition_variable cv_;
    std::atomic<size_t> amountOfActiveThreads_{0};
    std::unique_ptr<IStealingStrategy<QueueType>> stealingStrategy_ = std::make_unique<RoundRobinStealingStrategy<QueueType>>();

    static thread_local int thread_index_;

    /**
     * @brief Main worker loop for each thread.
     * @param index Index of the worker thread.
     */
    void WorkerLoop(size_t index)
    {
      thread_index_ = static_cast<int>(index);
      while (running_ || HasWork())
      {
        TaskType task;
        if (localQueues_[index]->Dequeue(task) || globalQueue_.Dequeue(task) || TryStealFromOtherWorkers(index, task))
        {
          ActiveThreadCounter activeThreadCounter(amountOfActiveThreads_);
          task();
          continue;
        }

        // 4) nothing found -> block until new work arrives or shutdown
        std::unique_lock<std::mutex> lock(mutex_);
        cv_.wait(lock, [this]
                 { return !running_ || HasWork(); });
        // then loop repeats (checks all queues again)
      }
      thread_index_ = -1;
    }

    /**
     * @brief Tries to steal a task from other workers using the current stealing strategy.
     * @param selfIndex Index of the current worker.
     * @param task Reference to store the stolen task.
     * @return True if a task was stolen, false otherwise.
     */
    bool TryStealFromOtherWorkers(size_t selfIndex, TaskType &task)
    {
      return stealingStrategy_->TryStealFromOtherWorkers(localQueues_, selfIndex, task);
    }

    /**
     * @brief Checks if there is any work left in the global or local queues.
     * @return True if there is work, false otherwise.
     */
    bool HasWork() const
    {
      if (!globalQueue_.IsEmpty())
        return true;
      for (const auto &q : localQueues_)
      {
        if (!q->IsEmpty())
          return true;
      }
      return false;
    }
  };

  template <WorkStealingQueueConcept QueueType>
  thread_local int StaticThreadPool<QueueType>::thread_index_ = -1;
}

#endif // LUMADI_DETAIL_THREADPOOL_STATICTHREADPOOL_H