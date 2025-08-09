#include <catch2/catch_test_macros.hpp>
#include "staticThreadPool.h"
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
  f1.get();
  f2.get();
  REQUIRE(counter == 3);
}

TEST_CASE("StaticThreadPool returns futures", "[StaticThreadPool]")
{
  StaticThreadPool<WorkStealingQueue> pool(2);
  auto fut = pool.AddTask([]
                          { return 42; });
  REQUIRE(fut.get() == 42);
}

TEST_CASE("StaticThreadPool can process many tasks in parallel", "[StaticThreadPool]")
{
  StaticThreadPool<WorkStealingQueue> pool(4);
  std::atomic<int> sum{0};
  std::vector<std::future<void>> futures;
  for (int i = 0; i < 100; ++i)
  {
    futures.push_back(pool.AddTask([&sum]
                                   { sum++; }));
  }
  for (auto &f : futures)
    f.get();
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
