#include "double_word/bsc_core.h"
#include <gtest/gtest.h>
#include <iomanip>
// NOTES:
// If the last digit of a binary number is 1, the number is odd; if it's 0, the number is even.
//
// These unit tests are not sufficient enough for a proof of correctness or a proof of error bound.
// The purpose of this file is to ensure that the implementation of certain algorithms are correct
// and working as intended.
// For proofs, refer to the work of Jean-Michel Muller and others in the field.

using basic_core::aint;
using basic_core::fast_two_diff;
using basic_core::fast_two_sum;
using basic_core::nint;
using basic_core::two_prod;
using basic_core::two_sum;

TEST(BasicCore, fast_two_sum) {
  // Testing that the computed result is correctly rounded and that the error is exact.
  double l = 4503599627370496.0;
  double r = 1.0;
  double e;
  auto out = fast_two_sum(l, r, e);
  EXPECT_EQ(e, 0.0);

  fast_two_sum(4503599627370496.0, 10.0, e);
  EXPECT_EQ(e, 0.0);

  fast_two_sum(4503599627370496.0, 1000.0, e);
  EXPECT_EQ(e, 0.0);

  double x = 4503599627370496.0;
  auto out2 = fast_two_sum(x, 0.5, e);
  EXPECT_EQ(out2, 4503599627370496.0); // Round to nearest - tie to even.
  EXPECT_EQ(e, 0.5);

  auto out3 = fast_two_sum(x, 0.6, e);
  // .6 is not exactly representable as a double. Nearest number is 0.59999999999999997779553950749686919152736663818359375.
  EXPECT_EQ(out3, 4503599627370497.0);                                    // Round to nearest - tie to even.
  EXPECT_EQ(e, -0.40000000000000002220446049250313080847263336181640625); // 0.4 cannot be exactly represented in binary.
                                                                          // The err is truncated to the precision of a double.

  auto out4 = fast_two_sum(x, 0.75, e);
  EXPECT_EQ(out4, 4503599627370497.0); // Round to nearest - tie to even.
  EXPECT_EQ(e, -0.25);

  auto out5 = fast_two_sum(x, 0.25, e);
  EXPECT_EQ(out5, 4503599627370496.0); // Round to nearest - tie to even.
  EXPECT_EQ(e, 0.25);

  auto out6 = fast_two_sum(x, 0.31250001490116130486995871251565404236316680908203125, e);
  // 0.31250001490116130486995871251565404236316680908203125 is exactly representable
  EXPECT_EQ(out6, 4503599627370496.0); // Round to nearest - tie to even.
  EXPECT_EQ(e, 0.31250001490116130486995871251565404236316680908203125);
}

TEST(BasicCore, fast_two_diff) {
  // Testing that the computed result is correctly rounded and that the error is exact.
  double l = 4503599627370496.0;
  double r = 1.0;
  double e;
  auto out = fast_two_diff(l, r, e);
  EXPECT_EQ(e, 0.0);

  fast_two_diff(4503599627370496.0, 10.0, e);
  EXPECT_EQ(e, 0.0);

  fast_two_diff(4503599627370496.0, 1000.0, e);
  EXPECT_EQ(e, 0.0);

  double x = 4503599627370496.0;
  auto out2 = fast_two_diff(x, 0.5, e);
  EXPECT_EQ(out2, 4503599627370495.5); // Round to nearest - tie to even.
  EXPECT_EQ(e, 0.0);

  auto out3 = fast_two_diff(x, 0.59999999999999997779553950749686919152736663818359375, e);
  EXPECT_EQ(out3, 4503599627370495.5); // Round to nearest - tie to even.
  EXPECT_EQ(e, -0.09999999999999997779553950749686919152736663818359375);

  auto out4 = fast_two_diff(x, 0.75, e);
  EXPECT_EQ(out4, 4503599627370495.0); // Round to nearest - tie to even.
  EXPECT_EQ(e, 0.25);

  auto out5 = fast_two_diff(x, 0.25, e);
  EXPECT_EQ(out5, 4503599627370496.0); // Round to nearest - tie to even.
  EXPECT_EQ(e, -0.25);

  auto out6 = fast_two_diff(x, 0.31250001490116130486995871251565404236316680908203125, e);
  // 0.31250001490116130486995871251565404236316680908203125 is exactly representable
  EXPECT_EQ(out6, 4503599627370495.5); // Round to nearest - tie to even.
  EXPECT_EQ(e, 0.5 - 0.31250001490116130486995871251565404236316680908203125);
}

TEST(BasicCore, two_sum) {
  // Testing that the computed result is correctly rounded and that the error is exact.
  double l = 4503599627370496.0;
  double r = 1.0;
  double e;
  auto out = fast_two_sum(l, r, e);
  EXPECT_EQ(e, 0.0);

  fast_two_sum(4503599627370496.0, 10.0, e);
  EXPECT_EQ(e, 0.0);

  fast_two_sum(4503599627370496.0, 1000.0, e);
  EXPECT_EQ(e, 0.0);

  double x = 4503599627370496.0;
  auto out2 = fast_two_sum(x, 0.5, e);
  EXPECT_EQ(out2, 4503599627370496.0); // Round to nearest - tie to even.
  EXPECT_EQ(e, 0.5);

  auto out3 = fast_two_sum(x, 0.6, e);
  // .6 is not exactly representable as a double. Nearest number is 0.59999999999999997779553950749686919152736663818359375.
  EXPECT_EQ(out3, 4503599627370497.0);                                    // Round to nearest - tie to even.
  EXPECT_EQ(e, -0.40000000000000002220446049250313080847263336181640625); // 0.4 cannot be exactly represented in binary.
                                                                          // The err is truncated to the precision of a double.

  auto out4 = fast_two_sum(x, 0.75, e);
  EXPECT_EQ(out4, 4503599627370497.0); // Round to nearest - tie to even.
  EXPECT_EQ(e, -0.25);

  auto out5 = fast_two_sum(x, 0.25, e);
  EXPECT_EQ(out5, 4503599627370496.0); // Round to nearest - tie to even.
  EXPECT_EQ(e, 0.25);

  auto out6 = fast_two_sum(x, 0.31250001490116130486995871251565404236316680908203125, e);
  // 0.31250001490116130486995871251565404236316680908203125 is exactly representable
  EXPECT_EQ(out6, 4503599627370496.0); // Round to nearest - tie to even.
  EXPECT_EQ(e, 0.31250001490116130486995871251565404236316680908203125);
}

