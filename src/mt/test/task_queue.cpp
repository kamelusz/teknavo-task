#include "mt/task_queue.hpp"

#include <thread>

#include "common.hpp"
#include "mt/task.hpp"

namespace teknavo {
namespace test {
namespace {

TEST(TaskQueue, ExecuteAllTasks) {
  int value = 0;
  TaskQueue queue;

  queue.Post([&] { ++value; });
  queue.Post([&] { ++value; });
  queue.Post([&] { ++value; });

  queue.ProcessAllTasks();
  ASSERT_EQ(value, 3);
}

TEST(TaskQueue, ExecuteOneByOneTasks) {
  int value = 0;
  TaskQueue queue;

  queue.Post([&] { value += 1; });
  queue.ProcessOneTask();
  ASSERT_EQ(value, 1);

  queue.Post([&] { value += 2; });
  queue.ProcessOneTask();
  ASSERT_EQ(value, 3);

  queue.Post([&] { value += 3; });
  queue.ProcessOneTask();
  ASSERT_EQ(value, 6);
}

TEST(TaskQueue, ExecuteOneByOneTasksGrouped) {
  int value = 0;
  TaskQueue queue;

  queue.Post([&] { value += 1; });
  queue.Post([&] { value += 2; });
  queue.Post([&] { value += 3; });

  queue.ProcessOneTask();
  ASSERT_EQ(value, 1);

  queue.ProcessOneTask();
  ASSERT_EQ(value, 3);

  queue.ProcessOneTask();
  ASSERT_EQ(value, 6);
}

TEST(TaskQueue, PostFromAnotherThreadOneByOne) {
  int value = 0;
  TaskQueue queue;

  std::thread t1{[&] { queue.Post([&] { value += 2; }); }};
  queue.ProcessOneTask(true);
  ASSERT_EQ(value, 2);
  t1.join();

  std::thread t2{[&] { queue.Post([&value] { value += 3; }); }};
  queue.ProcessOneTask(true);
  ASSERT_EQ(value, 5);
  t2.join();
}

TEST(TaskQueue, PostFromAnotherThreadSametime) {
  int value = 0;
  TaskQueue queue;

  std::thread t1{[&] { queue.Post([&] { value += 2; }); }};
  std::thread t2{[&] { queue.Post([&] { value += 3; }); }};

  t1.join();
  t2.join();

  queue.ProcessAllTasks(true);
  ASSERT_EQ(value, 5);
}

}  // namespace
}  // namespace test
}  // namespace cafe
