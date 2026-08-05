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

### The Size Selection

The CPU used for testing has 32 KiB L1 Data Cache and the element type used is double (8 bytes). The tile size was selected considering the amount of data that can fit in the cache.

The rigorous study of tile size: ...

### Blocking Results

Blocking was applied to the naive implementation and the reordered IKJ implementation. 

<img width="1000" height="600" alt="gflopsReordered" src="https://github.com/user-attachments/assets/036e00f8-cbfb-441d-8913-46358053f78d" />

The blocked naive implementation provides a small improvement compared with the original version. However, performance still decreases significantly for larger matrices.

This happens because the original loop ordering prevents efficient vectorization and generates less predictable memory accesses. 

For small matrices, the simple IKJ implementation can outperform the blocked version because blocking introduces additional loop overhead. However, for larger matrices, the blocked IKJ implementation becomes faster because it reduces cache misses and improves data reuse.

<img width="1000" height="600" alt="blockingSpeedup" src="https://github.com/user-attachments/assets/c89f0fe5-6a0c-4608-8a6b-d86e21c68831" />

The speedup results confirm that blocking becomes increasingly beneficial as the matrix size grows. For small matrices, the overhead of managing tiles can outweight the benefit. For larger matrices, blocking provides better scalability because it avoids frequent cache evictions and keeps active data closer to the CPU.

