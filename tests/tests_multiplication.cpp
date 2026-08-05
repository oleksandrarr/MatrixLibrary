#include <gtest/gtest.h>
#include "MatrixLibrary.h"

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
