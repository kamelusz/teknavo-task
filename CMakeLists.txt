cmake_minimum_required(VERSION 3.10)
project(teknavo_test CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# sanitizer options
option(TEKNAVO_SANITIZE_ADDRESS "Enable address sanitizer in tests" OFF)
option(TEKNAVO_SANITIZE_THREAD "Enable thread sanitizer in tests" OFF)

# testing options
option(TEKNAVO_BUILD_TESTS "Build tests" OFF)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

if(UNIX)
  add_compile_options(-Wall -Wextra -Werror -pedantic)
endif(UNIX)

if(TEKNAVO_BUILD_TESTS)
    message(STATUS "Enabling tests")
    enable_testing()
endif()

add_subdirectory(src)
