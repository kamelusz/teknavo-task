#pragma once
#include <condition_variable>
#include <mutex>

namespace teknavo {

template <bool Autoreset>
class Event {
  enum class State { kSignaled, kNonsignaled };

 public:
  Event(bool inited = false)
      : state_{inited ? State::kSignaled : State::kNonsignaled} {}

  Event(const Event&) = delete;
  Event& operator=(const Event&) = delete;

  void Wait() {
    std::unique_lock lock{mutex_};
    cv_.wait(lock, [this] { return state_ == State::kSignaled; });
    if constexpr (Autoreset) {
      state_ = State::kNonsignaled;
    }
  }

  template <typename Rep, typename Period>
  bool WaitFor(const std::chrono::duration<Rep, Period>& timeout) {
    auto result = false;
    std::unique_lock lock{mutex_};
    if (state_ == State::kNonsignaled) {
      result = cv_.wait_for(lock, timeout,
                            [this] { return state_ == State::kSignaled; });
    }

    if (state_ == State::kSignaled) {
      result = true;
      if constexpr (Autoreset) {
        state_ = State::kNonsignaled;
      }
    }

    return result;
  }

  void Signal() {
    std::unique_lock lock{mutex_};
    if (state_ == State::kNonsignaled) {
      state_ = State::kSignaled;
      if constexpr (Autoreset) {
        cv_.notify_one();
      } else {
        cv_.notify_all();
      }
    }
  }

  void Reset() {
    std::unique_lock lock{mutex_};
    state_ = State::kNonsignaled;
  }

 private:
  mutable std::mutex mutex_;
  std::condition_variable cv_;
  State state_;
};

}  // namespace teknavo
