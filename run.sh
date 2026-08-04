#!/bin/bash

set -e

rm -rf build
rm -rf results

mkdir -p build
mkdir -p results/csv
mkdir -p results/plots

echo "====== Configuring.. ======."

cmake -S . -B build \
    -DCMAKE_BUILD_TYPE=Release \
    -DBUILD_BENCHMARKS=ON

echo "====== Building... ======"

cmake --build build -j

echo "====== Running tests... ======"

./build/tests

echo "====== Running benchmarks... ======"

./build/benchmark_matrix \
    --benchmark_out=results/csv/benchmark.csv \
    --benchmark_out_format=csv \
    --benchmark_counters_tabular=true

echo "====== Generating plots... ======"

python3 benchmarks/plots.py

echo "====== Done! ======"
