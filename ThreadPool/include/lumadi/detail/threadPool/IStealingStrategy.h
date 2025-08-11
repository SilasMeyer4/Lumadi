#ifndef LUMADI_DETAIL_THREADPOOL_ISTEALINGSTRATEGY_H
#define LUMADI_DETAIL_THREADPOOL_ISTEALINGSTRATEGY_H

#include <vector>
#include <memory>
#include "concepts.h"

namespace Lumadi
{
  template <typename QueueType>
  class IStealingStrategy
  {
  public:
    virtual ~IStealingStrategy() = default;
    virtual bool TryStealFromOtherWorkers(const std::vector<std::unique_ptr<QueueType>> &localQueues, size_t selfIndex, TaskType &task) = 0;
  };
}

#endif // LUMADI_DETAIL_THREADPOOL_ISTEALINGSTRATEGY_H