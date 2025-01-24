#include "typing/Cube.h"
#include <gtest/gtest.h>
#include <iostream>
#include <stdexcept>
#include <vector>

TEST(CubeTest, instantiation) {
  Cube<float> mat1 = Cube<float>(3, 3, 1, 0);
  EXPECT_EQ(mat1.shape[1], 3);
  EXPECT_EQ(mat1.shape[0], 3);
  EXPECT_EQ(mat1.shape[2], 1);
  Cube<float> mat2 = Cube<float>(10, 20, 3);
  EXPECT_EQ(mat2.shape[0], 10);
  EXPECT_EQ(mat2.shape[1], 20);
  EXPECT_EQ(mat2.shape[2], 3);

  Cube<float> mat3{3, 1, 2, 10.0};
  EXPECT_EQ(mat3.shape[0], 3);
  EXPECT_EQ(mat3.shape[1], 1);
  EXPECT_EQ(mat3.shape[2], 2);
  EXPECT_EQ(mat3(0, 0, 0), 10.0);
  EXPECT_EQ(mat3(1, 0, 0), 10.0);
  EXPECT_EQ(mat3(2, 0, 0), 10.0);
  EXPECT_EQ(mat3(0, 0, 1), 10.0);
  EXPECT_EQ(mat3(1, 0, 1), 10.0);
  EXPECT_EQ(mat3(2, 0, 1), 10.0);
  float *row1 = new float[1];
  float *row2 = new float[1];
  float *row3 = new float[1];
  float *row4 = new float[1];
  float ***arr = new float **[4];
  arr[0] = new float *[1];
  arr[1] = new float *[1];
  arr[2] = new float *[1];
  arr[3] = new float *[1];
  arr[0][0] = row1;
  arr[1][0] = row2;
  arr[2][0] = row3;
  arr[3][0] = row4;
  arr[0][0][0] = 1.0;
  arr[1][0][0] = 2.5;
  arr[2][0][0] = 4.125;
  arr[3][0][0] = 1.0;
  Cube<float> mat4{arr, 4, 1, 1};
  EXPECT_EQ(mat4(0, 0, 0), 1.0);
  EXPECT_EQ(mat4(1, 0, 0), 2.5);
  EXPECT_EQ(mat4(2, 0, 0), 4.125);
  EXPECT_EQ(mat4(3, 0, 0), 1.0);
  delete[] row1;
  delete[] row2;
  delete[] row3;
  delete[] row4;
  delete[] arr[0];
  delete[] arr[1];
  delete[] arr[2];
  delete[] arr[3];
  delete[] arr;

  arr = new float **[2];
  row1 = new float[1];
  row2 = new float[1];
  row3 = new float[1];
  row4 = new float[1];
  arr[0] = new float *[2];
  arr[1] = new float *[2];
  arr[0][0] = row1;
  arr[1][0] = row2;
  arr[0][1] = row3;
  arr[1][1] = row4;
  arr[0][0][0] = 1.0;
  arr[0][1][0] = 0.0;
  arr[1][0][0] = 0.0;
  arr[1][1][0] = 2.5;
  Cube<float> mat5{arr, 2, 2, 1};
  EXPECT_EQ(mat5(0, 0, 0), 1.0);
  EXPECT_EQ(mat5(0, 1, 0), 0.0);
  EXPECT_EQ(mat5(1, 0, 0), 0.0);
  EXPECT_EQ(mat5(1, 1, 0), 2.5);

  delete[] row1;
  delete[] row2;
  delete[] row3;
  delete[] row4;
  delete[] arr[0];
  delete[] arr[1];
  delete[] arr;

  Cube<float> mat6{}; // Testing that this runs. Expected outcome is that it does nothing.
}

