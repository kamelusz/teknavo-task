#pragma once
#include <functional>
#include <memory>

#include "mt/task_queue_impl.hpp"

namespace teknavo {

class TaskQueue;

class TaskBase {
  using Callback = void (*)(TaskBase* self, bool execute);

 public:
  virtual ~TaskBase() = default;

  void Complete() { callback_(this, true); }
  void Destroy() { callback_(this, false); }

 protected:
  TaskBase(Callback callback) : callback_{callback} {}

 private:
  friend detail::TaskQueueAccess;

  Callback callback_;
  TaskBase* next_{nullptr};
};

template <typename Handler>
class Task : public TaskBase {
 public:
  template <typename Fn>
  Task(Fn&& fn) : TaskBase(DoCall), fn_{std::forward<Fn>(fn)} {}
  ~Task() override = default;

 private:
  static void DoCall(TaskBase* base, bool execute) {
    auto* self = static_cast<Task*>(base);
    if (execute) {
      std::invoke(self->fn_);
    }
  }

 private:
  Handler fn_;
};

}  // namespace teknavo
