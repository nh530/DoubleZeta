#include "typing/Matrix.h"
#include <gtest/gtest.h>
#include <iostream>
#include <stdexcept>
#include <vector>

TEST(MatrixTest, instantiation) {
  Matrix<float> mat1 = Matrix<float>(3, 1, 0);
  EXPECT_EQ(mat1.shape[1], 1);
  EXPECT_EQ(mat1.shape[0], 3);
  Matrix<float> mat2 = Matrix<float>(10, 20);
  EXPECT_EQ(mat2.shape[0], 10);
  EXPECT_EQ(mat2.shape[1], 20);

  Matrix<float> mat3{3, 1, 10.0};
  EXPECT_EQ(mat3.shape[0], 3);
  EXPECT_EQ(mat3.shape[1], 1);
  EXPECT_EQ(mat3[0][0], 10.0);
  EXPECT_EQ(mat3[1][0], 10.0);
  EXPECT_EQ(mat3[2][0], 10.0);

  float *row1 = new float[1]{1.0};
  float *row2 = new float[1]{2.5};
  float *row3 = new float[1]{4.125};
  float *row4 = new float[1]{1.0};
  float **arr = new float *[4];
  arr[0] = row1;
  arr[1] = row2;
  arr[2] = row3;
  arr[3] = row4;
  Matrix<float> mat4{arr, 4, 1};
  EXPECT_EQ(mat4[0][0], 1.0);
  EXPECT_EQ(mat4[1][0], 2.5);
  EXPECT_EQ(mat4[2][0], 4.125);
  EXPECT_EQ(mat4[3][0], 1.0);

  row1 = new float[2]{1.0, 0.0};
  row2 = new float[2]{0.0, 2.5};
  arr = new float *[2];
  arr[0] = row1;
  arr[1] = row2;
  Matrix<float> mat5{arr, 2, 2};
  EXPECT_EQ(mat5[0][0], 1.0);
  EXPECT_EQ(mat5[0][1], 0.0);
  EXPECT_EQ(mat5[1][0], 0.0);
  EXPECT_EQ(mat5[1][1], 2.5);

  delete[] row1;
  delete[] row2;
  delete[] row3;
  delete[] row4;
  delete[] arr;

  Matrix<float> mat6{}; // Testing that this runs. Expected outcome is that it does nothing.
}

TEST(MatrixTest, operator_indexing) {
  Matrix<float> test1 = Matrix<float>(3, 1, 0);
  test1.set_value(0, 0, 10);
  test1.set_value(1, 0, 20);

  EXPECT_EQ(test1[0][0], 10.0f);
  EXPECT_EQ(test1[1][0], 20.0f);
  EXPECT_EQ(test1[2][0], 0.0f);

  // Note that if the first index is within bounds but the second index is out of bounds, then undefined behavior occurs.
  // i.e. test1[2][10] will have undefined behavior. There is no way to force array indexing limit.
  EXPECT_THROW(test1[4], std::invalid_argument);
  EXPECT_THROW(test1[10], std::invalid_argument);
  EXPECT_THROW(test1[-1], std::invalid_argument);
}

TEST(MatrixTest, get_column) {
  Matrix<float> test1 = Matrix<float>(3, 3, 0);
  std::vector<float> col1 = test1.getColumn(0);
  EXPECT_EQ(col1[0], 0);
  EXPECT_EQ(col1.size(), 3);

  float actl = 0.01223423431f;
  test1.set_value(1, 1, actl);
  std::vector<float> col2 = test1.getColumn(1);
  EXPECT_EQ(col2[1], actl);

  test1.set_value(2, 1, 10.0f);
  EXPECT_EQ(test1.getColumn(1)[2], 10.0f);
}

