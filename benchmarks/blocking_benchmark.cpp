#include <benchmark/benchmark.h>
#include <MatrixLibrary.h>
#include "utils/benchmark_utils.h"

static void BM_Multiplication(benchmark::State& state){
  MultiplicationTemplate(state, [](const Matrix& A, const Matrix& B){
    return A*B;
  });
}

static void BM_MultiplicationNaiveBlocking(benchmark::State& state){ 
  MultiplicationTemplate(state, [](const Matrix& A, const Matrix& B){
    return A.naiveBlocking(B);
  });
}
static void BM_MultiplicationReorderedIKJ(benchmark::State& state){ 
  MultiplicationTemplate(state, [](const Matrix& A, const Matrix& B){
    return A.multiplyReorderedIKJ(B);
  });
}
static void BM_MultiplicationReorderedIKJBlocking(benchmark::State& state){ 
  MultiplicationTemplate(state, [](const Matrix& A, const Matrix& B){
    return A.reorderedIKJBlocking(B);
  });
}

int main(int argc, char** argv)
{
    std::cout << "=============================================================\n";
    std::cout << "             Blocking Matrix Multiplication Benchmark\n";
    std::cout << "=============================================================\n\n";

    benchmark::Initialize(&argc, argv);

    benchmark::RunSpecifiedBenchmarks();

    std::cout << "\nBenchmark finished.\n";
}

BENCHMARK(BM_Multiplication)->Name("Naive (IJK)")->RangeMultiplier(2)->Range(64, 4096);
BENCHMARK(BM_MultiplicationNaiveBlocking)->Name("Naive Blocking")->RangeMultiplier(2)->Range(64, 4096);
BENCHMARK(BM_MultiplicationReorderedIKJ)->Name("Reordered (IKJ)")->RangeMultiplier(2)->Range(64, 4096);
BENCHMARK(BM_MultiplicationReorderedIKJBlocking)->Name("Reordered Blocking (IKJ)")->RangeMultiplier(2)->Range(64, 4096);