TEST(CubeTest, operator_indexing) {
  Cube<float> test1 = Cube<float>(3, 1, 1, 0);
  test1.set_value(0, 0, 0, 10.0f);
  test1.set_value(1, 0, 0, 20.0f);

  EXPECT_EQ(test1(0, 0, 0), 10.0f);
  EXPECT_EQ(test1(1, 0, 0), 20.0f);
  EXPECT_EQ(test1(2, 0, 0), 0.0f);

  // Note that if the first index is within bounds but the second index is out of bounds, then undefined behavior occurs.
  // i.e. test1[2][10] will have undefined behavior. There is no way to force array indexing limit.
  EXPECT_THROW(test1(4, 0, 0), std::invalid_argument);
  EXPECT_THROW(test1(0, 10, 0), std::invalid_argument);
  EXPECT_THROW(test1(2, 0, 3), std::invalid_argument);
}

TEST(CubeTest, operator_add) {
  Cube<float> left1 = Cube<float>(4, 4, 1, 0);
  Cube<float> right1 = Cube<float>(4, 4, 1, 0);
  EXPECT_EQ((left1 + right1)(0, 2, 0), (Cube<float>(4, 4, 1, 0))(0, 2, 0));
  left1.set_value(0, 0, 0, 10);
  left1.set_value(1, 1, 0, 20);
  left1.set_value(1, 3, 0, 0.123123f);
  Cube<float> actual = Cube<float>(4, 4, 1, 0);
  actual.set_value(0, 0, 0, 10);
  actual.set_value(1, 1, 0, 20);
  actual.set_value(1, 3, 0, 0.123123f);
  for (int i = 0; i < actual.shape[0]; i++) {
    for (int j = 0; j < actual.shape[1]; j++) {
      EXPECT_FLOAT_EQ((left1 + right1)(i, j, 0), actual(i, j, 0));
    }
  }

  right1 += left1;
  for (int i = 0; i < actual.shape[0]; i++) {
    for (int j = 0; j < actual.shape[1]; j++) {
      EXPECT_FLOAT_EQ(right1(i, j, 0), actual(i, j, 0));
    }
  }
}

TEST(CubeTest, operator_add_2) {
  Cube<float> left1 = Cube<float>(4, 4, 1, 0);
  Cube<float> right1 = Cube<float>(4, 4, 1, 0);
  right1.set_value(0, 0, 0, 0.43f);
  right1.set_value(0, 3, 0, 4.91f);
  right1.set_value(1, 3, 0, 2.1f);
  right1.set_value(3, 3, 0, 900.2f);
  left1.set_value(0, 0, 0, 10);
  left1.set_value(1, 1, 0, 20);
  left1.set_value(1, 3, 0, 0.123123f);
  Cube<float> actual = Cube<float>(4, 4, 1, 0);
  actual.set_value(0, 0, 0, 10.43f);
  actual.set_value(0, 3, 0, 4.91f);
  actual.set_value(1, 1, 0, 20);
  actual.set_value(1, 3, 0, 2.223123f);
  actual.set_value(3, 3, 0, 900.2f);
  for (int i = 0; i < actual.shape[0]; i++) {
    for (int j = 0; j < actual.shape[1]; j++) {
      EXPECT_FLOAT_EQ((left1 + right1)(i, j, 0), actual(i, j, 0));
    }
  }
  left1 += right1;
  for (int i = 0; i < actual.shape[0]; i++) {
    for (int j = 0; j < actual.shape[1]; j++) {
      EXPECT_FLOAT_EQ(left1(i, j, 0), actual(i, j, 0));
    }
  }
}

TEST(CubeTest, operator_add_3) {
  Cube<float> left1 = Cube<float>(4, 4, 1, 0);
  Cube<float> right1 = Cube<float>(2, 4, 1, 0);
  EXPECT_THROW(left1 + right1, std::invalid_argument);
  EXPECT_THROW(left1 + Cube<float>(4, 2, 1, 0), std::invalid_argument);
  EXPECT_THROW(Cube<float>(2, 2, 1, 0) + Cube<float>(4, 2, 1, 0), std::invalid_argument);
  EXPECT_THROW(Cube<float>(0, 0, 0) + Cube<float>(4, 2, 1, 0),
               std::invalid_argument); // NOTE: Cube(0, 0, 0) has undefined behavior because array values not defined.
  EXPECT_FLOAT_EQ((Cube<float>(1, 1, 1) + Cube<float>(1, 1, 1))(0, 0, 0), (Cube<float>(1, 1, 1))(0, 0, 0));
  EXPECT_THROW(left1 += right1, std::invalid_argument);
}

