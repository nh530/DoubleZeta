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
  Matrix<float> _A = Matrix<float>(nums, 3, 3);

  Matrix<float> obs{3, 1, 1};
  obs.set_value(0,0, 10);
  obs.set_value(1,0, 20);
  obs.set_value(2,0, 30);

  Matrix<float> y_obs{3, 1};
  y_obs.set_value(0,0,11);
  y_obs.set_value(1,0,25);
  y_obs.set_value(2,0,32);
  LinearRegression model = LinearRegression(.00011, 1.0, false);
  model.fit(obs, y_obs);
  std::cout << model.get_parameters()[0] << '\t' << model.get_parameters()[1] << '\n';

  LinearRegression model2 = LinearRegression(.00011, .001, true);
  model2.fit(obs, y_obs);
	model2.print();
}
