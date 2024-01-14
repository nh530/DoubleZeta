#include "optimizer/GradientDescent.h"
#include "regression/Regression.h"
#include "typing/Matrix.h"
#include <gtest/gtest.h>
#include <iostream>
#include <stdexcept>
#include <vector>

TEST(MeanSquaredErrorTest, objective_func_vector) {
  MeanSquaredError mse{};
  std::vector<float> y_p{1.0, 2.0, 3.0, 5.1};
  std::vector<float> y_a{2.0, 2.5, 9.8, 5.0};
  float out = mse.objective_func(y_p, y_a);
  float actual = 11.875f;
  EXPECT_FLOAT_EQ(out, actual);
  float out2 = mse.objective_func(y_a, y_p);
  EXPECT_FLOAT_EQ(out2, actual);
  std::vector<float> y_p3{9.7, 8.102, 3.0, 100.12312};
  std::vector<float> y_a3{7.5, 2.1231, 9.8, 5.0};
  float out3 = mse.objective_func(y_p3, y_a3);
  float actual3 = 2283.808801f;
  EXPECT_FLOAT_EQ(out3, actual3);
  float out5 = mse.objective_func(y_a3, y_p3);
  float actual5 = 2283.808801f;
  EXPECT_FLOAT_EQ(out5, actual5);

  std::vector<float> y_p4{9.7, 8.102};
  std::vector<float> y_a4{7.5, 2.1231};
  float out4 = mse.objective_func(y_p4, y_a4);
  float actual4 = 20.293622f;
  EXPECT_FLOAT_EQ(out4, actual4);

  std::vector<float> y_a2{2.0, 2.5};
  EXPECT_THROW(mse.objective_func(y_p, y_a2), std::invalid_argument);
}

TEST(MeanSquaredErrorTest, objective_func_matrice) {
  MeanSquaredError mse{};
  Matrix<float> y_p{3, 1};
  y_p.set_value(0, 0, 0.123f);
  y_p.set_value(1, 0, 20.895f);
  y_p.set_value(2, 0, 6.643015f);
  Matrix<float> y_a{3, 1};
  y_a.set_value(0, 0, 0);
  y_a.set_value(1, 0, 12.94131f);
  y_a.set_value(2, 0, 10912.0f);
  float out = mse.objective_func(y_p, y_a);
  float actual = 39642291.4155f;
  EXPECT_FLOAT_EQ(out, actual);
  float out2 = mse.objective_func(y_a, y_p);
  EXPECT_FLOAT_EQ(out2, actual);
  Matrix<float> y_p3{2, 1};
  y_p3.set_value(0, 0, 0.123f);
  y_p3.set_value(1, 0, 10.895f);
  Matrix < float>y_a3{2, 1};
  y_a3.set_value(0, 0, 10.5423f);
  y_a3.set_value(1, 0, 90.94131f);
  float out3 = mse.objective_func(y_p3, y_a3);
  float actual3 = 3257.986779f;
  EXPECT_FLOAT_EQ(out3, actual3);
  float out4 = mse.objective_func(y_a3, y_p3);
  EXPECT_FLOAT_EQ(out4, actual3);

  Matrix<float> y_p5{1, 2};
  y_p5.set_value(0, 0, 0.123f);
  y_p5.set_value(0, 1, 10.895f);
  Matrix<float> y_a5{1, 2};
  y_a5.set_value(0, 0, 10.5423f);
  y_a5.set_value(0, 1, 90.94131f);
  float out5 = mse.objective_func(y_p5, y_a5);
  float actual5 = 3257.986779f;
  EXPECT_FLOAT_EQ(out5, actual5);
  float out6 = mse.objective_func(y_a5, y_p5);
  EXPECT_FLOAT_EQ(out6, actual3);

  Matrix<float> y_p6{1, 3};
  y_p6.set_value(0, 0, 0.123f);
  y_p6.set_value(0, 1, 20.895f);
  y_p6.set_value(0, 2, 6.643015f);
  Matrix<float> y_a6{1, 3};
  y_a6.set_value(0, 0, 0);
  y_a6.set_value(0, 1, 12.94131f);
  y_a6.set_value(0, 2, 10912.0f);
  float out7 = mse.objective_func(y_p6, y_a6);
  EXPECT_FLOAT_EQ(out7, actual);
  Matrix<float> y_a2{4, 1};
  EXPECT_THROW(mse.objective_func(y_p, y_a2), std::invalid_argument);
  EXPECT_THROW(mse.objective_func(y_a2, y_p), std::invalid_argument);

  Matrix<float> y_a4{2, 3};
  EXPECT_THROW(mse.objective_func(y_p, y_a4), std::invalid_argument);
  EXPECT_THROW(mse.objective_func(y_a4, y_p), std::invalid_argument);

  Matrix<float> err{2, 3};
  EXPECT_THROW(mse.objective_func(err, y_a), std::invalid_argument);
  Matrix<float> err1{3, 4};
  EXPECT_THROW(mse.objective_func(err1, err), std::invalid_argument);
}

