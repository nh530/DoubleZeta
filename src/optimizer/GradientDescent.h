#ifndef GRADIENTDESCENT_H
#define GRADIENTDESCENT_H
#include "typing/DTypes.h"
#include "typing/Matrix.h"
#include <functional>
#include <gtest/gtest.h>
#include <variant>
#include <vector>

class LossF {
public:
  virtual float objective_func(std::vector<float> &y_pred, std::vector<float> &y_act) = 0;
  virtual float objective_func(Matrix<float> &y_pred, Matrix<float> &y_act) = 0;
};

class MeanSquaredError : public LossF {
public:
  MeanSquaredError();
  float objective_func(std::vector<float> &y_pred, std::vector<float> &y_act) override;
  float objective_func(Matrix<float> &y_pred, Matrix<float> &y_act) override;
};

class BinaryCrossEntropy : public LossF {
public:
  BinaryCrossEntropy();
  float objective_func(std::vector<float> &y_pred, std::vector<float> &y_act) override;
  float objective_func(Matrix<float> &y_pred, Matrix<float> &y_act) override;
};

class GradientDescent {
  float _step; // size of step to take.
  float _tol;  // size of gradient vectob.

public:
  GradientDescent(float, float);
  Matrix<float> optimize(std::function<Matrix<float>(Matrix<float> &, Matrix<float> &, Matrix<float> &)>, Matrix<float> &x, Matrix<float> &y,
                         const Matrix<float> &init_weights = {1, 1, 0});
  // Gradient function with respect to the input weights vector x.
  // A 1 by 1 matrix of 0 is used as the sentinel value to signal the algorithm to generate its own initial weight.
private:
  Matrix<float> _set_initial(int rows, int cols, int seed = 100);
  FRIEND_TEST(GradientDescentTest, set_initial); // For google unit-test framework.
};

// TODO: Declare stochastic gradient descent, Momentum gradient descent, and any other gradient deescent variations

#endif
