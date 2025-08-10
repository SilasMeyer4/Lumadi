#include <catch2/catch_test_macros.hpp>
#include "Lumadi/workStealingQueue.h"
#include <thread>
#include <atomic>

using namespace Lumadi;

TEST_CASE("WorkStealingQueue basic enqueue/dequeue", "[WorkStealingQueue]")
{
  WorkStealingQueue queue;
  Task t = [] {};
  queue.Enqueue(t);
  Task out;
  REQUIRE(queue.Dequeue(out));
}

TEST_CASE("WorkStealingQueue WaitDequeue wakes up", "[WorkStealingQueue]")
{
  WorkStealingQueue queue;
  std::atomic<bool> done{false};
  Task t = [&done]
  { done = true; };
  std::thread producer([&]
                       {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        queue.Enqueue(t); });
  Task out;
  REQUIRE(queue.WaitDequeue(out));
  out();
  REQUIRE(done);
  producer.join();
}

TEST_CASE("WorkStealingQueue Steal steals from back", "[WorkStealingQueue]")
{
  WorkStealingQueue queue;
  int x = 0;
  queue.Enqueue([&]
                { x = 1; });
  queue.Enqueue([&]
                { x = 2; });
  Task stolen;
  REQUIRE(queue.Steal(stolen));
  stolen();
  REQUIRE(x == 2); // takes the last element
}

TEST_CASE("WorkStealingQueue Stop wakes WaitDequeue", "[WorkStealingQueue]")
{
  WorkStealingQueue queue;
  std::atomic<bool> finished{false};
  std::thread stopper([&]
                      {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        queue.Stop();
        finished = true; });
  Task out;
  REQUIRE_FALSE(queue.WaitDequeue(out));
  stopper.join();
  REQUIRE(finished);
}
