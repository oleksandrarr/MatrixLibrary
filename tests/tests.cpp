#include <gtest/gtest.h>
#include "MatrixLibrary.h"

//constructors
TEST(MatrixTest, InvalidConstructor){
  EXPECT_THROW(Matrix A(2,2,{1,1}), std::invalid_argument);
}

TEST(MatrixTest, ConstructorCustom){
  Matrix A(3,3,{2,0,1,3,0,0,5,1,1});
  Matrix expected(3,3,{2,0,1,3,0,0,5,1,1});
  EXPECT_TRUE(A==expected);
}

TEST(MatrixTest, ConstructorZeros){
  Matrix C(2,2);
  Matrix expected(2,2,{0,0,0,0});
  EXPECT_TRUE(C==expected);
}

TEST(MatrixTest, Addition){
  Matrix A(3,3,{2,0,1,3,0,0,5,1,1});
  Matrix B(3,3,{1,0,1,1,2,1,1,1,0});
  Matrix C = A+B;
  
  Matrix expected(3,3, {3,0,2,4,2,1,6,2,1});
  EXPECT_TRUE(C==expected);
}

TEST(MatrixTest, AdditionError){
  Matrix A(2,2);
  Matrix B(3,2);
  EXPECT_THROW(A+B, std::invalid_argument);
}

TEST(MatrixTest, Substraction){
  Matrix A(3,3,{2,0,1,3,0,0,5,1,1});
  Matrix B(3,3,{1,0,1,1,2,1,1,1,0});
  Matrix C = A-B;
  
  Matrix expected(3,3, {1,0,0,2,-2,-1,4,0,1});
  EXPECT_TRUE(C==expected);
}

TEST(MatrixTest, SubstractionError){
  Matrix A(2,2);
  Matrix B(3,2);
  EXPECT_THROW(A-B, std::invalid_argument);
}

TEST(MatrixTest, Product){
  Matrix A(3,3,{2,0,1,3,0,0,5,1,1});
  Matrix B(3,3,{1,0,1,1,2,1,1,1,0});
  Matrix C = A*B;
  
  Matrix expected(3,3, {3,1,2,3,0,3,7,3,6});
  EXPECT_TRUE(C==expected);
}

TEST(MatrixTest, ProductReorderedIKJ){
  Matrix A(3,3,{2,0,1,3,0,0,5,1,1});
  Matrix B(3,3,{1,0,1,1,2,1,1,1,0});
  Matrix C = A.multiplyReorderedIKJ(B);
  
  Matrix expected(3,3, {3,1,2,3,0,3,7,3,6});
  EXPECT_TRUE(C==expected);
}
TEST(MatrixTest, ProductReorderedJIK){
  Matrix A(3,3,{2,0,1,3,0,0,5,1,1});
  Matrix B(3,3,{1,0,1,1,2,1,1,1,0});
  Matrix C = A.multiplyReorderedJIK(B);
  
  Matrix expected(3,3, {3,1,2,3,0,3,7,3,6});
  EXPECT_TRUE(C==expected);
}
TEST(MatrixTest, ProductReorderedJKI){
  Matrix A(3,3,{2,0,1,3,0,0,5,1,1});
  Matrix B(3,3,{1,0,1,1,2,1,1,1,0});
  Matrix C = A.multiplyReorderedJKI(B);
  
  Matrix expected(3,3, {3,1,2,3,0,3,7,3,6});
  EXPECT_TRUE(C==expected);
}
TEST(MatrixTest, ProductReorderedKIJ){
  Matrix A(3,3,{2,0,1,3,0,0,5,1,1});
  Matrix B(3,3,{1,0,1,1,2,1,1,1,0});
  Matrix C = A.multiplyReorderedKIJ(B);
  
  Matrix expected(3,3, {3,1,2,3,0,3,7,3,6});
  EXPECT_TRUE(C==expected);
}
TEST(MatrixTest, ProductReorderedKJI){
  Matrix A(3,3,{2,0,1,3,0,0,5,1,1});
  Matrix B(3,3,{1,0,1,1,2,1,1,1,0});
  Matrix C = A.multiplyReorderedKJI(B);
  
  Matrix expected(3,3, {3,1,2,3,0,3,7,3,6});
  EXPECT_TRUE(C==expected);
}
TEST(MatrixTest, ProductNaiveBlocking){
  Matrix A(3,3,{2,0,1,3,0,0,5,1,1});
  Matrix B(3,3,{1,0,1,1,2,1,1,1,0});
  Matrix C = A.naiveBlocking(B);
  
  Matrix expected(3,3, {3,1,2,3,0,3,7,3,6});
  EXPECT_TRUE(C==expected);
}
TEST(MatrixTest, ProductReorderedIKJBlocking){
  Matrix A(3,3,{2,0,1,3,0,0,5,1,1});
  Matrix B(3,3,{1,0,1,1,2,1,1,1,0});
  Matrix C = A.reorderedIKJBlocking(B);
  
  Matrix expected(3,3, {3,1,2,3,0,3,7,3,6});
  EXPECT_TRUE(C==expected);
}

TEST(MatrixTest, ProductError){
  Matrix A(2,2);
  Matrix B(3,2);
  EXPECT_THROW(A*B, std::invalid_argument);
}

TEST(MatrixTest, ScalarProduct){
  Matrix A(3,3,{2,0,1,3,0,0,5,1,1});
  Matrix C = A*2;
  
  Matrix expected(3,3, {4,0,2,6,0,0,10,2,2});
  EXPECT_TRUE(C==expected);
}

TEST(MatrixTest, ScalarProductLeft){
  Matrix A(3,3,{2,0,1,3,0,0,5,1,1});
  Matrix C = 2*A;
  
  Matrix expected(3,3, {4,0,2,6,0,0,10,2,2});
  EXPECT_TRUE(C==expected);
}

TEST(MatrixTest, Access){
  Matrix A(3,3,{2,0,1,3,0,0,5,1,1});
  EXPECT_EQ(A(1,1),0);
}

TEST(MatrixTest, AccessConstMatrix){
  const Matrix A(3,3,{2,0,1,3,0,0,5,1,1});
  EXPECT_EQ(A(1,1),0);
}

TEST(MatrixTest, AccessAt){
  Matrix A(3,3,{2,0,1,3,0,0,5,1,1});
  EXPECT_EQ(A.at(1,1),0);
}

TEST(MatrixTest, AccessAtInvalid){
  Matrix A(3,3,{2,0,1,3,0,0,5,1,1});
  EXPECT_THROW(A.at(4,1), std::out_of_range);
}

TEST(MatrixTest, Equal){
  Matrix A(3,3,{2,0,1,3,0,0,5,1,1});
  Matrix B(3,3,{2,0,1,3,0,0,5,1,1});
  EXPECT_TRUE(A==B);
}

TEST(MatrixTest, EqualFalse){
  Matrix A(3,3,{2,0,1,3,0,0,5,1,1});
  Matrix B(3,3,{1,0,1,1,2,1,1,1,0});
  EXPECT_FALSE(A==B);
}

TEST(MatrixTest, Transpose){
  Matrix A(2,3,{1,2,3,4,5,6});
  Matrix T = A.transpose();
  Matrix expected(3,2,{1,4,2,5,3,6});
  EXPECT_TRUE(T==expected);
}

TEST(MatrixTest, Identity){
  Matrix I = Matrix::identity(2);
  Matrix expected(2,2,{1,0,0,1});
  EXPECT_TRUE(I==expected);
}

int main(int argc, char **argv){
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
