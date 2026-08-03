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
        for(int k=0; k<rows; k++){
          res[i*cols+j] += data[i*cols+k] * other.data[k*cols+j];
        }
      }
    }
    return Matrix(rows, cols, res);
  }

  Matrix operator*(const double k) const{
    vector<double> res(rows*cols);
    for(int i=0; i<rows*cols; i++){
      res[i] = data[i]*k;
    }
    return Matrix(rows, cols, res); 
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

  return 0;
}

//multiplicación por escalar
//comparar matrices ==
//acceo a elementos por at(i,j) o operator()(i,j)
//transpose()
//matrix identidad
//consultar dimensiones


//hacer documento matrix.h y main.cpp y CMakeLists.txt
