#include "Linear.h"
#include "typing/DTypes.h"
#include "typing/Matrix.h"
#include <cmath>
#include <iostream>
#include <stdexcept>
#include <vector>

float **array__init__() {
  float **out = new float *[3];
  for (int i = 0; i < 10; i++) {
    out[i] = new float[3]{1, 2, 1};
  }
  return out;
}
LinearModel::LinearModel(float step_size, float tolerance) : _step_size{step_size}, _tolerance{tolerance} {
  // Initailize constants
  _b = Matrix(3, 1, 1.0f);
  _b[0][0] = -1;
  _b[1][0] = -10;
  _b[2][0] = -20;
  float **nums = array__init__();
  _A = Matrix(nums, 3, 3);
};
float LinearModel::predict(Matrix &x) {
  float out;
  Matrix inter = std::move(0.5f * _A * x);
  out = ((inter - _b).transpose() * (inter - _b))[0][0];
  return out;
}
float LinearModel::_calculate_loss(Matrix &curr_val) { return predict(curr_val); }

float LinearModel::_calculate_gradient(Matrix &x) { return ((_A.transpose() * _A * x) - (_A.transpose() * _b))[0][0]; }

Matrix LinearModel::_optimize_loss() {
  // Starting point for optimization.
  Matrix x{3, 1, 10.0f};
  // Gradient of linear function
  // Gradient = A^T * A * x - A^T * b where T is tranpose operator and A is a 3 by 3 matrix and b is a 3 by 1 matrix.
  // The condition is the dot product of the gradient, which gives the size of the gradient.
  Matrix gradient = (_A.transpose() * _A * x) - (_A.transpose() * _b);
  while ((gradient.transpose() * gradient)[0][0] > _tolerance) {
    x = x - _step_size * gradient;
    gradient = (_A.transpose() * _A * x) - (_A.transpose() * _b);
  }
  return x;
}
