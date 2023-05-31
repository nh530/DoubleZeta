#include "regression/Linear.h"
#include <gtest/gtest.h>
#include <iostream>
#include <stdexcept>
#include <vector>

TEST(LinearTest, predict) {
  LinearModel lr = LinearModel();
  Matrix obs{3, 1};
  obs[0][0] = 10;
  obs[1][0] = 20;
  obs[2][0] = 30;
  float pred = lr.predict(obs);
  float actl = 10;
  //EXPECT_EQ(actl, pred);
}

TEST(LinearTest, _calculate_loss) {
	LinearModel lr = LinearModel();
	std::vector<float> test1 {0.0, 1.1, 2.3123123};
	//EXPECT_EQ(10.0f, lr._calculate_loss(test1));
}
