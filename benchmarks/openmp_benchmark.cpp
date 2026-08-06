#include <benchmark/benchmark.h>
#include <MatrixLibrary.h>
#include "utils/benchmark_utils.h"

static void BM_Multiplication(benchmark::State& state){
  MultiplicationTemplate(state, [](const Matrix& A, const Matrix& B){
    return A*B;
  });
}
static void BM_MultiplicationIKJOmpParallelFor(benchmark::State& state){
  MultiplicationTemplate(state, [](const Matrix& A, const Matrix& B){
    return A.multiplyReorderedIKJOmpParallelFor(B);
  });
}
static void BM_MultiplicationIKJOmpParallelForCollapse2(benchmark::State& state){
  MultiplicationTemplate(state, [](const Matrix& A, const Matrix& B){
    return A.multiplyReorderedIKJOmpParallelForCollapse2(B);
  });
}
static void BM_MultiplicationIKJOmpParallelForStatic(benchmark::State& state){
  MultiplicationTemplate(state, [](const Matrix& A, const Matrix& B){
    return A.multiplyReorderedIKJOmpParallelForStatic(B);
  });
}
int main(int argc, char** argv)
{
    std::cout << "=============================================================\n";
    std::cout << "    Matrix Multiplication Benchmark\n";
    std::cout << "=============================================================\n\n";

    benchmark::Initialize(&argc, argv);

    benchmark::RunSpecifiedBenchmarks();

    std::cout << "\nBenchmark finished.\n";
}

BENCHMARK(BM_Multiplication)->Name("Naive (IJK)")->RangeMultiplier(2)->Range(64, 2048);
BENCHMARK(BM_MultiplicationIKJOmpParallelFor)->Name("IKJ ParallelFor")->RangeMultiplier(2)->Range(64, 2048);
BENCHMARK(BM_MultiplicationIKJOmpParallelForCollapse2)->Name("IKJ ParallelFor Collapse2")->RangeMultiplier(2)->Range(64, 2048);
BENCHMARK(BM_MultiplicationIKJOmpParallelForStatic)->Name("IKJ ParallelFor Static")->RangeMultiplier(2)->Range(64, 2048);
