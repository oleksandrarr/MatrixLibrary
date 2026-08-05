cmake_minimum_required(VERSION 3.10)
project(MatrixLibrary)
set(CMAKE_CXX_STANDARD 20)

#general
add_library(MatrixLibrary src/MatrixLibrary.cpp src/MatrixMultiplication.cpp)
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
set(AVAILABLE_BENCHMARKS 
	reordered 
	blocking)
	
set(BENCHMARKS_TO_BUILD "all" CACHE STRING "Benchmarks to build")

if(BUILD_BENCHMARKS)
  find_package(benchmark REQUIRED)
  if(BENCHMARKS_TO_BUILD STREQUAL "all")
    set(SELECTED_BENCHMARKS ${AVAILABLE_BENCHMARKS})
  else()
    set(SELECTED_BENCHMARKS ${BENCHMARKS_TO_BUILD})
  endif()
  foreach(BENCH ${SELECTED_BENCHMARKS})
    add_executable(${BENCH}_benchmark benchmarks/${BENCH}_benchmark.cpp)
    target_link_libraries(${BENCH}_benchmark PRIVATE MatrixLibrary benchmark::benchmark)
  endforeach()
endif()


