#pragma once
#include <cassert>
#include <condition_variable>
#include <mutex>

#include <queue>

#include "mt/task.hpp"
#include "mt/task_queue_impl.hpp"

namespace teknavo {
namespace detail {

class QueueEvent {
 public:
  QueueEvent() = default;
  QueueEvent(const QueueEvent&) = delete;
  QueueEvent& operator=(const QueueEvent&) = delete;

  template <typename Lock>
  void Wait(Lock& lock) {
    assert(lock.owns_lock());
    LockAdapter lock_guard(lock);
    while (!signaled_) {
      Waiter waiter_guard(has_waiter_);
      cv_.wait(lock_guard.lock_);
    }
  }

  template <typename Lock>
  void UnlockAndSignal(Lock& lock) {
    assert(lock.owns_lock());
    signaled_ = true;
    lock.unlock();
    if (has_waiter_) {
      cv_.notify_one();
    }
  }

  template <typename Lock>
  bool MaybeUnlockAndSignal(Lock& lock) {
    assert(lock.owns_lock());
    signaled_ = true;
    if (has_waiter_) {
      lock.unlock();
      cv_.notify_one();
      return true;
    }
    return false;
  }

  template <typename Lock>
  void Clear(Lock& lock) {
    assert(lock.owns_lock());
    (void)lock;
    signaled_ = false;
  }

 private:
  class Waiter {
   public:
    explicit Waiter(bool& state) : state_{state} { state_ = true; }
    ~Waiter() { state_ = false; }

   private:
    bool& state_;
  };

  class LockAdapter {
   public:
    template <typename Lock>
    explicit LockAdapter(Lock& lock) : lock_(*lock.mutex(), std::adopt_lock) {}
    ~LockAdapter() { lock_.release(); }

   private:
    friend class QueueEvent;
    std::unique_lock<std::mutex> lock_;
  };

 private:
  bool signaled_{false};
  bool has_waiter_{false};
  std::condition_variable cv_;
};

}  // namespace detail

class TaskQueue {
 public:
  TaskQueue() = default;

  template <typename Handler>
  bool Post(Handler&& fn) {
    using FnType = std::decay_t<Handler>;

    auto task = std::make_unique<Task<FnType>>(std::forward<Handler>(fn));
    const auto result = Enqueue(*task);
    if (result) {
      task.release();
    }

    return result;
  }

  void PostQuit();

  void Run() {
    while (!quit_) {
      ProcessOneTask(true);
    }
  }

  void ProcessOneTask(bool wait = false);
  void ProcessAllTasks(bool wait = false);

 private:
  bool Enqueue(TaskBase& task);
  bool AcquireTasks(bool wait);
  void HandleNextTask();
  template <typename Lock>
  void WakeAndUnlock(Lock& lock);

 private:
  mutable std::mutex mutex_;
  detail::QueueEvent wakeup_event_;
  detail::TaskQueueImpl<TaskBase> external_tasks_;
  detail::TaskQueueImpl<TaskBase> internal_tasks_;
  bool quit_enqueued_{false};
  bool quit_{false};
};

}  // namespace teknavo
