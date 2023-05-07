#include "regression/Linear.tpp"
#include <gtest/gtest.h>
#include <vector>
#include <iostream>


TEST(LinearTest, BasicAssertions) {
	EXPECT_EQ(_vector_dotproduct(std::vector<float> {10, 10}, std::vector<float> {1, 2}), 30.0f);
	EXPECT_EQ(_vector_dotproduct(std::vector<float> {1.0, 1.0}, std::vector<float> {1, 2}), 3.0f);
	EXPECT_EQ(_vector_dotproduct(std::vector<float> {1.0, 2.0, 10.1, 20.5}, std::vector<float> {1, 2, 3, 4}), 117.3f);
	EXPECT_EQ(_vector_dotproduct(std::vector<float> {1.0, 2.0, 10.1, 20.5}, std::vector<float> {1.0, 14.98, 1123123.2345238, 2.902310918751231}), 11343635.126064214400236f);


}





