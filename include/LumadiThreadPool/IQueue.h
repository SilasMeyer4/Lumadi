#ifndef LUMADI_IQUEUE_H
#define LUMADI_IQUEUE_H

#include <functional>

namespace Lumadi
{

  using Task = std::function<void()>;

  class IStealingQueue
  {
  public:
    virtual ~IStealingQueue() = default;

    virtual void Enqueue(Task task) = 0;
    virtual bool Dequeue(Task &task) = 0;
    virtual bool IsEmpty() const = 0;
    virtual bool WaitDequeue(Task &task) = 0;
    virtual void NotifyOne() = 0;
    virtual void NotifyAll() = 0;
    virtual void Stop() = 0;
    virtual bool Steal(Task &task) = 0;
  };
}

#endif // LUMADI_IQUEUE_H