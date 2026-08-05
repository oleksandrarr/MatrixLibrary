#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
#include <vector>

class Matrix{
private:
  size_t rows = 0;
  size_t cols = 0;
  std::vector<double> data = {0};

public:
  Matrix(size_t r, size_t c, std::vector<double> d); //initialize with vector d
  Matrix(size_t r, size_t c); //initialize with 0
  Matrix(size_t r, size_t c, size_t min, size_t max); //initialize with random values between min and max

  Matrix operator+(const Matrix& other) const;
  Matrix operator-(const Matrix& other) const;
  Matrix operator*(const Matrix& other) const;
  Matrix operator*(double k) const;
  friend Matrix operator*(const double k, const Matrix& m);
  double& operator()(size_t i, size_t j);
  const double& operator()(size_t, size_t) const;
  double& at(size_t i, size_t j);
  bool operator==(const Matrix& other) const;
  Matrix transpose() const;
  static Matrix identity(size_t n);
  friend std::ostream& operator<<(std::ostream& os, const Matrix& m);
  
  size_t getRows() const;
  size_t getCols() const;
  
  Matrix multiplyReorderedIKJ(const Matrix& other) const;
  Matrix multiplyReorderedJIK(const Matrix& other) const;
  Matrix multiplyReorderedJKI(const Matrix& other) const;
  Matrix multiplyReorderedKIJ(const Matrix& other) const;
  Matrix multiplyReorderedKJI(const Matrix& other) const;
  
  Matrix naiveBlocking(const Matrix& other) const;
  Matrix reorderedIKJBlocking(const Matrix& other) const;
};
  
#endif
