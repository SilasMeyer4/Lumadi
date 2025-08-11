
#include <catch2/catch_test_macros.hpp>
#include "lumadi/detail/threadPool/staticThreadPool.h"
#include <future>
#include <atomic>
#include <thread>

using namespace Lumadi;

TEST_CASE("StaticThreadPool executes simple tasks", "[StaticThreadPool]")
{
  StaticThreadPool<WorkStealingQueue> pool(2);
  std::atomic<int> counter{0};
  auto f1 = pool.AddTask([&]
                         { counter++; });
  auto f2 = pool.AddTask([&]
                         { counter += 2; });
  f1.Get();
  f2.Get();
  REQUIRE(counter == 3);
}

TEST_CASE("StaticThreadPool returns futures", "[StaticThreadPool]")
{
  StaticThreadPool<WorkStealingQueue> pool(2);
  auto fut = pool.AddTask([]
                          { return 42; });
  REQUIRE(fut.Get() == 42);
}

TEST_CASE("StaticThreadPool can process many tasks in parallel", "[StaticThreadPool]")
{
  StaticThreadPool<WorkStealingQueue> pool(4);
  std::atomic<int> sum{0};
  std::vector<Lumadi::Task<void>> futures;
  for (int i = 0; i < 100; ++i)
  {
    futures.push_back(pool.AddTask([&sum]
                                   { sum++; }));
  }
  for (auto &f : futures)
    f.Get();
  REQUIRE(sum == 100);
}

TEST_CASE("StaticThreadPool waits for tasks before destruction", "[StaticThreadPool]")
{
  std::promise<void> p;
  std::future<void> f = p.get_future();
  {
    StaticThreadPool<WorkStealingQueue> pool(2);
    pool.AddTask([&p]
                 {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            p.set_value(); });
  }
  // The pool should not destruct until the task is done and the promise is set
  REQUIRE(f.wait_for(std::chrono::seconds(0)) == std::future_status::ready);
}

TEST_CASE("StaticThreadPool reports active thread count", "[StaticThreadPool]")
{
  StaticThreadPool<WorkStealingQueue> pool(2);
  std::atomic<bool> running{true};
  auto f1 = pool.AddTask([&]
                         {
    while (running.load()) {
      std::this_thread::sleep_for(std::chrono::milliseconds(10));
    } });
  // time for the thread
  std::this_thread::sleep_for(std::chrono::milliseconds(50));
  size_t active = pool.GetActiveThreadCount();
  REQUIRE(active >= 1);
  running = false;
  f1.Get();
}

// Dummy-Strategy for Test
template <typename QueueType>
class NoStealStrategy : public IStealingStrategy<QueueType>
{
public:
  bool TryStealFromOtherWorkers(const std::vector<std::unique_ptr<QueueType>> &, size_t, TaskType &) override
  {
    return false;
  }
};

TEST_CASE("StaticThreadPool can change stealing strategy at runtime", "[StaticThreadPool]")
{
  StaticThreadPool<WorkStealingQueue> pool(2);
  // Default Round robin.
  pool.ChangeStealingStrategy<NoStealStrategy>();
  // It should still be possible to process tasks
  auto f = pool.AddTask([]
                        { return 123; });
  REQUIRE(f.Get() == 123);
}