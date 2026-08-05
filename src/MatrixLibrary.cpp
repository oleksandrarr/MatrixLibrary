#include <stdexcept> //throws exception
#include <bits/stdc++.h>
#include <MatrixLibrary.h>
using namespace std;

  Matrix::Matrix(size_t r, size_t c, vector<double> d) : rows(r), cols(c) 
  {
    if(d.size() != r*c) throw invalid_argument("Data size must be of the size rows*columns");
    data = move(d); //to prevent copies
  }
  
  Matrix::Matrix(size_t r, size_t c) : rows(r), cols(c), data(r*c, 0.0) {} 
  
  Matrix::Matrix(size_t r, size_t c, size_t min, size_t max) : rows(r), cols(c)
  {
    vector<double> d(r*c);
    for(size_t i=0; i<r*c; i++) d[i] = min + rand()%max;
    
    data = move(d);
  }

  Matrix Matrix::operator+(const Matrix& other) const{
     if(rows!=other.rows || cols != other.cols) throw invalid_argument("The matrices must be of the same size.");

     vector<double> res(rows*cols);
     for(size_t i=0; i<rows*cols; i++){
       res[i]=data[i]+other.data[i];
     }
     return Matrix(rows, cols, res);
  }

  Matrix Matrix::operator-(const Matrix& other) const{
     if(rows!=other.rows || cols != other.cols) throw invalid_argument("The matrices must be of the same size.");

     vector<double> res(rows*cols);
     for(size_t i=0; i<rows*cols; i++){
       res[i]=data[i]-other.data[i];
     }
     return Matrix(rows, cols, res);
  }

  Matrix Matrix::operator*(const Matrix& other) const {
    if(cols != other.rows) throw invalid_argument("The number of columns in the first matrix must equal the number of rows in the second matrix.");
    vector<double> res(rows*other.cols);

    for(size_t i=0; i<rows; i++){
      for(size_t j=0; j<other.cols; j++){
        for(size_t k=0; k<cols; k++){
          res[i*other.cols+j] += data[i*cols+k] * other.data[k*other.cols+j];
        }
      }
    }
    return Matrix(rows, other.cols, res);
  }

  Matrix Matrix::operator*(const double k) const {
    vector<double> res(rows*cols);
    for(size_t i=0; i<rows*cols; i++){
      res[i] = data[i]*k;
    }
    return Matrix(rows, cols, res);
  }
  
  Matrix operator*(const double k, const Matrix& m) {
    return m*k;
  }

  double& Matrix::operator()(size_t i, size_t j) {
    return data[i*cols+j];
  }
  
  const double& Matrix::operator()(size_t i, size_t j) const { //to access const matrices
    return data[i*cols+j];
  }

  double& Matrix::at(size_t i, size_t j){ //STL like implementation
    if(i<0 || i>=rows || j<0 || j>=cols) throw out_of_range("Index out of range.");
    return data[i*cols+j];
  }

  bool Matrix::operator==(const Matrix& other) const{
    if(rows != other.rows || cols != other.cols)
      return false;

    for(size_t i=0; i<rows; i++){
      for(size_t j=0; j<cols; j++){
        if(data[i*cols+j] != other.data[i*cols+j])
          return false;
      }
    }
    return true;
  }
  
  Matrix Matrix::transpose() const {
    vector<double> res(rows* cols);
    for(size_t i=0; i<rows; i++){
      for(size_t j=0; j<cols; j++){
        res[j*rows+i] = data[i*cols+j];
      }
    }
    return Matrix(cols, rows, res);
  }

  Matrix Matrix::identity(size_t n)  {
    vector<double> res(n*n);
    for(size_t i=0; i<n; i++){
      for(size_t j=0; j<n; j++){
        if(i==j) res[i*n+j] = 1;
        else res[i*n+j] = 0;
      }
    }
    return Matrix(n, n, res);
  }

  ostream& operator<<(ostream& os, const Matrix& m) {
    for(size_t i=0; i<m.rows; i++){
      for(size_t j=0; j<m.cols; j++){
        os<<m.data[i*m.cols+j]<<" ";
      }
      os<<"\n";
    }
    return os;
  }
  
  size_t Matrix::getRows() const{
    return rows;
  }
  
  size_t Matrix::getCols() const{
    return cols;
  }

