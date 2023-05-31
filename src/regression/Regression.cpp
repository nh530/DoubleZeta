#include "Regression.h"
#include <iostream>

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
