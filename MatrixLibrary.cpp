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
  
  cout<<"main.cpp: A + B:"<<endl;
  C.print();
  
  cout<<"main.cpp: A - B:"<<"\n";
  D.print();
  return 0;
}