TEST(CubeTest, operator_sub) {
  Cube<float> left1 = Cube<float>(4, 4, 1, 0);
  Cube<float> right1 = Cube<float>(4, 4, 1, 0);
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      EXPECT_EQ((left1 - right1)(i, j, 0), Cube<float>(4, 4, 1, 0)(i, j, 0));
    }
  }

  right1.set_value(0, 0, 0, 10.0f);
  right1.set_value(1, 1, 0, 20.0f);
  right1.set_value(1, 3, 0, 0.123123f);
  Cube<float> actual = Cube<float>(4, 4, 1, 0);
  actual.set_value(0, 0, 0, -10.0f);
  actual.set_value(1, 1, 0, -20.0f);
  actual.set_value(1, 3, 0, -0.123123f);
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      EXPECT_FLOAT_EQ((left1 - right1)(i, j, 0), actual(i, j, 0));
    }
  }

  left1 -= right1;
  for (int i = 0; i < actual.shape[0]; i++) {
    for (int j = 0; j < actual.shape[1]; j++) {
      EXPECT_FLOAT_EQ(left1(i, j, 0), actual(i, j, 0));
    }
  }

  Cube<float> left2 = Cube<float>(4, 3, 0);
  Cube<float> right2 = Cube<float>(3, 3, 0);
  EXPECT_THROW(left2 - right2, std::invalid_argument);
  EXPECT_THROW(left2 -= right2, std::invalid_argument);
}

TEST(CubeTest, operator_multi) {
  Cube<float> left1 = Cube<float>(4, 4, 1, 0);
  Cube<float> right1 = Cube<float>(4, 4, 1, 0);
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      EXPECT_FLOAT_EQ((left1 * right1)(i, j, 0), Cube<float>(4, 4, 1, 0)(i, j, 0));
    }
  }

  right1.set_value(0, 0, 0, 0.43f);
  right1.set_value(0, 3, 0, 4.91f);
  right1.set_value(1, 3, 0, 2.1f);
  right1.set_value(3, 3, 0, 900.2f);
  left1.set_value(0, 0, 0, 10);
  left1.set_value(1, 1, 0, 20);
  left1.set_value(1, 3, 0, 0.123123f);
  Cube<float> actual = Cube<float>(4, 4, 1, 0);
  actual.set_value(0, 0, 0, 4.3f);
  actual.set_value(0, 3, 0, 49.1f);
  actual.set_value(1, 3, 0, 152.835325f);
  auto res2 = left1 * right1;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      EXPECT_FLOAT_EQ(res2(i, j, 0), actual(i, j, 0));
    }
  }
  Cube<float> left2 = Cube<float>(1, 3, 1, 2);
  Cube<float> right2 = Cube<float>(3, 1, 1, 4.125);
  auto res = left2 * right2;
  EXPECT_EQ(res(0, 0, 0), 24.75f);

  Cube<float> left3 = Cube<float>(3, 1, 1, 0);
  Cube<float> right3 = Cube<float>(3, 1, 1, 0);
  EXPECT_THROW(left3 * right3, std::invalid_argument);
}

TEST(CubeTest, operator_multi2) {
  Cube<float> left1 = Cube<float>(4, 4, 1, 1.0f);
  Cube<float> right1 = Cube<float>(4, 4, 1, 1.0f);
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      EXPECT_FLOAT_EQ((left1 * right1)(i, j, 0), Cube<float>(4, 4, 1, 4.0f)(i, j, 0));
      EXPECT_FLOAT_EQ((2 * right1)(i, j, 0), Cube<float>(4, 4, 1, 2.0f)(i, j, 0));
      EXPECT_FLOAT_EQ((right1 * 2)(i, j, 0), Cube<float>(4, 4, 1, 2.0f)(i, j, 0));
      EXPECT_FLOAT_EQ((0.12f * right1)(i, j, 0), Cube<float>(4, 4, 1, 0.12f)(i, j, 0));
      EXPECT_FLOAT_EQ((right1 * 0.12f)(i, j, 0), Cube<float>(4, 4, 1, 0.12f)(i, j, 0));
    }
  }
}