Matrix Matrix::multiplyReorderedIKJ(const Matrix& other) const {
    if(cols != other.rows) throw invalid_argument("The number of columns in the first matrix must equal the number of rows in the second matrix.");
    vector<double> res(rows*other.cols);

    for(size_t i=0; i<rows; i++){
      for(size_t k=0; k<cols; k++){
        for(size_t j=0; j<other.cols; j++){
        
          res[i*other.cols+j] += data[i*cols+k] * other.data[k*other.cols+j];
        }
      }
    }
    return Matrix(rows, other.cols, res);
  }
  
  Matrix Matrix::multiplyReorderedJIK(const Matrix& other) const {
    if(cols != other.rows) throw invalid_argument("The number of columns in the first matrix must equal the number of rows in the second matrix.");
    vector<double> res(rows*other.cols);

    for(size_t j=0; j<other.cols; j++){
      for(size_t i=0; i<rows; i++){
        for(size_t k=0; k<cols; k++){
          res[i*other.cols+j] += data[i*cols+k] * other.data[k*other.cols+j];
        }
      }
    }
    return Matrix(rows, other.cols, res);
  }
  
  Matrix Matrix::multiplyReorderedJKI(const Matrix& other) const {
    if(cols != other.rows) throw invalid_argument("The number of columns in the first matrix must equal the number of rows in the second matrix.");
    vector<double> res(rows*other.cols);

    for(size_t j=0; j<other.cols; j++){
      for(size_t k=0; k<cols; k++){
        for(size_t i=0; i<rows; i++){  
          res[i*other.cols+j] += data[i*cols+k] * other.data[k*other.cols+j];
        }
      }
    }
    return Matrix(rows, other.cols, res);
  }
  Matrix Matrix::multiplyReorderedKIJ(const Matrix& other) const {
    if(cols != other.rows) throw invalid_argument("The number of columns in the first matrix must equal the number of rows in the second matrix.");
    vector<double> res(rows*other.cols);
      
    for(size_t k=0; k<cols; k++){
     for(size_t i=0; i<rows; i++){
       for(size_t j=0; j<other.cols; j++){
          res[i*other.cols+j] += data[i*cols+k] * other.data[k*other.cols+j];
        }
      }
    }
    return Matrix(rows, other.cols, res);
  }
   Matrix Matrix::multiplyReorderedKJI(const Matrix& other) const {
    if(cols != other.rows) throw invalid_argument("The number of columns in the first matrix must equal the number of rows in the second matrix.");
    vector<double> res(rows*other.cols);
      
    for(size_t k=0; k<cols; k++){
     for(size_t j=0; j<other.cols; j++){
       for(size_t i=0; i<rows; i++){
          res[i*other.cols+j] += data[i*cols+k] * other.data[k*other.cols+j];
        }
      }
    }
    return Matrix(rows, other.cols, res);
  }
  
  
  
    Matrix Matrix::naiveBlocking(const Matrix& other) const {
    if(cols != other.rows) throw invalid_argument("The number of columns in the first matrix must equal the number of rows in the second matrix.");
    vector<double> res(rows*other.cols);

    const int BS = 32;
    
    for(size_t ii=0; ii<rows; ii+=BS){
      for(size_t jj=0; jj<other.cols; jj+=BS){
        for(size_t kk=0; kk<cols; kk+=BS){
          //block
          for(size_t i=ii; i<min(ii+BS, rows); i++)
            for(size_t j=jj; j<min(jj+BS, other.cols); j++)
              for(size_t k=kk; k<min(kk+BS, cols); k++)
                res[i*other.cols+j] += data[i*cols+k] * other.data[k*other.cols+j];
        }
      }
    }
    return Matrix(rows, other.cols, res);
  }

Matrix Matrix::reorderedIKJBlocking(const Matrix& other) const {
    if(cols != other.rows) throw invalid_argument("The number of columns in the first matrix must equal the number of rows in the second matrix.");
    vector<double> res(rows*other.cols);

    const int BS = 32;
    
    for(size_t ii=0; ii<rows; ii+=BS){
      for(size_t jj=0; jj<other.cols; jj+=BS){
        for(size_t kk=0; kk<cols; kk+=BS){
          //block
          for(size_t i=ii; i<min(ii+BS, rows); i++)
            for(size_t k=kk; k<min(kk+BS, cols); k++)
              for(size_t j=jj; j<min(jj+BS, other.cols); j++)
                res[i*other.cols+j] += data[i*cols+k] * other.data[k*other.cols+j];
        }
      }
    }
    return Matrix(rows, other.cols, res);
  }
