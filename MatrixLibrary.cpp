#include <iostream>
#include <vector>
#include <stdexcept> //throws exception
using namespace std;

class Matrix{
private:
  int rows = 0;
  int cols = 0;
  vector<double> data = {0};

public:
  Matrix(int r, int c, vector<double> d) : rows(r), cols(c), data(move(d)) {} //to prevent copies
  
  Matrix(int r, int c) : rows(r), cols(c), data(r*c, 0.0) {} //initialize to 0

  Matrix operator+(const Matrix& other) const{
     if(rows!=other.rows || cols != other.cols) throw invalid_argument("The matrices must be of the same size.");

     vector<double> res(rows*cols);
     for(int i=0; i<rows*cols; i++){
       res[i]=data[i]+other.data[i];
     }
     return Matrix(rows, cols, res);
  }

  Matrix operator-(const Matrix& other) const{
     if(rows!=other.rows || cols != other.cols) throw invalid_argument("The matrices must be of the same size.");

     vector<double> res(rows*cols);
     for(int i=0; i<rows*cols; i++){
       res[i]=data[i]-other.data[i];
     }
     return Matrix(rows, cols, res);
  }

  Matrix operator*(const Matrix& other) const {
    if(cols != other.rows) throw invalid_argument("The number of columns in the first matrix must equal the number of rows in the second matrix.");
    vector<double> res(rows*other.cols);

    for(int i=0; i<rows; i++){
      for(int j=0; j<other.cols; j++){
        for(int k=0; k<cols; k++){
          res[i*other.cols+j] += data[i*cols+k] * other.data[k*other.cols+j];
        }
      }
    }
    return Matrix(rows, cols, res);
  }

  Matrix operator*(const double k) const {
    vector<double> res(rows*cols);
    for(int i=0; i<rows*cols; i++){
      res[i] = data[i]*k;
    }
    return Matrix(rows, cols, res);
  }
  
  friend Matrix operator*(const double k, const Matrix& m) {
    return m*k;
  }

  double& operator()(int i, int j) {
    return data[i*cols+j];
  }

  double& at(int i, int j){ //STL like implementation
    if(i<0 || i>=rows || j<0 || j>=cols) throw out_of_range("Index out of range.");
    return data[i*cols+j];
  }

  bool operator==(const Matrix& other) const{
    if(rows != other.rows || cols != other.cols)
      return false;

    for(int i=0; i<rows; i++){
      for(int j=0; j<cols; j++){
        if(data[i*cols+j] != other.data[i*cols+j])
          return false;
      }
    }
    return true;
  }
  
  Matrix transpose() const {
    vector<double> res(rows* cols);
    for(int i=0; i<rows; i++){
      for(int j=0; j<cols; j++){
        res[j*rows+i] = data[i*cols+j];
      }
    }
    return Matrix(cols, rows, res);
  }

  Matrix identity(int n) const {
    vector<double> res(n*n);
    for(int i=0; i<n; i++){
      for(int j=0; j<n; j++){
        if(i==j) res[i*n+j] = 1;
        else res[i*n+j] = 0;
      }
    }
    return Matrix(n, n, res);
  }

  friend ostream& operator<<(ostream& os, const Matrix& m) {
    for(int i=0; i<m.rows; i++){
      for(int j=0; j<m.cols; j++){
        os<<m.data[i*m.cols+j]<<" ";
      }
      os<<"\n";
    }
    return os;
  }
  
  void print() const{
    for(int i=0; i<rows; i++){
      for(int j=0; j<cols; j++){
        cout<<data[i*cols+j]<<" ";
      }
      cout<<"\n";
    }
  }
};



int main(){
  Matrix A(3,3,{2,0,1,3,0,0,5,1,1});
  Matrix B(3,3,{1,0,1,1,2,1,1,1,0});
  Matrix C = A+B;
  Matrix D = A-B;

  cout<<"A:\n";
  A.print();
  cout<<"\n"<<"B:\n";
  B.print();
  cout<<"\n";

  cout<<"main.cpp: A + B:\n";
  C.print();

  cout<<"main.cpp: A - B:\n";
  D.print();

  cout<<"main.cpp: A * B:\n";
  Matrix E = A*B;
  E.print();

  cout<<"main.cpp: A * 2:\n";
  Matrix F = A*2;
  F.print();

  cout<<"main.cpp: A == B?\n";
  if(A==B) cout<<"True\n";
  else cout<<"False\n";

  cout<<"main.cpp: Accesing element i=1, j=2 of A\n";
  cout<<A(1,2)<<"\n";
  cout<<"main.cpp: Accesing element i=2, j=2 of A\n";
  cout<<A.at(2,2)<<"\n";

  cout<<"main.cpp: Transpose A\n";
  Matrix G = A.transpose();  
  G.print();

  cout<<"main.cpp: Identity matrix of B\n";
  Matrix H = A.identity(3);
  H.print();
  
  cout<<"main.cpp: cout<<A\n";
  cout<<A;
  
  cout<<"main.cpp: 2*B\n";
  Matrix BB = 2*B;
  cout<<BB;

  return 0;
}

