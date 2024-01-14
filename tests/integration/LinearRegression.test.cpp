/* The collection of integration tests for Linear Regression API are expected to take longer to complete and will test multiple underlying features working together in unison to produce an output.
 * For example, a test might be a test on how well the model performs and the validity of the model when training on larger amounts of data.
 *
 * */

#include "regression/Regression.h"
#include "typing/Matrix.h"
#include <gtest/gtest.h>
#include <iostream>
#include <stdexcept>
#include <vector>

TEST(LinearRegressionTest, integration_test) {
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

  LinearRegression model2 = LinearRegression(.00011, .001, true); // If the tolerance is not low enough, the coefficients do not converge to true values
  model2.fit(obs, y_obs);
  model2.print();
	EXPECT_NEAR(model2.get_parameters()[0], 1.05, .01);
	EXPECT_NEAR(model2.get_parameters()[1], 1.66, .01);
}
