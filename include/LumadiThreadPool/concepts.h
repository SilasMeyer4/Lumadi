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