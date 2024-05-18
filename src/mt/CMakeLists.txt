project(teknavo_mt)

set(LIB_SRC
  event.hpp
  task.hpp
  task_queue.hpp
  task_queue.cpp
  task_queue_impl.hpp
  execution_context.hpp
)
  
add_library(${PROJECT_NAME} STATIC ${LIB_SRC})
set_property(TARGET ${PROJECT_NAME} PROPERTY FOLDER "lib")
target_include_directories(${PROJECT_NAME} PUBLIC ../)

if(TEKNAVO_BUILD_TESTS)
  add_subdirectory(test)
endif()

if(TEKNAVO_SANITIZE_ADDRESS)
  teknavo_enable_address_sanitizer(${PROJECT_NAME})
endif()

if(TEKNAVO_SANITIZE_THREAD)
  teknavo_enable_thread_sanitizer(${PROJECT_NAME})
endif()
