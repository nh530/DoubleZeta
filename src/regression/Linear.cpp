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
  _b = Matrix<float>(3, 1, 1.0f);
  _b.set_value(0, 0, -1);
  _b.set_value(1, 0, -10);
  _b.set_value(2, 0, -20);
  float **nums = array__init__();
  _A = Matrix<float>(nums, 3, 3);
};
float LinearModel::predict(Matrix<float> &x) {
  float out;
  Matrix<float> inter = std::move(0.5f * _A * x);
  out = ((inter - _b).transpose() * (inter - _b))[0][0];
  return out;
}
float LinearModel::_calculate_loss(Matrix<float> &curr_val) { return predict(curr_val); }

float LinearModel::_calculate_gradient(Matrix<float> &x) { return ((_A.transpose() * _A * x) - (_A.transpose() * _b))[0][0]; }

Matrix<float> LinearModel::_optimize_loss() {
  // Starting point for optimization.
  Matrix<float> x{3, 1, 10.0f};
  // Gradient of linear function
  // Gradient = A^T * A * x - A^T * b where T is tranpose operator and A is a 3 by 3 matrix and b is a 3 by 1 matrix.
  // The condition is the dot product of the gradient, which gives the size of the gradient.
  Matrix<float> gradient = (_A.transpose() * _A * x) - (_A.transpose() * _b);
  while ((gradient.transpose() * gradient)[0][0] > _tolerance) {
    x = x - _step_size * gradient;
    gradient = (_A.transpose() * _A * x) - (_A.transpose() * _b);
  }
  return x;
}
