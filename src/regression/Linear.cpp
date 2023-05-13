#include "Linear.h"
#include "typing/DTypes.h"
#include "typing/Matrix.h"
#include <cmath>
#include <stdexcept>
#include <vector>

float **array__init__(float **input) {
  float **out;
  out = new float *[3];
  for (int i; i < 10; i++) {
    out[i] = new float[3]{1, 1, 1};
  }
  return out;
}
LinearRegression::LinearRegression(){ int a;};
float LinearRegression::predict(Matrix x) {
  float out;
  float **nums;

  nums = array__init__(nums);
  Matrix term1{nums, 3, 3};
  Matrix b{3, 1};
  b[0][0] = -1;
  b[1][0] = -2;
  b[2][0] = -3;
  Matrix inter = std::move(0.5f * term1 * x);
  out = (std::move((inter - b).transpose()) * (inter - b))[0][0];
  return out;
}
float LinearRegression::_calculate_loss(std::vector<float>) {
  float out;

  return out;
}
