#ifndef LUMADI_THREAD_POOL_CONCEPTS_H
#define LUMADI_THREAD_POOL_CONCEPTS_H

#include <concepts>
#include <functional>

using Task = std::function<void()>;

template <typename Q>
concept WorkStealingQueueConcept = requires(Q q, Task t) {
  { q.Enqueue(t) };

  { q.Dequeue(t) } -> std::convertible_to<bool>;

  { q.Steal(t) } -> std::convertible_to<bool>;

  { q.IsEmpty() } -> std::convertible_to<bool>;

  { q.Stop() };
};

#endif // LUMADI_THREAD_POOL_CONCEPTS_H
