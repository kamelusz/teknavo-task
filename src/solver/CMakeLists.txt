project(teknavo_solver)

set(LIB_SRC
  trey.cpp
  trey.hpp
  execution_manager.hpp
  execution_manager.cpp
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
  
add_library(${PROJECT_NAME} STATIC ${LIB_SRC})
set_property(TARGET ${PROJECT_NAME} PROPERTY FOLDER "lib")
target_include_directories(${PROJECT_NAME} PUBLIC ../)
target_link_libraries(${PROJECT_NAME} 
  PUBLIC
    teknavo_mt)

if(TEKNAVO_BUILD_TESTS)
  add_subdirectory(test)
endif()

if(TEKNAVO_SANITIZE_ADDRESS)
  teknavo_enable_address_sanitizer(${PROJECT_NAME})
endif()

if(TEKNAVO_SANITIZE_THREAD)
  teknavo_enable_thread_sanitizer(${PROJECT_NAME})
endif()
