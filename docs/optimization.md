# Matrix Multiplication Performance and Optimization

## 1. Baseline Implementation

Matrix multiplication is one of the most computationally expensive operations in linear algebra libraries.
Using a benchmark to evaluate the naive implementation, we observe that execution time increases significantly as the matrix size grows.

The computational complexity of matrix multiplication is: 

```
O(N^3)
```

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

### Vectorization Analysys

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

## 5. OpenMP

The last optimization explored is done with OpenMP to explot thread-level parallelism. Among the different loop orderings analyzed previously the `IKJ` implementation is the best candidate for parallelization. Parallelizing the outer `i` loop allows each thread to process and independent row of the output matrix, avoiding data races while preserving good cache behavior. 

### Study of OpenMP pragmas

To evaluate the impact of the different OpenMP directives independently from the compiler optimizations, automatic vectorization was disabled during these experiments. The following OpenMP pragmas were evaluated:

- `#pragma omp parallel`
- `#pragma omp parallel for`
- `#pragma omp parallel for collapse(2)`
- `#pragma omp parallel for collapse(2)` applied to nested loops
- `#pragma omp parallel for collapse(3)`
- `#pragma omp parallel for schedule(static)`
- `#pragma omp parallel for schedule(dynamic)`
- `#pragma omp parallel for schedule(guided)`

The performance of the different OpenMP scheduling strategies strongly depends on the matrix size, as the amount of available parallel work change significantly.

For 64x64 matrices the best performing strategy is `collapse(2)` reaching 18.8 GFLOPS. This is approximately 2.7x faster than `schedule(static)` (7.0 GFLOPS). At this size, the workload is relatively small so parallelizing only the outer loop provides just 64 iterations to distribute among the threads. By collapsing two loops, the iteration space increases to 4096 iterations leading to much better load balancing and higher thread utilization.

For medium sized matrices, `schedule(static)` consistently provide the highest performance. It outperform `collapse(2)` by around 27% at 128 matrices and 15% at 512 matrices. At these sizes the outer loop already exposes enough parallelism making loop collapsing unnecessary.

For large matrices, `schedule(guided)` and `schedule(dynamic)` become competitive and eventually outperform `schedule(static)`. At 2048 matrix it improves by almost 33%. As the problem size increases, execution becomes more sensitive to memory latency, cache misses and small workload imbalances. Dynamic scheduling policies redistribute work during execution, preventing some threads from becoming idle while others are still processing large chunks of data.

We also evaluated `collapse(3)` that performs better than the implementation that parallelizes only the `parallel` region but it remains slower than all strategies based on `parallel for`. The reason is that collapsing the third loop (`k`) causes multiple threads to contribute to the same output elements of the result matrix, increasing synchronization costs and reducing memory locality.

### Thread Number

To evaluate the scalability of the OpenMP implementations we studied the influence of the number of threads on the performance of three different strategies: `parallel for`, `parallel for schedule(static)` and `parallel for collapse(2)`. The experiments were carried out using 1, 2, 4, 8 and 16 threads. 

For small matrices performance increases as the number of threads grows, reaching its maximum with 8 threads. Beyond this point, adding more threads introduces overhead of thread creation, scheduling and synchronization. For example, for `parallel for` using 16 threads decrease the performance of approximately 20%. 

For bigger sized matrices the computational workload is large enough to amortize the parallelization overhead and the best results are obtained using 16 threads. 

Overall, these results confirm the theoretical expectations of parallel computing. When the workload is small the cost of managing threads dominated execution time, limiting scalability. Consequently, 8 threads are sufficient for small matrices while 16 threads provide the best performance for medium and large matrices.

Below, we present the graph of GFLOPS and speedup of the best configurations found, all using 16 threads.

<img width="1000" height="600" alt="openmpMem" src="https://github.com/user-attachments/assets/d73c63c5-86ea-43f7-912c-2303128e00ac" />

<img width="1000" height="600" alt="speedupOpenmpMP" src="https://github.com/user-attachments/assets/360b3dfb-4003-4a4e-bafe-72d58ee9bfa3" />

