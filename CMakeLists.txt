cmake_minimum_required(VERSION 3.10)
project(MatrixLibrary)

add_executable(main main.cpp MatrixLibrary.cpp)

find_package(benchmark REQUIRED)
add_executable(benchmark_matrix benchmark.cpp MatrixLibrary.cpp)
target_link_libraries(benchmark_matrix PRIVATE benchmark::benchmark)
