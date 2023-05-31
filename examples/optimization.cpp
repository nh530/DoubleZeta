#include "optimizer/GradientDescent.h"
#include "regression/Regression.h"
#include "typing/Matrix.h"
#include <iostream>

float **array__init__() {
  float **out = new float *[3];
  for (int i = 0; i < 10; i++) {
    out[i] = new float[3]{1, 2, 1};
  }
  return out;
}

int main() {
  float **nums = array__init__();
  Matrix _A = Matrix(nums, 3, 3);

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
}
