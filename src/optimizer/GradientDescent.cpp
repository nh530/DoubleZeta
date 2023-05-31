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
    std::cout << "Weights: " << weights[0][0] << '\n';
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

LinearRegression::LinearRegression(float step_size, float tolerance, bool fit_intercept)
    : _fit_intercept{fit_intercept}, objective{new MeanSquaredError()}, optimizer{new GradientDescent(step_size, tolerance)} {
  _fit_flag = false;
}

Matrix LinearRegression::gradient(Matrix &weights, Matrix &x, Matrix &y) {
  Matrix out;
  out = (2 * x.transpose() * x * weights) - (2 * x.transpose() * y);
  return out;
}

float LinearRegression::score() {
  if (!_fit_flag)
    throw InvalidModel("The model has not been fit yet!");
  Matrix y_pred = this->predict(x_obs);
  return objective->objective_func(y_obs, y_pred);
}

void _val_model() {
  // TODO: validate model
  if (true)
    throw InvalidModel();
}

Matrix LinearRegression::predict(Matrix &x) {
  if (!_fit_flag)
    throw InvalidModel("The model has not yet been fit!");
  return _A.transpose() * x;
}

float LinearRegression::predict(float x) {
  if (!_fit_flag)
    throw InvalidModel();
  return (_A * x)[0][0];
}

float LinearRegression::get_intercept() { return _A[0][_A.shape[1]]; }

std::vector<float> LinearRegression::get_parameters() {
  if (!_fit_flag)
    throw InvalidModel("The model has not yet been fit!");
  return std::vector<float>{*_A[0]};
}

void LinearRegression::print() {
  for (int j = 0; j < _A.shape[1]; j++) {
    std::cout << "coef " << j << ":" << _A[0][j] << '\t';
  }
}

void LinearRegression::_verify_model(Matrix &x, Matrix &y) {
  // TODO: Verify assumptions hold.
  if (y.shape[0] < 30)
    throw InvalidModel("There should be more than 30 records to train the model.");
}

Matrix LinearRegression::_prep_train_data(Matrix &x) {
  // Add a column of 1's to the right most side of the matrix. This represents the intercept.
  Matrix out{x.shape[0], x.shape[1] + 1, 1.0f};
  if (_fit_intercept) {
    for (int i = 0; i < x.shape[0]; i++) {
      for (int j = 0; j < x.shape[1]; j++) {
        out[i][j] = x[i][j];
      }
    }
  }
  // TODO: Placeholder for adding other preprocessing steps as needed.
  return out;
}

void LinearRegression::fit(Matrix &x, Matrix &y) {
  /* Executes gradient descent to determine the optimal weights for the Linear Regression model for the observed input dataset.
   * */
  std::function<Matrix(Matrix &, Matrix &, Matrix &)> new_grad =
      std::bind(&LinearRegression::gradient, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
  if (_fit_intercept) {
    Matrix new_x = _prep_train_data(x);
    _A = optimizer->optimize(new_grad, new_x, y); // Returns the most optimal weights.
  } else {
    _A = optimizer->optimize(new_grad, x, y);
  }
  _fit_flag = true;
}
