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
add_executable(tests tests/tests.cpp tests/tests_multiplication.cpp)
target_link_libraries(tests PRIVATE MatrixLibrary GTest::gtest_main)
include(GoogleTest)
gtest_discover_tests(tests)

#benchmarks
option(BUILD_BENCHMARKS "Build benchmarks" OFF)
if(BUILD_BENCHMARKS)
  find_package(benchmark REQUIRED)
  add_executable(reordered_benchmark benchmarks/reordered_benchmark.cpp)
  target_link_libraries(reordered_benchmark PRIVATE MatrixLibrary benchmark::benchmark)
  
  add_executable(blocking_benchmark benchmarks/blocking_benchmark.cpp)
  target_link_libraries(blocking_benchmark PRIVATE MatrixLibrary benchmark::benchmark)
endif()


