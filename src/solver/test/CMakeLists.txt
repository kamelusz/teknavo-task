project(teknavo_solver_test)

include(${CMAKE_SOURCE_DIR}/cmake/utils.cmake)

find_package(GTest REQUIRED)

include(GoogleTest)
enable_testing()

set(TEST_SRC
  main.cpp
  common.hpp
  trey_calculation.cpp)
  
add_executable(${PROJECT_NAME} ${TEST_SRC})
target_link_libraries(${PROJECT_NAME} 
  PRIVATE
    teknavo_solver
    GTest::GTest)

set_property(TARGET ${PROJECT_NAME} PROPERTY FOLDER "tests")
add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})

if(TEKNAVO_SANITIZE_ADDRESS)
  teknavo_enable_address_sanitizer(${PROJECT_NAME})
endif()

if(TEKNAVO_SANITIZE_THREAD)
  teknavo_enable_thread_sanitizer(${PROJECT_NAME})
endif()
