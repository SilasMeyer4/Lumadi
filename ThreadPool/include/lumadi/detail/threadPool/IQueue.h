#ifndef LUMADI_IQUEUE_H
#define LUMADI_IQUEUE_H

#include <functional>

namespace Lumadi
{

  using TaskType = std::function<void()>;

  class IStealingQueue
  {
  public:
    virtual ~IStealingQueue() = default;

    virtual void Enqueue(TaskType task) = 0;
    virtual bool Dequeue(TaskType &task) = 0;
    virtual bool IsEmpty() const = 0;
    virtual bool WaitDequeue(TaskType &task) = 0;
    virtual void NotifyOne() = 0;
    virtual void NotifyAll() = 0;
    virtual void Stop() = 0;
    virtual bool Steal(TaskType &task) = 0;
  };
}

#endif // LUMADI_IQUEUE_H