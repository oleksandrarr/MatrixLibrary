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

static void BM_Multiplication(benchmark::State& state){
  MultiplicationTemplate(state, [](const Matrix& A, const Matrix& B){
    return A*B;
  });
}

static void BM_MultiplicationReorderedIKJ(benchmark::State& state){ 
  MultiplicationTemplate(state, [](const Matrix& A, const Matrix& B){
    return A.multiplyReorderedIKJ(B);
  });
}
static void BM_MultiplicationReorderedJIK(benchmark::State& state){ 
  MultiplicationTemplate(state, [](const Matrix& A, const Matrix& B){
    return A.multiplyReorderedJIK(B);
  });
}
static void BM_MultiplicationReorderedKIJ(benchmark::State& state){ 
  MultiplicationTemplate(state, [](const Matrix& A, const Matrix& B){
    return A.multiplyReorderedKIJ(B);
  });
}
static void BM_MultiplicationReorderedJKI(benchmark::State& state){ 
  MultiplicationTemplate(state, [](const Matrix& A, const Matrix& B){
    return A.multiplyReorderedJKI(B);
  });
}
static void BM_MultiplicationReorderedKJI(benchmark::State& state){ 
  MultiplicationTemplate(state, [](const Matrix& A, const Matrix& B){
    return A.multiplyReorderedKJI(B);
  });
}
int main(int argc, char** argv)
{
    std::cout << "=============================================================\n";
    std::cout << "              Matrix Multiplication Benchmark\n";
    std::cout << "=============================================================\n\n";

    benchmark::Initialize(&argc, argv);

    benchmark::RunSpecifiedBenchmarks();

    std::cout << "\nBenchmark finished.\n";
}
//BENCHMARK(BM_Multiplication)->Name("Naive (IJK)")->RangeMultiplier(2)->Range(64, 2048);
BENCHMARK(BM_MultiplicationReorderedIKJ)->Name("Reordered (IKJ)")->RangeMultiplier(2)->Range(64, 2048);
BENCHMARK(BM_MultiplicationReorderedJIK)->Name("Reordered (JIK)")->RangeMultiplier(2)->Range(64, 2048);
//BENCHMARK(BM_MultiplicationReorderedJKI)->Name("Reordered (JKI)")->RangeMultiplier(2)->Range(64, 2048);
//BENCHMARK(BM_MultiplicationReorderedKIJ)->Name("Reordered (KIJ)")->RangeMultiplier(2)->Range(64, 2048);
//BENCHMARK(BM_MultiplicationReorderedKJI)->Name("Reordered (KJI)")->RangeMultiplier(2)->Range(64, 2048);
//BENCHMARK_MAIN();
