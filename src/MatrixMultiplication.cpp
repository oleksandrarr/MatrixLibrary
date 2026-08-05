#include <stdexcept> //throws exception
#include <bits/stdc++.h>
#include <MatrixLibrary.h>
using namespace std;

inline void Matrix::checkDimensions(const Matrix& other) const{
  if(cols != other.rows) throw invalid_argument("The number of columns in the first matrix must equal the number of rows in the second matrix.");
}

Matrix Matrix::multiplyReorderedIKJ(const Matrix& other) const {
    checkDimensions(other);
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
    checkDimensions(other);
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
    checkDimensions(other);
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
    checkDimensions(other);
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
    checkDimensions(other);
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
    checkDimensions(other);
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
    checkDimensions(other);
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
