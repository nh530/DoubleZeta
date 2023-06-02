#include "regression/Regression.h"
#include "typing/Matrix.h"
#include <gtest/gtest.h>
#include <iostream>
#include <stdexcept>
#include <vector>

TEST(LinearRegressionTest, coefficients) {
  Matrix obs{3, 1, 1};
  obs[0][0] = 10;
  obs[1][0] = 20;
  obs[2][0] = 30;

  Matrix y_obs{3, 1};
  y_obs[0][0] = 11;
  y_obs[1][0] = 25;
  y_obs[2][0] = 32;
  LinearRegression model = LinearRegression(.00011, 1.0, false);
  model.fit(obs, y_obs);
	EXPECT_NEAR(model.get_parameters()[0], 1.12f, .01);
	EXPECT_FLOAT_EQ(model.get_parameters()[1], 0.0f);
}
