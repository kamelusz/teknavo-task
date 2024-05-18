#pragma once
#include <thread>

#include "mt/task_queue.hpp"

namespace teknavo {

class ExecutionContext {
 public:
  void Run() {
    executor_ = std::thread{[this] { queue_.Run(); }};
  }

  void Stop() {
    queue_.PostQuit();
    executor_.join();
  }

  template <typename Fn>
  void Post(Fn&& fn) {
    queue_.Post(std::forward<Fn>(fn));
  }

 private:
  teknavo::TaskQueue queue_;
  std::thread executor_;
};

}  // namespace teknavo
