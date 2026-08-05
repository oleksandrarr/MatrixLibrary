# Matrix Multiplication Performance and Optimization

## 1. Baseline Implementation

Matrix multiplication is one of the most computationally expensive operations in linear algebra libraries.
Using a benchmark to evaluate the naive implementation, we observe that execution time increases significantly as the matrix size grows.

The computational complexity of matrix multiplication is: 

\[ 
O(N^3)
\]

because each output element requires a dot product between one row of the first matrix and one column of the second matrix.

The initial implementation follows the classical `IJK` loop ordering:

```cpp
for (i)
  for (j)
    for (k)
      C[i][j] += A[i][k] * B[k][j];
```
Benchmark results:

| Matrix size | GFLOPS   |
|-------------|----------|
| 64          | 3.28085  |
| 128         | 2.06193  |
| 256         | 0.96215  |
| 512         | 0.26315  |
| 1024        | 0.251644 |
| 2048        | 0.199324 |

As the matrix size increases, performance decreases because the working set becomes larger that the CPU cache hierarchy causing more memory accesses and cache misses.


## 2. Loop Reordering Optimization

The first optimization explored is loop reordering.

In C++ multidimensional arrays are usually stored is row-major order, meaning that elements belonging to the same row are stored contiguously in memory while elements belonging to the same column are separated by the row stride.

Because of this memory layout, accessing elements sequentially improves spatial locality and reduces cache misses.

The original implementation (`IJK`order) accesses matrix `B`by columns which is inefficient in row-major storage.

### Tested Loop Orders

All six possible loop permutations were implemented: `IJK, IKJ, JIK, JKI, KIJ, KJI`. This allows comparing the effect of memory locality independently from the mathematical operation.

The main improvement comes from implementations where the innermost loop accesses contiguous memory: `IKJ` and `KIJ`.

### Results

<img width="1000" height="600" alt="gflops" src="https://github.com/user-attachments/assets/547a68f2-b94a-4db7-a6d4-3b5a1eedd467" />

The results confirm the initial hypothesis. 

The reordered implementations generally achieve better performance than the naive version. Among them, IKJ provides the best results because it combines:
- Sequential access to matrix `B`.
- Good reuse of elements from matrix `A`.
- Better cache utilization.

The speedup graph shows a similar behavior:

<img width="1000" height="600" alt="plotSpeedup" src="https://github.com/user-attachments/assets/8040627a-8536-49a7-929b-5c485b26f767" />

`IKJ` achieves the highest speedup, especially for larger matrices. While some implementations degrades as the matrix size increases, `IKJ` maintains better scalability. 


## 3. Blocking / Tiling Optimization

The next optimization applied is blocking (also called tiling).

The objective of blocking is to improve temporal locality by dividing the matrices into smaller submatrices (tiles). 

Instead of processing the complete matrix the algorithm operated on smaller blocks. This allows frequently reused data to remain in the CPU cache.

The blocked multiplication follows the structure:

```cpp
for(block_i)
  for(block_j)
    for(block_k)
      multiply tiles
```

### Tile Size Selection

The processor used for the experiments has a 32 KiB L1 data cache, and the matrix elements are stored as `double` values (8 bytes each). Since blocking aims to keep the working data set in the cache, the tile size has a significant impact on performance.

Rather than selecting a tile size based solely on the cache capacity, an empirical evaluation was performed. Several tile sizes were benchmarked across different matrix sizes for both the naive and the reordered implementations.

<img width="800" height="500" alt="naiveSizes" src="https://github.com/user-attachments/assets/cb2b760a-ad6c-44a0-be10-425dce5cd9cb" />

<img width="800" height="500" alt="blockingSizes" src="https://github.com/user-attachments/assets/e658881f-b13b-4531-9b75-924f6f4e561f" />

The results show that:

- For the naive implementation, tile sizes of **16** and **32** consistently provide the best performance.
- For the reordered implementation, larger tiles (**64** and **128**) perform better for very small and very large matrices, while **16** and **32** are more efficient for intermediate sizes.

Although no single tile size is optimal for every configuration, **32×32** provides the best overall trade-off. It delivers consistently high performance across all tested matrix sizes while avoiding the performance degradation observed with larger tiles in some cases.

For this reason, a tile size of **32×32** was selected for the remainder of the experiments.

