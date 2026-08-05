#include <benchmark/benchmark.h>
#include <MatrixLibrary.h>
#include "utils/benchmark_utils.h"

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
BENCHMARK(BM_Multiplication)->Name("Naive (IJK)")->RangeMultiplier(2)->Range(64, 2048);
BENCHMARK(BM_MultiplicationReorderedIKJ)->Name("Reordered (IKJ)")->RangeMultiplier(2)->Range(64, 2048);
BENCHMARK(BM_MultiplicationReorderedJIK)->Name("Reordered (JIK)")->RangeMultiplier(2)->Range(64, 2048);
BENCHMARK(BM_MultiplicationReorderedJKI)->Name("Reordered (JKI)")->RangeMultiplier(2)->Range(64, 2048);
BENCHMARK(BM_MultiplicationReorderedKIJ)->Name("Reordered (KIJ)")->RangeMultiplier(2)->Range(64, 2048);
BENCHMARK(BM_MultiplicationReorderedKJI)->Name("Reordered (KJI)")->RangeMultiplier(2)->Range(64, 2048);

//BENCHMARK_MAIN();
