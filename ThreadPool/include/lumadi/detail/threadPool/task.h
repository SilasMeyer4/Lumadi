#ifndef LUMADI_DETAIL_THREADPOOL_TASK_H
#define LUMADI_DETAIL_THREADPOOL_TASK_H

#include <functional>
#include <future>

namespace Lumadi
{

  /**
   * @brief Wrapper class for an asynchronous task returning a result of type T.
   *
   * Provides access to the result and waiting for completion.
   *
   * @tparam T Return type of the task.
   */
  template <typename T>
  class Task
  {
  public:
    /**
     * @brief Constructor. Takes ownership of a shared_future.
     * @param future The shared_future object holding the result.
     */
    Task(std::shared_future<T> &&future) : future_(std::move(future)) {}

    /**
     * @brief Gets the result of the task (blocks if not ready).
     * @return The result of type T.
     */
    T Get() { return future_.get(); }

    /**
     * @brief Blocks until the result is available.
     */
    void Wait() { return future_.wait(); }

  private:
    std::shared_future<T> future_;
  };

}

#endif // LUMADI_DETAIL_THREADPOOL_TASK_H