### Blocking Results

Blocking was applied to the naive implementation and the reordered IKJ implementation. 

<img width="1000" height="600" alt="gflopsReordered" src="https://github.com/user-attachments/assets/036e00f8-cbfb-441d-8913-46358053f78d" />

The blocked naive implementation provides a small improvement compared with the original version. However, performance still decreases significantly for larger matrices.

This happens because the original loop ordering prevents efficient vectorization and generates less predictable memory accesses. 

For small matrices, the simple IKJ implementation can outperform the blocked version because blocking introduces additional loop overhead. However, for larger matrices, the blocked IKJ implementation becomes faster because it reduces cache misses and improves data reuse.

<img width="1000" height="600" alt="blockingSpeedup" src="https://github.com/user-attachments/assets/c89f0fe5-6a0c-4608-8a6b-d86e21c68831" />

The speedup results confirm that blocking becomes increasingly beneficial as the matrix size grows. For small matrices, the overhead of managing tiles can outweight the benefit. For larger matrices, blocking provides better scalability because it avoids frequent cache evictions and keeps active data closer to the CPU.

## 4. Vectorization

The next optimization step consists of enabling compiler optimizations that allow automatic SIMD vectorization. The project is compiled using: 
 
```bash 
-o3 -march=native
```
where:
- `-O3` enables aggressive compiler optimizations, including loop transformations and automatic vectorization.
- `-march=native` allows the compiler to generate instructions specifically optimized for the current CPU architecture, enabling the use of available SIMD extensions such as AVX.

The same benchmarks from the previous section are executed again and the obtained results are compared.

<img width="881" height="547" alt="vectorizadoBien" src="https://github.com/user-attachments/assets/abbc7961-7c12-4c02-afc9-f4e92d44ba53" />

The results show that enabling compiler vectorization does not improve all implementions equally. The naive implementation shows almost no improvement compared with the non-vectorized version. This is expected because its main limitation is memory access inefficiency. 

Similarly, naive blocking does not significantly benefit from vectorization. Although blocking improves cache locality, the loop ordering used inside the blocks still limits the compiler's ability to generate efficient vector instructions.

On the other hand, loop reordering and the combination of reordering with blocking show a significant improvements. These implementations expose more regular memory access patterns, allowing the compiler to automatically generate SIMD instructions.

Reordered multiplication still performs better than reordered + blocking for smaller matrix sizes. However, the performance gap is reduced after enabling vectorization and the advantage of blocking becomes more noticeable for larger matrices where cache efficiency becomes increasingly important.

The improvement can also be observed through the normalized speedup: 

<img width="1000" height="600" alt="speedupVectorization" src="https://github.com/user-attachments/assets/d2f4f464-b45c-4be3-8d39-a3441dc48783" />

The reordered implementation reaches a peak performance of almost 36 GFLOPS compared with aproximately 33.5 GFLOPS without compiler vectorization.

The behavior of reordered + blocking follows the expected trend: for small matrices the overhead introduced by blocking can reduce performance, while for larger matrices the improved cache utilization compensates for this overhead and provides better scalability. 

### Vectorization Analysy

To understand these results the vectorization report generated by GCC was analyzed.

The compiler mainly vectorizes the innermost loops of the multiplication implementations. Outer loops usually fail with messages such as:

```bash
missed: not vectorized: multiple nested loops.
```
because only the inerrmost loop is a suitable candidate for SIMD execution.

The implementations successfully vectorized are: IKJ, KIJ and IKJ blocking. The compiler generates AVX vector instructions for these implementations:
```bash
optimized: loop vectorized using 32 byte vectors
```
a 32-byte vector corresponds to AVX instruction processing four `double` values simultaneously. 

The implementations that fail to vectorize are: naive, JIK, JKI, KJI and naive blocking. The main reason is their memory access pattern. Some loop orderings require accessing matrix columns instead of rows that generate non-contiguous memory loads. 

The compiler reports this situation with messages such as:
```bash
not suitable for strided load
no vectype for stmt
```
meaning that it cannot efficiently transform these memory accesses into vector operations.

The behavior of the JKI and KJI implementations is particularly interesting because their loop order increases the amount of column-wise traversal, making the memory access pattern even less favorable. The compiler cannot generate efficient vector loads because the required data elements are too far apart in memory.

