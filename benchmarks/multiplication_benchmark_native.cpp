#include <benchmark/benchmark.h>
#include <MatrixLibrary.h>

static void BM_Multiplication(benchmark::State& state){ 
  const int n = state.range(0); //for custom arguments and sizes
  
  Matrix A(n, n, 0, 100);
  Matrix B(n, n, 0, 100);
  
  for(auto _ : state) { //temporized
    Matrix C = A*B;
    benchmark::DoNotOptimize(C);
  }
  
  const double gflops = (2.0 * n*n*n) / 1e9;
  state.counters["GFLOPS"] = benchmark::Counter(gflops, benchmark::Counter::kIsIterationInvariantRate);
  
}

BENCHMARK(BM_Multiplication)
  ->RangeMultiplier(2)
  ->Range(64, 4096);
BENCHMARK_MAIN();
