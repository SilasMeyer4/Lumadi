#ifndef LUMADI_THREADPOOL_H
#define LUMADI_THREADPOOL_H

#include <vector>
#include <thread>
#include <queue>
#include <functional>
#include <future>
#include <mutex>
#include <condition_variable>
#include "IQueue.h"
namespace Lumadi
{
  class IThreadPool
  {
  public:
    IThreadPool() = default;
    ~IThreadPool() = default;
    virtual void AddTask(std::function<void()> task) = 0;
  };
}

#endif // LUMADI_THREADPOOL_H