### Automatic Vectorization

Now we study the combination of compiler's vectorization and OpenMP. Overall, automatic vectorization produces a significant performance improvement, particularly for small and medium sized matrices. For the `parallel for` implementation, performance increases almost 50% for 64 matrices, +98% for 256 matrices and +56% for 512 matrices. Similar improvements are observed for `schedule(static)`. 

Interestingly, after enabling vectorization, the difference between the default `parallel for` scheduling and `schedule(static)` becomes almost negligible. The `collapse(2)` implementation also benefits from automatic vectorization. The performance gap between `parallel for` and `collapse(2)` remains almost unchanged.

For the largest matrices the performance gain obtained through vectorization becomes much smaller. In some cases, the vectorized version is even slightly slower than the non-vectorized implementation. This behaviour suggests that the computation is no longer limited by arithmetic throughput but by memory bandwidth and cache accesses.

<img width="1000" height="600" alt="oprnmpVect" src="https://github.com/user-attachments/assets/e0a8b37f-18df-4845-9a9b-50d59c0e1da2" />


<img width="1000" height="600" alt="speedupopenmpVect" src="https://github.com/user-attachments/assets/e2503eac-6daa-4899-bb86-00f0b206a3df" />

### OpenMP SIMD

Finally, the `#pragma omp simd` directive was added on top of automatic vectorization to explicitly instruct the compiler to vectorize the innermost loop. This optimization does not improve the overall performance. The results are very similar to those obtained with automatic vectorization alone and, in several cases, are slightly worse. The only implementation that consistently benefits from the explicit SIMD directive is `collapse(2)`. After adding `omp simd`, its performance becomes almost identical to the other OpenMP variants for all matrix sizes. This indicates that, without the directive, the compiler was more conservative when vectorizing the collapsed loops, whereas the explicit SIMD pragma helps expose additional vectorization opportunities.

<img width="1000" height="600" alt="openmpVectSIMD" src="https://github.com/user-attachments/assets/a3f905ae-6cfe-4665-9b21-654eec73d79f" />

<img width="1000" height="600" alt="speedupOpenmpSIMD" src="https://github.com/user-attachments/assets/710ca28a-9ae6-4746-842b-52b1ddcf0dd0" />


## 6. Key Results

* Loop reordering was one of the most effective optimizations. The `IKJ` loop ordering consistently achieved better performance than the naive implementation by improving spatial locality, cache utilization and data reuse.
* Blocking improves scalability for large matrices. Although tiling introduces some overhead for small inputs, it becomes increasingly beneficial as the matrix size grows because it reduces cache evictions and keeps frequently accessed data closer to the CPU.
* A tile size of 32x32 provided the best overall trade-off across the tested matrix sizes, although the optimal tile size depends on the matrix size and implementation.
* Compiler vectorization has a strong dependency on memory access patterns. The compiler successfully vectorized implementations such as `IKJ` and `KIJ`, generating AVX instructions that process multiple double-precision values simultaneously. In contrast, column-wise and non-contiguous memory accesses limited vectorization opportunities.
* The optimized reordered implementation reached almost 36 GFLOPS, compared with approximately 35.5 GFLOPS without compiler vectorization.
* OpenMP scheduling strategies depend strongly on problem size. `collapse(2)` performed particularly well for small matrices, while `schedule(static)` was generally better for medium sized matrices. For large matrices, `dynamic` and `guided` scheduling became increasingly competitive.
* Thread scalability depends on the workload. Small matrices reached their best performance aroun 8 threads, while larger matrices benefited from up to 16 threads. Adding threads beyond the useful parallelism can introduce scheduling and synchronization overhead.
* Combining OpenMP with compiler vectorization produced significant gains, particularly for small and medium sized matrices, with improvements of almost 50% for 64x64, 98% for 256x256 and 56% for 512x512 matrices in the `parallel for` implementation.
* Explicit `omp simd` did not generally outperform automatic vectorization. This suggests that GCC was already able to exploit most of the available SIMD opportunities, although `omp simd` helped in the `collapse(2)` case.
