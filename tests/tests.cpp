#include <gtest/gtest.h>
#include "MatrixLibrary.h"

TEST(MatrixTest, Addition){
  Matrix A(3,3,{2,0,1,3,0,0,5,1,1});
  Matrix B(3,3,{1,0,1,1,2,1,1,1,0});
  Matrix C = A+B;
  
  Matrix expected(3,3, {3,0,2,4,2,1,6,2,1});
  EXPECT_TRUE(C==expected);
}

TEST(MatrixTest, InvalidConstructor){
  EXPECT_THROW(Matrix A(2,2,{1,1}), std::invalid_argument);
}

//A(1,2) = 10;
//expect_eq(A(1,2), 10)

//si algo es critico usar ASSERT_THROW que detiene los tests, se suelen usar para constructores
