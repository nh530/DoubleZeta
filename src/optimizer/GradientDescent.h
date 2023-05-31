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
  Matrix optimize(std::function<Matrix(Matrix &, Matrix &, Matrix &)>, Matrix &x,
                  Matrix &y); // Gradient function with respect to the input weights vector x.
private:
  Matrix _set_initial(int rows, int cols, int seed = 100);
};

// TODO: Declare stochastic gradient descent, Momentum gradient descent, and any other gradient deescent variations

#endif
