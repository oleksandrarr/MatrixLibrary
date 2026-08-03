cmake_minimum_required(VERSION 3.10)
project(MatrixLibrary)
set(CMAKE_CXX_STANDARD 20)

#general
add_library(MatrixLibrary src/MatrixLibrary.cpp)
target_include_directories(MatrixLibrary PUBLIC include)

#tests
add_executable(matrix_tests tests/matrix_tests.cpp)
target_link_libraries(matrix_tests PRIVATE MatrixLibrary)

#benchmarks
find_package(benchmark REQUIRED)
add_executable(benchmark_matrix benchmarks/multiplication_benchmark_native.cpp)
target_link_libraries(benchmark_matrix PRIVATE MatrixLibrary benchmark::benchmark)
