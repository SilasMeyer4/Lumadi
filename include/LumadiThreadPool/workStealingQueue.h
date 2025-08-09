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
    void Enqueue(Task task) override;
    bool Dequeue(Task &task) override;
    bool WaitDequeue(Task &task) override;
    void NotifyOne() override;
    void NotifyAll() override;
    bool IsEmpty() const override;
    bool Steal(Task &task) override;
    void Stop() override;

  private:
    mutable std::mutex mutex_;
    std::deque<Task> queue_;
    std::condition_variable cv_;
    bool stopped_ = false;
  };
}

#endif // WORKSTEALINGQUEUE_H