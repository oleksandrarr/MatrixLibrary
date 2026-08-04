cmake_minimum_required(VERSION 3.10)
project(MatrixLibrary)
set(CMAKE_CXX_STANDARD 20)

#general
add_library(MatrixLibrary src/MatrixLibrary.cpp)
target_include_directories(MatrixLibrary PUBLIC include)

#tests
#google tests
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/heads/main.zip
)
FetchContent_MakeAvailable(googletest)

enable_testing()
add_executable(tests tests/tests.cpp)
target_link_libraries(tests PRIVATE MatrixLibrary GTest::gtest_main)
include(GoogleTest)
gtest_discover_tests(tests)

#benchmarks
option(BUILD_BENCHMARKS "Build benchmarks" OFF)
if(BUILD_BENCHMARKS)
  find_package(benchmark REQUIRED)
  add_executable(benchmark_matrix benchmarks/benchmark_matrix.cpp)
  target_link_libraries(benchmark_matrix PRIVATE MatrixLibrary benchmark::benchmark)
endif()


