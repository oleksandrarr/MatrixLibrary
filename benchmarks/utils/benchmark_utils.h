#ifndef BENCHMARK_UTILS_H
#define BENCHMARK_UTILS_H

#include <benchmark/benchmark.h>
#include <MatrixLibrary.h>

template<typename Func>
static void MultiplicationTemplate(benchmark::State& state, Func multiply){ 
  const int n = state.range(0); //for custom arguments and sizes
  
  Matrix A(n, n, 0, 100);
  Matrix B(n, n, 0, 100);
  
  for(auto _ : state) { //temporized
    Matrix C = multiply(A,B);
    benchmark::DoNotOptimize(C);
  }
  
  const double gflops = (2.0 * n*n*n) / 1e9;
  state.counters["GFLOPS"] = benchmark::Counter(gflops, benchmark::Counter::kIsIterationInvariantRate);
}
#endif