TEST(MatrixTest, operator_add) {
  Matrix<float> left1 = Matrix<float>(4, 4, 0);
  Matrix<float> right1 = Matrix<float>(4, 4, 0);
  EXPECT_EQ((left1 + right1)[0][2], (Matrix<float>(4, 4, 0))[0][2]);
  left1.set_value(0, 0, 10);
  left1.set_value(1, 1, 20);
  left1.set_value(1, 3, 0.123123f);
  Matrix<float> actual = Matrix<float>(4, 4, 0);
  actual.set_value(0, 0, 10);
  actual.set_value(1, 1, 20);
  actual.set_value(1, 3, 0.123123f);
  for (int i = 0; i < actual.shape[0]; i++) {
    for (int j = 0; j < actual.shape[1]; j++) {
      EXPECT_FLOAT_EQ((left1 + right1)[i][j], actual[i][j]);
    }
  }

  right1 += left1;
  for (int i = 0; i < actual.shape[0]; i++) {
    for (int j = 0; j < actual.shape[1]; j++) {
      EXPECT_FLOAT_EQ(right1[i][j], actual[i][j]);
    }
  }
}

TEST(MatrixTest, operator_add_2) {
  Matrix<float> left1 = Matrix<float>(4, 4, 0);
  Matrix<float> right1 = Matrix<float>(4, 4, 0);
  right1.set_value(0, 0, 0.43f);
  right1.set_value(0, 3, 4.91f);
  right1.set_value(1, 3, 2.1f);
  right1.set_value(3, 3, 900.2f);
  left1.set_value(0, 0, 10);
  left1.set_value(1, 1, 20);
  left1.set_value(1, 3, 0.123123f);
  Matrix<float> actual = Matrix<float>(4, 4, 0);
  actual.set_value(0, 0, 10.43f);
  actual.set_value(0, 3, 4.91f);
  actual.set_value(1, 1, 20);
  actual.set_value(1, 3, 2.223123f);
  actual.set_value(3, 3, 900.2f);
  for (int i = 0; i < actual.shape[0]; i++) {
    for (int j = 0; j < actual.shape[1]; j++) {
      EXPECT_FLOAT_EQ((left1 + right1)[i][j], actual[i][j]);
    }
  }
  left1 += right1;
  for (int i = 0; i < actual.shape[0]; i++) {
    for (int j = 0; j < actual.shape[1]; j++) {
      EXPECT_FLOAT_EQ(left1[i][j], actual[i][j]);
    }
  }
}

TEST(MatrixTest, operator_add_3) {
  Matrix<float> left1 = Matrix<float>(4, 4, 0);
  Matrix<float> right1 = Matrix<float>(2, 4, 0);
  EXPECT_THROW(left1 + right1, std::invalid_argument);
  EXPECT_THROW(left1 + Matrix<float>(4, 2, 0), std::invalid_argument);
  EXPECT_THROW(Matrix<float>(2, 2, 0) + Matrix<float>(4, 2, 0), std::invalid_argument);
  EXPECT_THROW(Matrix<float>(0, 0) + Matrix<float>(4, 2, 0),
               std::invalid_argument); // NOTE: Matrix(0, 0) has undefined behavior because array values not defined.
  EXPECT_FLOAT_EQ((Matrix<float>(1, 1) + Matrix<float>(1, 1))[0][0], (Matrix<float>(1, 1))[0][0]);
  EXPECT_THROW(left1 += right1, std::invalid_argument);
}

TEST(MatrixTest, operator_sub) {
  Matrix<float> left1 = Matrix<float>(4, 4, 0);
  Matrix<float> right1 = Matrix<float>(4, 4, 0);
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      EXPECT_EQ((left1 - right1)[i][j], Matrix<float>(4, 4, 0)[i][j]);
    }
  }

  right1.set_value(0, 0, 10.0f);
  right1.set_value(1, 1, 20.0f);
  right1.set_value(1, 3, 0.123123f);
  Matrix<float> actual = Matrix<float>(4, 4, 0);
  actual.set_value(0, 0, -10.0f);
  actual.set_value(1, 1, -20.0f);
  actual.set_value(1, 3, -0.123123f);
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      EXPECT_FLOAT_EQ((left1 - right1)[i][j], actual[i][j]);
    }
  }

  left1 -= right1;
  for (int i = 0; i < actual.shape[0]; i++) {
    for (int j = 0; j < actual.shape[1]; j++) {
      EXPECT_FLOAT_EQ(left1[i][j], actual[i][j]);
    }
  }

  Matrix<float> left2 = Matrix<float>(4, 3, 0);
  Matrix<float> right2 = Matrix<float>(3, 3, 0);
  EXPECT_THROW(left2 - right2, std::invalid_argument);
  EXPECT_THROW(left2 -= right2, std::invalid_argument);
}

