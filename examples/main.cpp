#include "optimizer/GradientDescent.h"
#include "regression/Linear.h"
#include "threadpool/ZetaSession.h"
#include <iostream>

int main() {
  LinearModel lr = LinearModel();
  Matrix obs{3, 1, 1};
  obs[0][0] = 10;
  obs[1][0] = 20;
  obs[2][0] = 30;
  Matrix left1 = Matrix(4, 4, 1);
  Matrix right1 = Matrix(4, 4, 1);
  right1[0][0] = 0.43f;
  right1[0][3] = 4.91f;
  right1[1][3] = 2.1f;
  right1[3][3] = 900.2f;
  left1[0][0] = 10;
  left1[1][1] = 20;
  left1[1][3] = 0.123123f;
  std::cout << lr.predict(obs) << '\n';
  Matrix out = lr._optimize_loss();
  for (int i = 0; i < out.shape[0]; i++) {
    for (int j = 0; j < out.shape[1]; j++) {
      std::cout << out[i][j] << '\n';
    }
  }
}
