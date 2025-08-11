#ifndef LUMADI_THREAD_POOL_CONCEPTS_H
#define LUMADI_THREAD_POOL_CONCEPTS_H

#include <concepts>
#include <functional>

using TaskType = std::function<void()>;

template <typename Q>
concept WorkStealingQueueConcept = requires(Q q, TaskType t) {
  { q.Enqueue(t) };

  { q.Dequeue(t) } -> std::convertible_to<bool>;

  { q.Steal(t) } -> std::convertible_to<bool>;

  { q.IsEmpty() } -> std::convertible_to<bool>;

  { q.Stop() };
};

#endif // LUMADI_THREAD_POOL_CONCEPTS_H
