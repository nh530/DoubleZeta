#include <any>
#include <gtest/gtest.h>
#include <iostream>



TEST(HelloTest, BasicAssertions) {
  EXPECT_STRNE("HELLO", "WORLD");
  EXPECT_EQ(42, 42);
}
