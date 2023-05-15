#include "regression/Linear.h"
#include "threadpool/ZetaSession.h"
#include <iostream>

int main() {
  // LinearRegression lr = LinearRegression();
  Matrix obs{3, 1};
  obs[0][0] = 10;
  obs[1][0] = 20;
  obs[2][0] = 30;

  Matrix left1 = Matrix(4, 4);
  Matrix right1 = Matrix(4, 4);
  std::cout << (left1 + right1)[0][0] << Matrix(4, 4)[0][0] << '\n';
  right1[0][0] = 0.43f;
  right1[0][3] = 4.91f;
  right1[1][3] = 2.1f;
  right1[3][3] = 900.2f;
  left1[0][0] = 10;
  left1[1][1] = 20;
  left1[1][3] = 0.123123f;
  Matrix actual = Matrix(4, 4);
  actual[0][0] = 10.43f;
  actual[0][3] = 4.91f;
  actual[1][1] = 20;
  actual[1][3] = 2.223123f;
  actual[3][3] = 900.2f;
  std::cout << (left1 + right1)[1][1] << actual[1][1] << '\n';
  std::vector<float> test(left1[0], left1[0] + 4);
  std::cout << test[0] << '\n';
	std::cout << left1.shape[0] << '\n';

  // std::cout << lr.predict(obs) << '\n';
}