TEST(MatrixTest, operator_multi) {
  Matrix<float> left1 = Matrix<float>(4, 4, 0);
  Matrix<float> right1 = Matrix<float>(4, 4, 0);
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      EXPECT_FLOAT_EQ((left1 * right1)[i][j], Matrix<float>(4, 4)[i][j]);
    }
  }

  right1.set_value(0, 0, 0.43f);
  right1.set_value(0, 3, 4.91f);
  right1.set_value(1, 3, 2.1f);
  right1.set_value(3, 3, 900.2f);
  left1.set_value(0, 0, 10);
  left1.set_value(1, 1, 20);
  left1.set_value(1, 3, 0.123123f);
  Matrix<float> actual = Matrix<float>(4, 4, 0);
  actual.set_value(0, 0, 4.3f);
  actual.set_value(0, 3, 49.1f);
  actual.set_value(1, 3, 152.835325f);
  auto res2 = left1 * right1;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      EXPECT_FLOAT_EQ((res2)[i][j], actual[i][j]);
    }
  }
  Matrix<float> left2 = Matrix<float>(1, 3, 2);
  Matrix<float> right2 = Matrix<float>(3, 1, 4.125);
  auto res = left2 * right2;
  EXPECT_EQ(res[0][0], 24.75f);

  Matrix<float> left3 = Matrix<float>(3, 1, 0);
  Matrix<float> right3 = Matrix<float>(3, 1, 0);
  EXPECT_THROW(left3 * right3, std::invalid_argument);
}

TEST(MatrixTest, operator_multi2) {
  Matrix<float> left1 = Matrix<float>(4, 4, 1.0f);
  Matrix<float> right1 = Matrix<float>(4, 4, 1.0f);
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      EXPECT_FLOAT_EQ((left1 * right1)[i][j], Matrix<float>(4, 4, 4.0f)[i][j]);
      EXPECT_FLOAT_EQ((2 * right1)[i][j], Matrix<float>(4, 4, 2.0f)[i][j]);
      EXPECT_FLOAT_EQ((right1 * 2)[i][j], Matrix<float>(4, 4, 2.0f)[i][j]);
      EXPECT_FLOAT_EQ((0.12f * right1)[i][j], Matrix<float>(4, 4, 0.12f)[i][j]);
      EXPECT_FLOAT_EQ((right1 * 0.12f)[i][j], Matrix<float>(4, 4, 0.12f)[i][j]);
    }
  }
}

TEST(MatrixTest, operator_equality) {
  Matrix<float> left1 = Matrix<float>(4, 4, 1.0);
  Matrix<float> right1 = Matrix<float>(4, 4, 1.0);
  EXPECT_TRUE(left1 == right1);
  EXPECT_FALSE(left1 != right1);
  EXPECT_TRUE((2 * left1) != right1);
  EXPECT_FALSE((2 * left1) == right1);
}

TEST(MatrixTest, operator_transpose) {
  Matrix<float> left1 = Matrix<float>(4, 4, 1.0f);
  Matrix<float> actual = Matrix<float>(4, 4, 1.0f);
  left1.set_value(0, 0, 10.0f);
  left1.set_value(0, 3, 2.1f);
  left1.set_value(1, 2, 5.3f);
  actual.set_value(0, 0, 10.0f);
  actual.set_value(3, 0, 2.1f);
  actual.set_value(2, 1, 5.3f);
  Matrix<float> obs = left1.transpose();
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      EXPECT_FLOAT_EQ(obs[i][j], actual[i][j]);
    }
  }

  Matrix<float> left2 = Matrix<float>(3, 1, 2.5f);
  auto res = left2.transpose();
  EXPECT_EQ(res.shape[0], 1);
  EXPECT_EQ(res.shape[1], 3);
  EXPECT_EQ(res[0][0], 2.5f);
  EXPECT_EQ(res[0][1], 2.5f);
  EXPECT_EQ(res[0][2], 2.5f);

  Matrix<float> left3 = Matrix<float>(3, 2, 2.5f);
  Matrix<float> res2 = left3.transpose();
  EXPECT_EQ(res2.shape[0], 2);
  EXPECT_EQ(res2.shape[1], 3);
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 3; j++) {
      EXPECT_EQ(res2[i][j], 2.5f);
    }
  }
  EXPECT_THROW(res2[4], std::invalid_argument);
}

