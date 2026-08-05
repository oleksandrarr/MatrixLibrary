#!/bin/bash

set -e

if [ $# -eq 0 ]; then
    RUN_BENCHMARKS="reordered blocking"
else
    RUN_BENCHMARKS="$@"
fi
BUILD_BENCHMARKS=$(echo "$RUN_BENCHMARKS" | tr ' ' ';')

rm -rf build
rm -rf results

mkdir -p build
mkdir -p results/csv
mkdir -p results/plots

echo "====== Configuring.. ======."

cmake -S . -B build \
    -DCMAKE_BUILD_TYPE=Release \
    -DBUILD_BENCHMARKS=ON \
    -DBENCHMARKS_TO_BUILD="$BUILD_BENCHMARKS" \
    -DVECTORIZATION_REPORT=ON

echo "====== Building... ======"

cmake --build build -j

echo "====== Running tests... ======"

./build/tests

echo "====== Running benchmarks... ======"

for BENCH in $RUN_BENCHMARKS
do
  echo "Running $BENCH"
  ./build/${BENCH}_benchmark \
    --benchmark_out=results/csv/${BENCH}_benchmark.csv \
    --benchmark_out_format=csv \
    --benchmark_counters_tabular=true
done

echo "====== Generating plots... ======"
for BENCH in $RUN_BENCHMARKS
do
	python3 benchmarks/plots/plots.py $BENCH
	python3 benchmarks/plots/plotSpeedup.py $BENCH
	python3 benchmarks/plots/summary.py $BENCH
done

echo "====== Done! ======"
