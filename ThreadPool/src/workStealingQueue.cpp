#include "lumadi/detail/threadPool/workStealingQueue.h"

void Lumadi::WorkStealingQueue::Enqueue(TaskType task)
{
  {
    std::lock_guard<std::mutex> lock(mutex_);
    queue_.push_back(std::move(task));
  }
  NotifyOne();
}

bool Lumadi::WorkStealingQueue::Dequeue(TaskType &task)
{
  std::lock_guard<std::mutex> lock(mutex_);
  if (queue_.empty())
  {
    return false;
  }
  task = std::move(queue_.front());
  queue_.pop_front();
  return true;
}

bool Lumadi::WorkStealingQueue::WaitDequeue(TaskType &task)
{
  std::unique_lock<std::mutex> lock(mutex_);
  cv_.wait(lock, [this]
           { return !queue_.empty() || stopped_; });
  if (queue_.empty())
  {
    return false;
  }

  task = std::move(queue_.front());
  queue_.pop_front();
  return true;
}

void Lumadi::WorkStealingQueue::NotifyOne()
{
  cv_.notify_one();
}

void Lumadi::WorkStealingQueue::NotifyAll()
{
  cv_.notify_all();
}

bool Lumadi::WorkStealingQueue::IsEmpty() const
{
  std::lock_guard<std::mutex> lock(mutex_);
  return queue_.empty();
}

bool Lumadi::WorkStealingQueue::Steal(TaskType &task)
{
  std::lock_guard<std::mutex> lock(mutex_);
  if (queue_.empty())
  {
    return false;
  }
  task = std::move(queue_.back());
  queue_.pop_back();
  return true;
}

void Lumadi::WorkStealingQueue::Stop()
{
  {
    std::lock_guard<std::mutex> lock(mutex_);
    stopped_ = true;
  }
  NotifyAll();
}
