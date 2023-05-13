#ifndef LINEAR_H
#define LINEAR_H

#include "typing/DTypes.h"
#include "typing/Matrix.h"

class LinearRegression {
public:
  LinearRegression();
  float predict(Matrix);
	float _calculate_loss(std::vector<float>);

private:
};

#endif
