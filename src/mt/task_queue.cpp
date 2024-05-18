#include "mt/task_queue.hpp"

namespace teknavo {

void TaskQueue::HandleNextTask() {
  auto raw_ptr = internal_tasks_.Front();
  auto task = std::unique_ptr<TaskBase>(raw_ptr);
  internal_tasks_.Pop();
  task->Complete();
}

void TaskQueue::PostQuit() {
  {
    std::unique_lock lock{mutex_};
    if (quit_enqueued_) {
      return;
    }

    quit_enqueued_ = true;
  }

  Post([this] { quit_ = true; });
}

void TaskQueue::ProcessOneTask(bool wait) {
  if (!internal_tasks_.Empty() || AcquireTasks(wait)) {
    HandleNextTask();
  }
}

void TaskQueue::ProcessAllTasks(bool wait) {
  if (!internal_tasks_.Empty() || AcquireTasks(wait)) {
    do {
      HandleNextTask();
    } while (!internal_tasks_.Empty());
  }
}

bool TaskQueue::Enqueue(TaskBase& task) {
  std::unique_lock<std::mutex> lock{mutex_};

  external_tasks_.Push(&task);
  WakeAndUnlock(lock);

  return true;
}

bool TaskQueue::AcquireTasks(bool wait) {
  using std::swap;

  std::unique_lock lock{mutex_};
  if (wait && external_tasks_.Empty()) {
    wakeup_event_.Clear(lock);
    wakeup_event_.Wait(lock);
  }

  swap(internal_tasks_, external_tasks_);
  lock.unlock();

  return !internal_tasks_.Empty();
}

template <typename Lock>
void TaskQueue::WakeAndUnlock(Lock& lock) {
  if (!wakeup_event_.MaybeUnlockAndSignal(lock)) {
    lock.unlock();
  }
}

}  // namespace teknavo
