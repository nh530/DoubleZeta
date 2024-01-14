#include "optimizer/GradientDescent.h"
#include "typing/DTypes.h"
#include "typing/Matrix.h"

#ifndef LINEAR_H
#define LINEAR_H
// deprecated

class LinearModel {
  Matrix<float> _A;
  Matrix<float> _b;
  float _step_size;
  float _tolerance;

public:
  LinearModel(float step_size = 0.1f, float tolerance = 0.001f);
  float predict(Matrix<float> &);
  float _calculate_loss(Matrix<float> &);
  Matrix<float> _optimize_loss();

private:
  float _calculate_gradient(Matrix<float> &);
  Matrix<float> _set_initial(); // Set gradient descent initial point.
};

#endif
