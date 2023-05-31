#ifndef LINEAR_H
#define LINEAR_H

#include "typing/DTypes.h"
#include "typing/Matrix.h"

// TODO: Is there a modifiable hashmap in cpp?
struct params {};

class TempModel {
public:
  virtual params get_params() = 0;
  virtual void _validate_data() = 0;
  virtual float get_score() = 0;
  virtual void fit() = 0;
  virtual float predict(Matrix &) = 0;
};

class LinearModel {
  Matrix _A;
  Matrix _b;
  float _step_size;
  float _tolerance;

public:
  LinearModel(float step_size = 0.1f, float tolerance = 0.001f);
  float predict(Matrix &);
  float _calculate_loss(Matrix &);
  Matrix _optimize_loss();

private:
  float _calculate_gradient(Matrix &);
  Matrix _set_initial(); // Set gradient descent initial point.
};

#endif
