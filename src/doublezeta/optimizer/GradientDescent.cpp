#include "GradientDescent.h"
#include "typing/DTypes.h"
#include "typing/Matrix.h"
#include "typing/exceptions.h"
#include <cmath>
#include <functional>
#include <iostream>
#include <random>

GradientDescent::GradientDescent(float step_size, float tolerance) : _step{step_size}, _tol{tolerance} {}

Matrix<float> GradientDescent::_set_initial(int rows, int cols, int seed) {
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
  Matrix<float> out(nums, rows, cols);
  return out;
};

Matrix<float> GradientDescent::optimize(std::function<Matrix<float>(Matrix<float> &, Matrix<float> &, Matrix<float> &)> gradient_fn, Matrix<float> &x,
                                        Matrix<float> &y, const Matrix<float> &init_weights) {
  /*
   * Gradient function with respect to the input weights vector x.
   * A 1 by 1 matrix of 0 is used as the sentinel value to signal the algorithm to generate its own initial weight.
   *
   * */
  Matrix<float> weights{x.shape[1], 1}; //  temporary initialization; TODO: Implement copy initialization.
  if (init_weights == Matrix<float>{1, 1, 0})
    weights = _set_initial(x.shape[1], 1);
  else
    weights = init_weights;
  Matrix<float> gradient = gradient_fn(weights, x, y);
  float stable_c = 0.0000001; // Stabilizing constant against numerical underflow.
  while (sqrt((gradient.transpose() * gradient)[0][0]) + stable_c > _tol) {
    if (sqrt((gradient.transpose() * gradient)[0][0]) > 5000.0f) {
      std::cout << "Warning: The gradient needed to be reduced because it was too large." << '\n';
      std::cout << "Please reduce the step_size parameter in order to converge onto the minimum" << '\n';
      for (int i = 0; i < gradient.shape[0]; i++) {
        // Gradient reduction heuristic
        gradient.set_value(i, 0, (gradient[i][0] - (3 * gradient[i][0] / 4)));
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

  float out = 0;                     // Will get undefined behavior in output if don't initialize.
  unsigned int size = y_pred.size(); // Number of elements
  for (unsigned int i = 0; i < size; i++) {
    out += pow(y_pred[i] - y_act[i], 2);
  }
  return out / size;
}

float MeanSquaredError::objective_func(Matrix<float> &y_pred, Matrix<float> &y_act) {
  /* The below is the mathematical equation:
   * MSE = $1/n * \sum_{i=1}^{n} (y_pred - y_act)^{2}$
   *
   * */
  if (y_pred.shape[0] != y_act.shape[0] || y_pred.shape[1] != y_act.shape[1])
    throw std::invalid_argument("The size of the Matrices do not match!");
  if (!(y_pred.shape[0] != 1) && (y_pred.shape[1] == 1) && !(y_pred.shape[0] == 1) && (y_pred.shape[1] != 1))
    throw std::invalid_argument("The y_pred Matrix is not a vector! i.e. length of row or length of column is not 1.");
  if (!(y_act.shape[0] != 1) && (y_act.shape[1] == 1) && !(y_act.shape[0] == 1) && (y_act.shape[1] != 1))
    throw std::invalid_argument("The y_act Matrix is not a vector! i.e. length of row or length of column is not 1.");

  float out = 0;
  unsigned int size;
  if (y_pred.shape[0] > 1) {
    size = y_pred.shape[0]; // Number of elements
    for (unsigned int i = 0; i < size; i++) {
      out += pow(y_pred[i][0] - y_act[i][0], 2);
    }
  } else if (y_pred.shape[1] > 1) {
    size = y_pred.shape[1];
    for (unsigned int i = 0; i < size; i++) {
      out += pow(y_pred[0][i] - y_act[0][i], 2);
    }
  }
  return out / size;
}

BinaryCrossEntropy::BinaryCrossEntropy() {}
float BinaryCrossEntropy::objective_func(std::vector<float> &y_pred, std::vector<float> &y_act) {
  /* The below is the mathematical equation:
   * BCE = $-1/n * \sum_{i=1}^{n} y_{act}*ln(y_{pred}) + (1 - y_{act})ln(1 - y_{pred})$
   *
   * */

  if (y_pred.size() != y_act.size())
    throw std::invalid_argument("The size of the vectors do not match!");

  float out = 0;
  unsigned int size = y_pred.size();
  for (unsigned int i = 0; i < size; i++) {
    // This runtime checking might slowdown the code but a significant factor.
    // Float comparison might also be problematic.
    // if (!(0.000000f <= y_pred[i] < 1.0000001f) || !(0.000000f <= y_act[i] < 1.0000001f))
    if ((y_pred[i] > 1.0000000f) || (y_pred[i] < 0.0000000f)) {
      std::cout << "A value of of " << y_pred[i] << " and " << y_act[i] << " was found. " << '\n';
      throw std::invalid_argument("The range of values is not between 0 and 1.");
    }
    out += (y_act[i] * log(y_pred[i])) + ((1 - y_act[i]) * log(1 - y_pred[i]));
  }
  return (-1) * out / size;
}

float BinaryCrossEntropy::objective_func(Matrix<float> &y_pred, Matrix<float> &y_act) {}
