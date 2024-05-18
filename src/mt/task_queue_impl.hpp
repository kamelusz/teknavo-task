#pragma once
#include <utility>

namespace teknavo {
namespace detail {

struct TaskQueueAccess {
  template <typename Task>
  static Task* Next(Task* task) {
    return static_cast<Task*>(task->next_);
  }

  template <typename Task1, typename Task2>
  static void Next(Task1*& task1, Task2* task2) {
    task1->next_ = task2;
  }

  template <typename Task>
  static void Destroy(Task* task) {
    task->Destroy();
  }
};

template <typename Task>
class TaskQueueImpl {
 public:
  TaskQueueImpl() = default;
  TaskQueueImpl(const TaskQueueImpl&) = delete;
  const TaskQueueImpl& operator=(const TaskQueueImpl&) = delete;

  ~TaskQueueImpl() {
    while (Task* op = front_) {
      Pop();
      TaskQueueAccess::Destroy(op);
    }
  }

  void swap(TaskQueueImpl& other) noexcept {
    using std::swap;

    swap(other.front_, front_);
    swap(other.back_, back_);
  }

  Task* Front() noexcept { return front_; }

  void Pop() {
    if (front_) {
      Task* tmp = front_;
      front_ = TaskQueueAccess::Next(front_);
      if (!front_) {
        back_ = nullptr;
      }
      TaskQueueAccess::Next(tmp, static_cast<Task*>(nullptr));
    }
  }

  void Push(Task* task) {
    TaskQueueAccess::Next(task, static_cast<Task*>(nullptr));
    if (back_) {
      TaskQueueAccess::Next(back_, task);
      back_ = task;
    } else {
      front_ = back_ = task;
    }
  }

  bool Empty() const noexcept { return front_ == 0; }

 private:
  Task* front_{nullptr};
  Task* back_{nullptr};
};

template <typename Task>
void swap(TaskQueueImpl<Task>& lhs, TaskQueueImpl<Task>& rhs) {
  lhs.swap(rhs);
}

}  // namespace detail
}  // namespace teknavo
