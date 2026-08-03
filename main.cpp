#include <iostream>
#include "MatrixLibrary.h"
using namespace std;

int main(){
  Matrix A(3,3,{2,0,1,3,0,0,5,1,1});
  Matrix B(3,3,{1,0,1,1,2,1,1,1,0});

  cout<<"A:\n"<<A;
  cout<<"\n"<<"B:\n"<<B;
  cout<<"\n";

  Matrix C = A+B;
  cout<<"main.cpp: A + B:\n"<<C;
  Matrix D = A-B;
  cout<<"main.cpp: A - B:\n"<<D;
  Matrix E = A*B;
  cout<<"main.cpp: A * B:\n"<<E;
  Matrix F = A*2;
  cout<<"main.cpp: A * 2:\n"<<F;
  Matrix BB = 2*B;
  cout<<"main.cpp: 2*B\n"<<BB;
  
  cout<<"main.cpp: A == B?\n";
  if(A==B) cout<<"True\n";
  else cout<<"False\n";

  cout<<"main.cpp: Accesing element i=1, j=2 of A\n";
  cout<<A(1,2)<<"\n";
  cout<<"main.cpp: Accesing element i=2, j=2 of A\n";
  cout<<A.at(2,2)<<"\n";

  cout<<"main.cpp: Transpose A\n";
  Matrix G = A.transpose();  
  cout<<G;

  cout<<"main.cpp: Identity matrix of B\n"<<Matrix::identity(3);
  
  cout<<"Size of A: ("<<A.getRows()<<", "<<A.getCols()<<")\n";
  return 0;
}
