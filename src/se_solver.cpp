#include <array>
#include <charconv>
#include <iostream>
#include <thread>

#include "solver/execution_manager.hpp"

int main(int argc, char** argv) {
  teknavo::ExecutionManager manager{argc, argv};
  manager.Run();
  manager.Stop();

  return 0;
}