TEST(BasicCore, two_diff) {
  // Testing that the computed result is correctly rounded and that the error is exact.
  double l = 4503599627370496.0;
  double r = 1.0;
  double e;
  auto out = fast_two_diff(l, r, e);
  EXPECT_EQ(e, 0.0);

  fast_two_diff(4503599627370496.0, 10.0, e);
  EXPECT_EQ(e, 0.0);

  fast_two_diff(4503599627370496.0, 1000.0, e);
  EXPECT_EQ(e, 0.0);

  double x = 4503599627370496.0;
  auto out2 = fast_two_diff(x, 0.5, e);
  EXPECT_EQ(out2, 4503599627370495.5); // Round to nearest - tie to even.
  EXPECT_EQ(e, 0.0);

  auto out3 = fast_two_diff(x, 0.59999999999999997779553950749686919152736663818359375, e);
  EXPECT_EQ(out3, 4503599627370495.5); // Round to nearest - tie to even.
  EXPECT_EQ(e, -0.09999999999999997779553950749686919152736663818359375);

  auto out4 = fast_two_diff(x, 0.75, e);
  EXPECT_EQ(out4, 4503599627370495.0); // Round to nearest - tie to even.
  EXPECT_EQ(e, 0.25);

  auto out5 = fast_two_diff(x, 0.25, e);
  EXPECT_EQ(out5, 4503599627370496.0); // Round to nearest - tie to even.
  EXPECT_EQ(e, -0.25);

  auto out6 = fast_two_diff(x, 0.31250001490116130486995871251565404236316680908203125, e);
  // 0.31250001490116130486995871251565404236316680908203125 is exactly representable
  EXPECT_EQ(out6, 4503599627370495.5); // Round to nearest - tie to even.
  EXPECT_EQ(e, 0.5 - 0.31250001490116130486995871251565404236316680908203125);
}

TEST(BasicCore, two_prod) {
  // Testing that the computed result is correctly rounded and that the error is exact.
  double e;
  two_prod(1.0, 2.0, e);
  EXPECT_EQ(e, 0.0);

  EXPECT_EQ(two_prod(2.0, 2.0, e), 4.0);
  EXPECT_EQ(e, 0.0);

  double out = two_prod(2.3, 2, e); // 2.3 is not exacctly representable. It is rounded to 2.29999999999999982236431605997495353221893310546875
  EXPECT_EQ(out, 4.5999999999999996447286321199499070644378662109375);
  EXPECT_EQ(e, 0.0);

  // Using large numbers makes rounding error more likely because floats are further away from
  // each other.
  double out2 = two_prod(73786905926110806016.0, 73786905926110806016.0, e);
  EXPECT_EQ(out2, 5444507486148725985994448939565027688448.0);
  EXPECT_EQ(e, -2361182959959845896192.0); // 17.16015625 cannot be represented exactly as a double.
}

TEST(BasicCore, two_square) {
  // Testing that the computed result is correctly rounded and that the error is exact.
  double e;
  two_prod(1.0, 2.0, e);
  EXPECT_EQ(e, 0.0);

  EXPECT_EQ(two_prod(2.0, 2.0, e), 4.0);
  EXPECT_EQ(e, 0.0);

  double out = two_prod(2.3, 2, e); // 2.3 is not exacctly representable. It is rounded to 2.29999999999999982236431605997495353221893310546875
  EXPECT_EQ(out, 4.5999999999999996447286321199499070644378662109375);
  EXPECT_EQ(e, 0.0);

  // Using large numbers makes rounding error more likely because floats are further away from
  // each other.
  double out2 = two_prod(73786905926110806016.0, 73786905926110806016.0, e); // These two numbers are exactly representable.
  EXPECT_EQ(out2, 5444507486148725985994448939565027688448.0);
  EXPECT_EQ(e, -2361182959959845896192.0); // 17.16015625 cannot be represented exactly as a double.
}

TEST(BasicCore, nint) {
  EXPECT_EQ(nint(20.4), 20.0);
  EXPECT_EQ(nint(20.1), 20.0);
  EXPECT_EQ(nint(20.5), 21.0);
  EXPECT_EQ(nint(20.8), 21.0);
}

TEST(BasicCore, aint) {
  EXPECT_EQ(aint(20.4), 20.0);
  EXPECT_EQ(aint(20.1), 20.0);
  EXPECT_EQ(aint(20.5), 20.0);
  EXPECT_EQ(aint(20.8), 20.0);

  EXPECT_EQ(aint(80.123), 80.0);
  EXPECT_EQ(aint(80.9430), 80.0);
  EXPECT_EQ(aint(80.556901), 80.0);
  EXPECT_EQ(aint(80.469650), 80.0);
}
