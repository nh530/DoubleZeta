#include "GradientDescent.h"
#include "typing/DTypes.h"
#include "typing/Matrix.h"
#include "typing/exceptions.h"
#include <cmath>
#include <functional>
#include <iostream>
#include <random>

GradientDescent::GradientDescent(float step_size, float tolerance) : _step{step_size}, _tol{tolerance} {}

Matrix GradientDescent::_set_initial(int rows, int cols, int seed) {
  // TODO: Implement a initialization protocol from a paper or textbook.
  // Initialize matrix with 2-d array with random floats between 2.0 and 4.0;
  float **nums = new float *[rows];
  std::mt19937 gen;
  gen.seed(seed);
  std::uniform_real_distribution<float> gener(2.0, 4.0);
  for (int i = 0; i < rows; i++) {
    nums[i] = new float[cols];
    for (int j = 0; j < cols; j++) {
      nums[i][j] = gener(gen);
    }
  }
  Matrix out(nums, rows, cols);
  return out;
};

Matrix GradientDescent::optimize(std::function<Matrix(Matrix &, Matrix &, Matrix &)> gradient_fn, Matrix &x, Matrix &y) {
  /*
   * Gradient function with respect to the input weights vector x.
   *
   * */
  Matrix weights = _set_initial(x.shape[1], 1);
  Matrix gradient = gradient_fn(weights, x, y);
  float stable_c = 0.0000001; // Stabilizing constant against numerical underflow.
  while (sqrt((gradient.transpose() * gradient)[0][0]) + stable_c > _tol) {
    if (sqrt((gradient.transpose() * gradient)[0][0]) > 5000.0f) {
      std::cout << "Warning: The gradient needed to be reduced because it was too large." << '\n';
      std::cout << "Please reduce the step_size parameter in order to converge onto the minimum" << '\n';
      for (int i = 0; i < gradient.shape[0]; i++) {
        // Gradient reduction heuristic
        gradient[i][0] = gradient[i][0] - (3 * gradient[i][0] / 4);
      }
    }
    weights = weights - (_step * gradient);
    gradient = gradient_fn(weights, x, y); // calculate gradient with new x.
  }
  return weights;
}

MeanSquaredError::MeanSquaredError() {}
float MeanSquaredError::objective_func(std::vector<float> &y_pred, std::vector<float> &y_act) {
  /* The below is the mathematical equation:
   * MSE = $1/n * \sum_{i=1}^{n} (y_pred - y_act)^{2}$
   *
   * */
  if (y_pred.size() != y_act.size())
    throw std::invalid_argument("The size of the vectors do not match!");

  float out;
  unsigned int size = y_pred.size(); // Number of elements
  for (unsigned int i = 0; i < size; i++) {
    out += pow(y_pred[i] - y_act[i], 2);
  }
  return out / size;
}

float MeanSquaredError::objective_func(Matrix &y_pred, Matrix &y_act) {
  /* The below is the mathematical equation:
   * MSE = $1/n * \sum_{i=1}^{n} (y_pred - y_act)^{2}$
   *
   * */
  if (y_pred.shape[0] != y_act.shape[0] & y_pred.shape[1] != y_act.shape[1])
    throw std::invalid_argument("The size of the Matrices do not match!");
  if (y_pred.shape[0] != 1 || y_pred.shape[1] != 1)
    throw std::invalid_argument("The y_pred Matrix is not a vector! i.e. length of row or length of column is not 1.");
  if (y_act.shape[0] != 1 || y_act.shape[1] != 1)
    throw std::invalid_argument("The y_act Matrix is not a vector! i.e. length of row or length of column is not 1.");

  float out;
  unsigned int size;
  if (y_pred.shape[0] > 1) {
    unsigned int size = y_pred.shape[0]; // Number of elements
    for (unsigned int i = 0; i < size; i++) {
      out += pow(y_pred[i][0] - y_act[i][0], 2);
    }
  } else if (y_pred.shape[1] > 1) {
    unsigned int size = y_pred.shape[1];
    for (unsigned int i = 0; i < size; i++) {
      out += pow(y_pred[0][i] - y_act[0][i], 2);
    }
  }
  return out / size;
}

float BinaryCrossEntropy::objective_func(std::vector<float> &y_pred, std::vector<float> &y_act) {
  float out;
  unsigned int size = y_pred.size();
  for (unsigned int i = 0; i < size; i++) {
    out += (y_act[i] * log(y_pred[i])) + ((1 - y_act[i]) * log(1 - y_pred[i]));
  }
  return out / size;
}
