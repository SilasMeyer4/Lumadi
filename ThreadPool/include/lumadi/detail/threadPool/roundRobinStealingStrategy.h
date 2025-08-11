#ifndef LUMADI_DETAIL_THREADPOOL_ROUNDROBINSTEALINGSTRATEGY_H
#define LUMADI_DETAIL_THREADPOOL_ROUNDROBINSTEALINGSTRATEGY_H

#include "IStealingStrategy.h"
#include <vector>
#include <memory>

namespace Lumadi
{
  template <typename QueueType>
  class RoundRobinStealingStrategy : public IStealingStrategy<QueueType>
  {
  public:
    bool TryStealFromOtherWorkers(const std::vector<std::unique_ptr<QueueType>> &localQueues, size_t selfIndex, TaskType &task) override
    {
      size_t n = localQueues.size();
      for (size_t offset = 1; offset < n; ++offset)
      {
        size_t victim = (selfIndex + offset) % n;
        if (localQueues[victim]->Steal(task))
          return true;
      }
      return false;
    }
  };
}

#endif // LUMADI_DETAIL_THREADPOOL_ROUNDROBINSTEALINGSTRATEGY_H