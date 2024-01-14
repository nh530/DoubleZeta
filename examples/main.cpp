#include "optimizer/GradientDescent.h"
#include "regression/Linear.h"
#include "threadpool/ZetaSession.h"
#include <iostream>

int main() {
  LinearModel lr = LinearModel();
  Matrix<float> obs{3, 1, 1};
  obs.set_value(0, 0, 10);
  obs.set_value(1, 0, 20);
  obs.set_value(2, 0, 30);
  Matrix<float> left1 = Matrix<float>(4, 4, 1);
  Matrix<float> right1 = Matrix<float>(4, 4, 1);
  right1.set_value(0, 0, 0.43f);
  right1.set_value(0, 3, 4.91f);
  right1.set_value(1, 3, 2.1f);
  right1.set_value(3, 3, 900.2f);
  left1.set_value(0, 0, 10);
  left1.set_value(1,1, 20);
  left1.set_value(1,3, 0.123123f);
  std::cout << lr.predict(obs) << '\n';
  Matrix<float> out = lr._optimize_loss();
  for (int i = 0; i < out.shape[0]; i++) {
    for (int j = 0; j < out.shape[1]; j++) {
      std::cout << out[i][j] << '\n';
    }
  }
}
