#include "common.hpp"
#include "mt/event.hpp"
#include "mt/execution_context.hpp"

namespace teknavo {
namespace test {
namespace {

TEST(MT, PostLambda) {
  std::uint32_t value = 0;
  Event<true> event;

  teknavo::ExecutionContext runner;
  runner.Run();

  runner.Post([&] { value = 0xDEADBEEF; });
  runner.Post([&] { event.Signal(); });

  ASSERT_TRUE(event.WaitFor(std::chrono::milliseconds{100}));
  ASSERT_EQ(value, 0xDeadBeef);

  runner.Stop();
}

TEST(MT, PostFunctor) {
  class Functor {
   public:
    Functor(std::string& value, Event<true>& event)
        : value_{value}, event_{event} {}

    void operator()() {
      value_ = "I am the Great Cornholio!";
      event_.Signal();
    }

   private:
    std::string& value_;
    Event<true>& event_;
  };

  teknavo::ExecutionContext runner;
  runner.Run();

  std::string value;
  Event<true> event;

  runner.Post(Functor{value, event});
  ASSERT_TRUE(event.WaitFor(std::chrono::milliseconds{100}));
  ASSERT_EQ(value, "I am the Great Cornholio!");

  value.clear();
  ASSERT_EQ(value, "");

  Functor func{value, event};
  runner.Post(func);
  ASSERT_TRUE(event.WaitFor(std::chrono::milliseconds{100}));
  ASSERT_EQ(value, "I am the Great Cornholio!");

  runner.Stop();
}

TEST(MT, PostFunction) {
  teknavo::ExecutionContext runner;
  runner.Run();

  std::string value;
  Event<true> event;
  std::function<void()> function = [&] {
    value = "I am the Great Cornholio!";
    event.Signal();
  };

  runner.Post(function);
  ASSERT_TRUE(event.WaitFor(std::chrono::milliseconds{100}));
  ASSERT_EQ(value, "I am the Great Cornholio!");

  runner.Stop();
}

TEST(MT, PostFromSeveralThreads) {
  int value = 0;
  std::vector<std::thread> threads;

  teknavo::ExecutionContext runner;
  runner.Run();

  // post 100 tasks from 10 different threads
  for (int i = 0; i < 10; ++i) {
    threads.emplace_back([&] {
      for (auto j = 0; j < 10; ++j) {
        runner.Post([&] { ++value; });
      }
    });
  }

  Event<true> event;
  std::thread quit{[&] {
    for (auto& thread : threads) {
      thread.join();
    }

    event.Signal();
    runner.Stop();
  }};

  ASSERT_TRUE(event.WaitFor(std::chrono::milliseconds{10}));

  quit.join();

  ASSERT_EQ(value, 100);
}

}  // namespace
}  // namespace test
}  // namespace teknavo