TEST(MatrixTest, move_assignment) {
  Matrix<float> test1 = std::move(Matrix<float>(4, 4, 0));
  EXPECT_EQ(test1.shape[0], 4);
  EXPECT_EQ(test1.shape[1], 4);
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      EXPECT_EQ(test1[i][j], 0);
    }
  }
  float *row1 = new float[1]{1.0};
  float *row2 = new float[1]{2.5};
  float *row3 = new float[1]{4.125};
  float *row4 = new float[1]{1.0};
  float **arr = new float *[4];
  arr[0] = row1;
  arr[1] = row2;
  arr[2] = row3;
  arr[3] = row4;
  Matrix<float> test2 = std::move(Matrix<float>{arr, 4, 1});
  EXPECT_EQ(test2.shape[0], 4);
  EXPECT_EQ(test2.shape[1], 1);
  EXPECT_EQ(test2[0][0], 1.0);
  EXPECT_EQ(test2[1][0], 2.5);
  EXPECT_EQ(test2[2][0], 4.125);
  EXPECT_EQ(test2[3][0], 1.0);
}

Matrix<float> *move_func(Matrix<float> &&x) {
  Matrix<float> *out = new Matrix<float>(std::move(x));
  return out;
}

Matrix<float> *copy_func(const Matrix<float> &x) {
  Matrix<float> *out = new Matrix<float>(x);
  return out;
}

TEST(MatrixTest, move_constructor) {
  Matrix<float> *test1 = move_func(std::move(Matrix<float>(4, 4, 0)));
  EXPECT_EQ((*test1).shape[0], 4);
  EXPECT_EQ((*test1).shape[1], 4);
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      EXPECT_EQ((*test1)[i][j], 0);
    }
  }
  float *row1 = new float[1]{1.0};
  float *row2 = new float[1]{2.5};
  float *row3 = new float[1]{4.125};
  float *row4 = new float[1]{1.0};
  float **arr = new float *[4];
  arr[0] = row1;
  arr[1] = row2;
  arr[2] = row3;
  arr[3] = row4;
  Matrix<float> *test2 = move_func(std::move(Matrix<float>{arr, 4, 1}));
  EXPECT_EQ(test2->shape[0], 4);
  EXPECT_EQ(test2->shape[1], 1);
  EXPECT_EQ((*test2)[0][0], 1.0);
  EXPECT_EQ((*test2)[1][0], 2.5);
  EXPECT_EQ((*test2)[2][0], 4.125);
  EXPECT_EQ((*test2)[3][0], 1.0);
  delete test1;
  delete test2;
}

TEST(MatrixTest, copy_constructor) {
  Matrix<float> *test1 = copy_func(Matrix<float>{4, 4, 0});
  EXPECT_EQ((*test1).shape[0], 4);
  EXPECT_EQ((*test1).shape[1], 4);
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      EXPECT_EQ((*test1)[i][j], 0);
    }
  }
  float *row1 = new float[1]{1.0};
  float *row2 = new float[1]{2.5};
  float *row3 = new float[1]{4.125};
  float *row4 = new float[1]{1.0};
  float **arr = new float *[4];
  arr[0] = row1;
  arr[1] = row2;
  arr[2] = row3;
  arr[3] = row4;
  Matrix<float> *test2 = copy_func(Matrix<float>{arr, 4, 1});
  EXPECT_EQ(test2->shape[0], 4);
  EXPECT_EQ(test2->shape[1], 1);
  EXPECT_EQ((*test2)[0][0], 1.0);
  EXPECT_EQ((*test2)[1][0], 2.5);
  EXPECT_EQ((*test2)[2][0], 4.125);
  EXPECT_EQ((*test2)[3][0], 1.0);
  delete test1;
  delete test2;
}

