#include "optimizer/GradientDescent.h"
#include "typing/DTypes.h"
#include "typing/Matrix.h"

#ifndef REGRESSION_H
#define REGRESSION_H

class ModelBase {

public:
  virtual Matrix<float> gradient(Matrix<float> &weights, Matrix<float> &x, Matrix<float> &y) = 0;
  virtual void fit(Matrix<float> &x, Matrix<float> &y) = 0;
  virtual Matrix<float> predict(Matrix<float> &x) = 0;
  virtual float predict(float x) = 0;
  virtual void print() = 0;
  virtual float score() = 0;
  virtual std::vector<float> get_parameters() = 0;
  virtual float get_intercept() = 0;

private:
  virtual void _verify_model(Matrix<float> &x, Matrix<float> &y) = 0;
};

class LogisticRegression {
  Matrix<float> _A; // Matrix of weights to optimize over.
  Matrix<float> _b; // matrix of intercepts
public:
  Matrix<float> gradient();
  void _val_model();
  float score();
};

class LinearRegression : public virtual ModelBase {
  Matrix<float> _A; // Matrix of weights to optimize over. Intercept is included in this.

public:
  LinearRegression(float step_size, float tolerance, bool fit_intercept = false);
  Matrix<float> gradient(Matrix<float> &weights, Matrix<float> &x, Matrix<float> &y) override; // The gradient functional form depends on the objective function and the
                                                                   // type of model that is used for predictions.
  float score() override;
  Matrix<float> predict(Matrix<float> &x) override;
  float predict(float x) override;
  void fit(Matrix<float> &x, Matrix<float> &y) override;
  float get_intercept() override;
  std::vector<float> get_parameters() override;
  void print() override;

private:
  void _verify_model(Matrix<float> &x, Matrix<float> &y) override; // validate the equation. Ensure it is correct. Validate assumptions.
  bool _fit_intercept;
  Matrix<float> _prep_train_data(Matrix<float> &x); // Pad 1's to the input vector x for the intercept weight.
  bool _fit_flag;
  Matrix<float> y_obs;
  Matrix<float> x_obs;
  LossF *objective;
  GradientDescent *optimizer;
};

#endif
