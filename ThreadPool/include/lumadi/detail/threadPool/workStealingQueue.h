#ifndef WORKSTEALINGQUEUE_H
#define WORKSTEALINGQUEUE_H

#include "IQueue.h"
#include <deque>
#include <mutex>
#include <condition_variable>

namespace Lumadi
{
  class WorkStealingQueue : public IStealingQueue
  {
  public:
    void Enqueue(TaskType task) override;
    bool Dequeue(TaskType &task) override;
    bool WaitDequeue(TaskType &task) override;
    void NotifyOne() override;
    void NotifyAll() override;
    bool IsEmpty() const override;
    bool Steal(TaskType &task) override;
    void Stop() override;

  private:
    mutable std::mutex mutex_;
    std::deque<TaskType> queue_;
    std::condition_variable cv_;
    bool stopped_ = false;
  };
}

#endif // WORKSTEALINGQUEUE_H