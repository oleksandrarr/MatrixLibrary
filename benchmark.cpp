#include <benchmark/benchmark.h>
#include "MatrixLibrary.h"

static void BM_Multiplication(benchmark::State& state){ 
  const size_t n = state.range(0); //for custom arguments and sizes
  
  Matrix A(n, n, 0, 100);
  Matrix B(n, n, 0, 100);
  
  for(auto _ : state) { //temporized
    Matrix C = A*B;
    benchmark::DoNotOptimize(C);
  }
}

BENCHMARK(BM_Multiplication)
  ->Arg(64)
  ->Arg(128)
  ->Arg(256)
  ->Arg(512)
  ->Arg(1024)
  ->Arg(2048)
  ->Arg(4096);
BENCHMARK_MAIN();
