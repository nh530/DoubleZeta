#include "typing/DTypes.h"
#include "typing/Matrix.h"
#include <functional>
#include <variant>
#include <vector>

#ifndef GRADIENTDESCENT_H
#define GRADIENTDESCENT_H

class LossF {
public:
  virtual float objective_func(std::vector<float> &y_pred, std::vector<float> &y_act) = 0;
  virtual float objective_func(Matrix &y_pred, Matrix &y_act) = 0;
};

class MeanSquaredError : public LossF {
public:
  MeanSquaredError();
  float objective_func(std::vector<float> &y_pred, std::vector<float> &y_act);
  float objective_func(Matrix &y_pred, Matrix &y_act);
};

class BinaryCrossEntropy : public LossF {
public:
  BinaryCrossEntropy();
  float objective_func(std::vector<float> &y_pred, std::vector<float> &y_act);
};

class GradientDescent {
  float _step; // size of step to take.
  float _tol;  // size of gradient vectob.

public:
  GradientDescent(float, float);
  Matrix optimize(std::function<Matrix(Matrix &, Matrix &, Matrix &)>, Matrix &x, Matrix &y); // Gradient function with respect to the input weights vector x.
private:
  Matrix _set_initial(int rows, int cols, int seed = 100);
};

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
