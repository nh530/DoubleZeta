#ifndef LINEAR_H
#define LINEAR_H

#include "typing/DTypes.h"
#include "typing/Matrix.h"

class LinearRegression {
  Matrix _A;
  Matrix _b;
  float _step_size=0.1f;
  float _tolerance=0.001f;

public:
  LinearRegression(float, float);
  float predict(Matrix &);
  float _calculate_loss(Matrix &);
  Matrix _optimize_loss();

private:
  float _calculate_gradient(Matrix &);
	Matrix _set_initial(); // Set gradient descent initial point.
};

#endif