TEST(CubeTest, operator_equality) {
  Cube<float> left1 = Cube<float>(4, 4, 1, 1.0);
  Cube<float> right1 = Cube<float>(4, 4, 1, 1.0);
  EXPECT_TRUE(left1 == right1);
  EXPECT_FALSE(left1 != right1);
  EXPECT_TRUE((2 * left1) != right1);
  EXPECT_FALSE((2 * left1) == right1);
}

TEST(DISABLED_CubeTest, operator_transpose) {
  Cube<float> left1 = Cube<float>(4, 4, 1, 1.0f);
  Cube<float> actual = Cube<float>(4, 4, 1, 1.0f);
  left1.set_value(0, 0, 1, 10.0f);
  left1.set_value(0, 3, 1, 2.1f);
  left1.set_value(1, 2, 1, 5.3f);
  actual.set_value(0, 0, 1, 10.0f);
  actual.set_value(3, 0, 1, 2.1f);
  actual.set_value(2, 1, 1, 5.3f);
  Cube<float> obs = left1.transpose();
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      EXPECT_FLOAT_EQ(obs(i, j, 0), actual(i, j, 0));
    }
  }

  Cube<float> left2 = Cube<float>(3, 1, 1, 2.5f);
  auto res = left2.transpose();
  EXPECT_EQ(res.shape[0], 1);
  EXPECT_EQ(res.shape[1], 3);
  EXPECT_EQ(res(0, 0, 0), 2.5f);
  EXPECT_EQ(res(0, 1, 0), 2.5f);
  EXPECT_EQ(res(0, 2, 0), 2.5f);

  Cube<float> left3 = Cube<float>(3, 2, 1, 2.5f);
  Cube<float> res2 = left3.transpose();
  EXPECT_EQ(res2.shape[0], 2);
  EXPECT_EQ(res2.shape[1], 3);
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 3; j++) {
      EXPECT_EQ(res2(i, j, 0), 2.5f);
    }
  }
}

TEST(CubeTest, move_assignment) {
  Cube<float> test1 = std::move(Cube<float>(4, 4, 1, 0));
  EXPECT_EQ(test1.shape[0], 4);
  EXPECT_EQ(test1.shape[1], 4);
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      EXPECT_EQ(test1(i, j, 0), 0);
    }
  }
}

Cube<float> *move_func(Cube<float> &&x) {
  Cube<float> *out = new Cube<float>(std::move(x));
  return out;
}

Cube<float> *copy_func(const Cube<float> &x) {
  Cube<float> *out = new Cube<float>(x);
  return out;
}

TEST(CubeTest, move_constructor) {
  Cube<float> *test1 = move_func(std::move(Cube<float>(4, 4, 1, 0)));
  EXPECT_EQ((*test1).shape[0], 4);
  EXPECT_EQ((*test1).shape[1], 4);
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      EXPECT_EQ((*test1)(i, j, 0), 0);
    }
  }
  delete test1;
}

TEST(CubeTest, copy_constructor) {
  Cube<float> *test1 = copy_func(Cube<float>{4, 4, 1, 0});
  EXPECT_EQ((*test1).shape[0], 4);
  EXPECT_EQ((*test1).shape[1], 4);
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      EXPECT_EQ((*test1)(i, j, 0), 0);
    }
  }
  delete test1;
}

TEST(CubeTest, copy_assignment) {
  Cube<float> test1 = Cube<float>(4, 4, 1, 0);
  EXPECT_EQ(test1.shape[0], 4);
  EXPECT_EQ(test1.shape[1], 4);
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      EXPECT_EQ(test1(i, j, 0), 0);
    }
  }
}
