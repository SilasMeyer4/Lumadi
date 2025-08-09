

#include "IThreadPool.h"
#include "IQueue.h"
#include "workStealingQueue.h"
#include "concepts.h"

namespace Lumadi
{
  template <WorkStealingQueueConcept QueueType>
  class StaticThreadPool
  {
  public:
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

    template <typename Func, typename... Args>
    auto AddTask(Func &&f, Args &&...args) -> std::future<std::invoke_result_t<Func, Args...>>
    {
      using ReturnType = std::invoke_result_t<Func, Args...>;
      auto task = std::make_shared<std::packaged_task<ReturnType()>>(
          std::bind(std::forward<Func>(f), std::forward<Args>(args)...));
      auto result = task->get_future();

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
      return result;
    }

  private:
    QueueType globalQueue_;
    std::vector<std::unique_ptr<QueueType>> localQueues_;
    std::vector<std::thread> threads_;
    std::atomic<bool> running_{false};
    std::mutex mutex_;
    std::condition_variable cv_;

    static thread_local int thread_index_;

    void WorkerLoop(size_t index)
    {
      thread_index_ = static_cast<int>(index);
      while (running_ || HasWork())
      {
        Task task;
        if (localQueues_[index]->Dequeue(task))
        {
          task();
          continue;
        }

        // 2) global queue (non-blocking)
        if (globalQueue_.Dequeue(task))
        {
          task();
          continue;
        }

        // 3) try steal from other workers
        if (TryStealFromOtherWorkers(index, task))
        {
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

    bool TryStealFromOtherWorkers(size_t selfIndex, Task &task)
    {
      size_t n = localQueues_.size();
      for (size_t offset = 1; offset < n; ++offset)
      {
        size_t victim = (selfIndex + offset) % n;
        if (localQueues_[victim]->Steal(task))
          return true;
      }
      return false;
    }

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