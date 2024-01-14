#include "Regression.h"
#include <iostream>

LinearRegression::LinearRegression(float step_size, float tolerance, bool fit_intercept)
    : _fit_intercept{fit_intercept}, objective{new MeanSquaredError()}, optimizer{new GradientDescent(step_size, tolerance)} {
  _fit_flag = false;
}

Matrix<float> LinearRegression::gradient(Matrix<float> &weights, Matrix<float> &x, Matrix<float> &y) {
  Matrix<float> out;
  out = (2 * x.transpose() * x * weights) - (2 * x.transpose() * y);
  return out;
}

float LinearRegression::score() {
  if (!_fit_flag)
    throw InvalidModel("The model has not been fit yet!");
  Matrix<float> y_pred = this->predict(x_obs);
  return objective->objective_func(y_obs, y_pred);
}

Matrix<float> LinearRegression::predict(Matrix<float> &x) {
  if (!_fit_flag)
    throw InvalidModel("The model has not yet been fit!");
  return _A.transpose() * x;
}

float LinearRegression::predict(float x) {
  if (!_fit_flag)
    throw InvalidModel("The model has not yet been fit!");
  Matrix<float> m_x{1, 1, x};
  return (_A * m_x)[0][0];
}

float LinearRegression::get_intercept() {
  if (!_fit_flag)
    throw InvalidModel("The model has not yet been fit!");
  return _A[0][_A.shape[1]];
}

std::vector<float> LinearRegression::get_parameters() {
  if (!_fit_flag)
    throw InvalidModel("The model has not yet been fit!");
  const float *temp = _A.transpose()[0]; // Pointer to existing object. Nothing new being created.
  return std::vector<float>{temp, temp + (_A.transpose().shape[1])};
}

void LinearRegression::print() {
  if (!_fit_flag)
    throw InvalidModel("The model has not yet been fit!");
  for (int i = 0; i < _A.shape[0]; i++) {
    std::cout << "coef_" << i << ": " << _A[i][0] << '\t';
  }
  std::cout << '\n';
}

void LinearRegression::_verify_model(Matrix<float> &x, Matrix<float> &y) {
  // TODO: Verify assumptions hold.
  if (y.shape[0] < 30)
    throw InvalidModel("There should be more than 30 records to train the model.");
}

Matrix<float> LinearRegression::_prep_train_data(Matrix<float> &x) {
  // Add a column of 1's to the right most side of the matrix. This represents the intercept.
  Matrix<float> out{x.shape[0], x.shape[1] + 1, 1.0};

  if (_fit_intercept) {
    for (int i = 0; i < x.shape[0]; i++) {
      for (int j = 0; j < x.shape[1]; j++) {
        out.set_value(i, j, x[i][j]);
      }
    }
  }
  // TODO: Placeholder for adding other preprocessing steps as needed.
  return out;
}

void LinearRegression::fit(Matrix<float> &x, Matrix<float> &y) {
  /* Executes gradient descent to determine the optimal weights for the Linear Regression model for the observed input dataset.
   * */
  std::function<Matrix<float>(Matrix<float> &, Matrix<float> &, Matrix<float> &)> new_grad =
      std::bind(&LinearRegression::gradient, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
  if (_fit_intercept) {
    Matrix<float> new_x = _prep_train_data(x);
    _A = optimizer->optimize(new_grad, new_x, y); // Returns the most optimal weights.
  } else {
    _A = optimizer->optimize(new_grad, x, y);
  }
  _fit_flag = true;
}
