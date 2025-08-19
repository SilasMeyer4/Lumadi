#include <catch2/catch_test_macros.hpp>
#include "lumadi/detail/threadPool/roundRobinStealingStrategy.h"
#include "lumadi/detail/threadPool/workStealingQueue.h"
#include <vector>
#include <memory>

using namespace Lumadi;

TEST_CASE("RoundRobinStealingStrategy steals from other workers in round robin order", "[RoundRobinStealingStrategy]")
{
  using Queue = WorkStealingQueue;
  RoundRobinStealingStrategy<Queue> strategy;
  std::vector<std::unique_ptr<Queue>> queues;
  for (int i = 0; i < 3; ++i)
    queues.push_back(std::make_unique<Queue>());

  // Put a task in queue 1
  bool called = false;
  queues[1]->Enqueue([&]
                     { called = true; });
  TaskType task;
  // Worker 0 tries to steal
  bool result = strategy.TryStealFromOtherWorkers(queues, 0, task);
  REQUIRE(result);
  task();
  REQUIRE(called);
}

TEST_CASE("RoundRobinStealingStrategy does not steal from self", "[RoundRobinStealingStrategy]")
{
  using Queue = WorkStealingQueue;
  RoundRobinStealingStrategy<Queue> strategy;
  std::vector<std::unique_ptr<Queue>> queues;
  for (int i = 0; i < 2; ++i)
    queues.push_back(std::make_unique<Queue>());

  // Put a task in own queue
  bool called = false;
  queues[0]->Enqueue([&]
                     { called = true; });
  TaskType task;
  // Worker 0 tries to steal (should not steal from self)
  bool result = strategy.TryStealFromOtherWorkers(queues, 0, task);
  REQUIRE_FALSE(result);
  REQUIRE_FALSE(called);
}

TEST_CASE("RoundRobinStealingStrategy returns false if no tasks available", "[RoundRobinStealingStrategy]")
{
  using Queue = WorkStealingQueue;
  RoundRobinStealingStrategy<Queue> strategy;
  std::vector<std::unique_ptr<Queue>> queues;
  for (int i = 0; i < 2; ++i)
    queues.push_back(std::make_unique<Queue>());
  TaskType task;
  bool result = strategy.TryStealFromOtherWorkers(queues, 0, task);
  REQUIRE_FALSE(result);
}
