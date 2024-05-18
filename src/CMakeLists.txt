project(se_solver CXX C)

add_executable(se_solver se_solver.cpp)
target_link_libraries(se_solver 
  teknavo_solver)
target_include_directories(se_solver
  PUBLIC 
    ../)

add_subdirectory(mt)
add_subdirectory(solver)
