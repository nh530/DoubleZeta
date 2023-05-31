#include "optimizer/GradientDescent.h"
#include "typing/DTypes.h"
#include "typing/Matrix.h"

#ifndef REGRESSION_H
#define REGRESSION_H

class ModelBase {

public:
  virtual Matrix gradient(Matrix &weights, Matrix &x, Matrix &y) = 0;
  virtual void fit(Matrix &x, Matrix &y) = 0;
  virtual Matrix predict(Matrix &x) = 0;
  virtual float predict(float x) = 0;
  virtual void print() = 0;
  virtual float score() = 0;
  virtual std::vector<float> get_parameters() = 0;
  virtual float get_intercept() = 0;

private:
  virtual void _verify_model(Matrix &x, Matrix &y) = 0;
};

class LogisticRegression {
  Matrix _A; // Matrix of weights to optimize over.
  Matrix _b; // matrix of intercepts
public:
  Matrix gradient();
  void _val_model();
  float score();
};

class LinearRegression : public virtual ModelBase {
  Matrix _A; // Matrix of weights to optimize over. Intercept is included in this.

public:
  LinearRegression(float step_size, float tolerance, bool fit_intercept = false);
  Matrix gradient(Matrix &weights, Matrix &x, Matrix &y) override; // The gradient functional form depends on the objective function and the
                                                                   // type of model that is used for predictions.
  float score() override;
  Matrix predict(Matrix &x) override;
  float predict(float x) override;
  void fit(Matrix &x, Matrix &y) override;
  float get_intercept() override;
  std::vector<float> get_parameters() override;
  void print() override;

private:
  void _verify_model(Matrix &x, Matrix &y) override; // validate the equation. Ensure it is correct. Validate assumptions.
  bool _fit_intercept;
  Matrix _prep_train_data(Matrix &x); // Pad 1's to the input vector x for the intercept weight.
  bool _fit_flag;
  Matrix y_obs;
  Matrix x_obs;
  LossF *objective;
  GradientDescent *optimizer;
};

#endif
