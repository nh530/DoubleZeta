/* Faster, smaller, and more trivial sanity checks.
 * TODO: Not fully tested.
 *
 * */
#include "regression/Regression.h"
#include "typing/exceptions.h"
#include <gtest/gtest.h>
#include <iostream>
#include <stdexcept>
#include <vector>

TEST(LinearRegressionTest, throwerror) {
  LinearRegression lr = LinearRegression(.00011, 1.0, false);
  Matrix<float> obs{3, 1};
  obs.set_value(0, 0, 10);
  obs.set_value(1, 0, 20);
  obs.set_value(2, 0, 30);

  EXPECT_THROW(lr.predict(obs), InvalidModel);
  EXPECT_THROW(lr.score(), InvalidModel);
  EXPECT_THROW(lr.predict(10.0f), InvalidModel);
  EXPECT_THROW(lr.get_intercept(), InvalidModel);
  EXPECT_THROW(lr.get_parameters(), InvalidModel);
  EXPECT_THROW(lr.print(), InvalidModel);
}

TEST(LinearRegressionTest, get_parameters) {
  LinearRegression lr = LinearRegression(.00011, .01, false);
  Matrix<float> obs{3, 1};
  obs.set_value(0,0, 10);
  obs.set_value(1,0, 20);
  obs.set_value(2,0, 30);

  Matrix<float> y_obs{3, 1};
  y_obs.set_value(0,0,11);
  y_obs.set_value(1,0, 25);
  y_obs.set_value(2,0, 32);
  lr.fit(obs, y_obs);

  EXPECT_NEAR(lr.get_parameters()[0], 1.12f, .1); // It is possible that this test will fail if the optimization algorithm is changed.
  EXPECT_FLOAT_EQ(lr.get_parameters()[1], 0.0f);
}

TEST(LinearRegressionTest, predict) {
  LinearRegression lr = LinearRegression(.00011, .01, false);
  Matrix<float> obs{3, 1};
  obs.set_value(0,0,10);
  obs.set_value(1,0, 20);
  obs.set_value(2,0, 30);

  Matrix<float> y_obs{3, 1};
  y_obs.set_value(0,0, 11);
  y_obs.set_value(1,0, 25);
  y_obs.set_value(2,0, 32);
  lr.fit(obs, y_obs);
  EXPECT_NEAR(lr.predict(0), 0.0f, .00001);
  EXPECT_NEAR(lr.predict(1), lr.get_parameters()[0], .00001);
  EXPECT_NEAR(lr.predict(3), lr.get_parameters()[0] * 3, .00001);
}

TEST(LinearRegressionTest, get_intercept) {
  // Note fit intercept is set to false.
  LinearRegression lr = LinearRegression(.00011, .01, false);
  Matrix<float> obs{3, 1};
  obs.set_value(0,0, 10);
  obs.set_value(1,0, 20);
  obs.set_value(2,0, 30);

  Matrix<float> y_obs{3, 1};
  y_obs.set_value(0,0, 11);
  y_obs.set_value(1,0, 25);
  y_obs.set_value(2,0, 32);
  lr.fit(obs, y_obs);

  EXPECT_NEAR(lr.get_intercept(), 0.0f, .0001);
}

TEST(DISABLED_LinearRegressionTest, score) {
  LinearRegression lr = LinearRegression(.00011, .01, false);
  Matrix<float> obs{3, 1};
  obs.set_value(0,0,10);
  obs.set_value(1,0, 20);
  obs.set_value(2,0, 30);

  Matrix<float> y_obs{3, 1};
  y_obs.set_value(0,0, 11);
  y_obs.set_value(1,0, 25);
  y_obs.set_value(2,0, 32);
  lr.fit(obs, y_obs);
  float coef = lr.get_parameters()[0];
  float actual = pow((coef * 10 + coef * 20 + coef * 30) - (11 + 25 + 32), 2) / 3; // This is the actual mean squared error given input data above.
  EXPECT_NEAR(lr.score(), actual, .001);
}

TEST(LinearRegressionTest, gradient) {
  LinearRegression lr = LinearRegression(.00011, .01, false);
  Matrix<float> obs{3, 1};
  obs.set_value(0,0, 10);
  obs.set_value(1,0,20);
  obs.set_value(2,0, 30);

  Matrix<float> y_obs{3, 1};
  y_obs.set_value(0,0, 11);
  y_obs.set_value(1,0, 25);
  y_obs.set_value(2,0, 32);
  lr.fit(obs, y_obs);

  Matrix<float> w{1, 1, 2};

  Matrix<float> grad = lr.gradient(w, obs, y_obs);
  float actual = 2460.0f;
  EXPECT_NEAR(grad[0][0], actual, 0.0001);
}