TEST(MatrixTest, copy_assignment) {
  Matrix<float> test1 = Matrix<float>(4, 4, 0);
  EXPECT_EQ(test1.shape[0], 4);
  EXPECT_EQ(test1.shape[1], 4);
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      EXPECT_EQ(test1[i][j], 0);
    }
  }
  float *row1 = new float[1]{1.0};
  float *row2 = new float[1]{2.5};
  float *row3 = new float[1]{4.125};
  float *row4 = new float[1]{1.0};
  float **arr = new float *[4];
  arr[0] = row1;
  arr[1] = row2;
  arr[2] = row3;
  arr[3] = row4;
  Matrix<float> test2 = Matrix<float>{arr, 4, 1};
  EXPECT_EQ(test2.shape[0], 4);
  EXPECT_EQ(test2.shape[1], 1);
  EXPECT_EQ(test2[0][0], 1.0);
  EXPECT_EQ(test2[1][0], 2.5);
  EXPECT_EQ(test2[2][0], 4.125);
  EXPECT_EQ(test2[3][0], 1.0);
}

TEST(VectorTest, instantiation) {
  Vector<float> mat1 = Vector<float>(5, 0);
  EXPECT_EQ(mat1.shape[0], 1);
  EXPECT_EQ(mat1.shape[1], 5);
  EXPECT_EQ(mat1.get_value(0), 0.0);
  EXPECT_EQ(mat1.get_value(1), 0.0);
  EXPECT_EQ(mat1.get_value(2), 0.0);
  EXPECT_EQ(mat1.get_value(3), 0.0);
  EXPECT_EQ(mat1.get_value(4), 0.0);
  Vector<float> mat3{3, 10.0};
  EXPECT_EQ(mat3.shape[0], 1);
  EXPECT_EQ(mat3.shape[1], 3);
  EXPECT_EQ(mat3.get_value(0), 10.0);
  EXPECT_EQ(mat3.get_value(1), 10.0);
  EXPECT_EQ(mat3.get_value(2), 10.0);

  Vector<float> mat6{}; // Testing that this runs. Expected outcome is that it does nothing.
}

TEST(VectorTest, operator_indexing) {
  Vector<float> test1 = Vector<float>(3, 0);
  test1.set_value(0, 10);
  test1.set_value(1, 20);

  EXPECT_EQ(test1[0], 10.0f);
  EXPECT_EQ(test1[1], 20.0f);
  EXPECT_EQ(test1[2], 0.0f);

  // Note that if the first index is within bounds but the second index is out of bounds, then undefined behavior occurs.
  // i.e. test1[2][10] will have undefined behavior. There is no way to force array indexing limit.
  EXPECT_THROW(test1[4], std::invalid_argument);
  EXPECT_THROW(test1[10], std::invalid_argument);
  EXPECT_THROW(test1[-1], std::invalid_argument);
}

TEST(VectorTest, operator_add) {
  Vector<float> left1 = Vector<float>(4, 0);
  Vector<float> right1 = Vector<float>(4, 0);
  EXPECT_EQ((left1 + right1)[2], Vector<float>(4, 0)[2]);

  left1.set_value(0, 10);
  left1.set_value(1, 20);
  left1.set_value(1, 0.123123f);
  Vector<float> actual = Vector<float>(4, 0);
  actual.set_value(0, 10);
  actual.set_value(1, 20);
  actual.set_value(1, 0.123123f);
  for (int i = 0; i < actual.shape[0]; i++) {
    EXPECT_FLOAT_EQ((left1 + right1)[i], actual[i]);
  }

  // right1 += left1;
  // for (int i = 0; i < actual.shape[0]; i++) {
  //   EXPECT_FLOAT_EQ(right1[i], actual[i]);
  // }
}