TEST(GradientDescentTest, set_initial) {}

TEST(GradientDescentTest, optimize) {
  /* The following test does not test that the gradient descent algorithm will converge onto the optimal set of weights when given the
   * gradient, observed inputs, and observed target variable. It solely tests the scenario when the algorithm encounters the 0
   * gradient, the current weight is returned. Such is case when a gradient returning the 0 matrix is passed into the gradient descent algo.
   * Therefore, the success criteria is that the gradient descent algorithm returns the initial weights passed into it.
   *
   * The test for convergence to the optimal set of weights is left as an integration test.
   * */
  GradientDescent opti = GradientDescent(.0001, .01);
  Matrix<float> obs{3, 1};
  obs.set_value(0, 0, 10);
  obs.set_value(1, 0, 20);
  obs.set_value(2, 0, 30);

  Matrix<float> y_obs{3, 1};
  y_obs.set_value(0, 0, 11);
  y_obs.set_value(1, 0, 25);
  y_obs.set_value(2, 0, 32);
  std::function<Matrix<float>(Matrix<float> &, Matrix<float> &, Matrix<float> &)> gradnt = [](Matrix<float> &, Matrix<float> &,
                                                                                              Matrix<float> &) -> Matrix<float> {
    return Matrix<float>{1, 1, 0};
  };
  Matrix<float> w{1, 1, 1};
  Matrix<float> out = opti.optimize(gradnt, obs, y_obs, w);
  EXPECT_FLOAT_EQ(out[0][0], w[0][0]); // The gradient is 0, so the weights returned should be the same as the weights passed.
  Matrix<float> w2{1, 1, 0};
  int out2 = opti.optimize(gradnt, obs, y_obs, w2)[0][0];
  int i_w2 = w2[0][0];
  EXPECT_NE(out2, i_w2); // Testing new inital weight generated when passing in the sentinal value.
}

TEST(BinaryCrossEntropy, objective_func_vector) {
  BinaryCrossEntropy bce{};
  std::vector<float> y_a{1.0, 0.0, 1.0, 0.0};
  std::vector<float> y_p{0.213491, 0.34, .984, 0.603};
  float out = bce.objective_func(y_p, y_a);
  float actual = 0.7249061f;
  EXPECT_FLOAT_EQ(out, actual);

  std::vector<float> y_a4{1.0, 1.0};
  std::vector<float> y_p4{0.8744301, 0.0123090};
  float out4 = bce.objective_func(y_p4, y_a4);
  float actual4 = 2.2658037f;
  EXPECT_FLOAT_EQ(out4, actual4);

  std::vector<float> y_a2{2.0, 2.5};
  EXPECT_THROW(bce.objective_func(y_p, y_a2), std::invalid_argument);
  std::vector<float> y_p3{0.213491, 1000.0, .984, 0.603};
  std::vector<float> y_a3{2.0, 2.5, 1000.0, 1.0};
  EXPECT_THROW(bce.objective_func(y_p3, y_a3), std::invalid_argument);
}
