# MatrixLibrary
A simple C++ matrix library that implements the most common matrix operations using **operator overloading** and modern C++ features.

The library stores matrix elements in a contiguous `std::vector<double>`, providing efficient memory access and a clean interface for mathematical operations.

---

## Features

### Matrix Addition

Two matrices can be added if they have the same dimensions. Each element is computed independently:

```text
C(i,j) = A(i,j) + B(i,j)
```
---

### Matrix Subtraction

Two matrices can be subtracted if they have the same dimensions.

```text
C(i,j) = A(i,j) - B(i,j)
```

---

### Matrix Multiplication

Two matrices can be multiplied when `A.columns == B.rows`. Each element of the resulting matrix is computed as

```text
C(i,j) = Σ A(i,k) · B(k,j)
```

### Scalar Multiplication

Multiply every element of the matrix by a scalar.

```cpp
Matrix B = A * 2.0;
Matrix C = 2.0 * A;
```

Both left and right scalar multiplication are supported.

---

### Element Access

Access matrix elements using

```cpp
A(i, j)
```

or

```cpp
A.at(i, j)
```

`at()` performs bounds checking and throws `std::out_of_range`, similarly to the STL containers.

---

### Matrix Comparison

Matrices can be compared using

```cpp
A == B
```

Both matrices must have the same dimensions and equal elements.

---

### Transpose

Compute the transpose of a matrix.

```text
A(i,j) → Aᵀ(j,i)
```

---

### Identity Matrix

Generate an identity matrix of size *n × n*.

```cpp
Matrix I = Matrix::identity(4);
```

---

### Output Stream

Matrices can be printed directly.

```cpp
std::cout << A;
```

---

## Storage

The matrix is internally stored as a one-dimensional vector in **row-major order**.

For a matrix

```text
|a b c|
|d e f|
```

the internal representation is

```text
[a b c d e f]
```

An element located at `(i, j)` is stored at

```cpp
data[i * cols + j]
```

---

## Current Functionality

- Matrix addition
- Matrix subtraction
- Matrix multiplication
- Scalar multiplication
- Element access (`operator()` and `at()`)
- Matrix comparison
- Matrix transpose
- Identity matrix generation
- Stream output (`operator<<`)
- Bounds checking with `at()`

---

## Future Improvements

- Iterators
- Determinant
- Matrix inverse
- LU decomposition
- Optimized matrix multiplication (cache blocking / SIMD)
- Benchmark suite for performance comparison
