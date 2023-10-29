#include "double_word/dw.h"
#include "double_word/fpu_fix.h"
#include <gtest/gtest.h>
#include <iomanip>
#include <limits>
#include <utility>

TEST(DoubleWord, hi_lo) {
  auto test = dbl_wrd(10);
  EXPECT_EQ(test.hi(), 10.0);
  EXPECT_EQ(test.lo(), 0.0);

  auto test2 = dbl_wrd(90.0);
  EXPECT_EQ(test2.hi(), 90.0);
  EXPECT_EQ(test2.lo(), 0.0);

  auto test3 = dbl_wrd(102.9123, 8.24);
  EXPECT_EQ(test3.hi(), 102.9123);
  EXPECT_EQ(test3.lo(), 8.24);
}

TEST(DoubleWord, isnan) {
  auto test = dbl_wrd(std::numeric_limits<double>::quiet_NaN());
  EXPECT_TRUE(test.isnan());
  auto test2 = dbl_wrd(std::numeric_limits<double>::quiet_NaN(), std::numeric_limits<double>::quiet_NaN());
  EXPECT_TRUE(test2.isnan());
  auto test3 = dbl_wrd(10.0, std::numeric_limits<double>::quiet_NaN());
  EXPECT_TRUE(test3.isnan());
  auto test4 = dbl_wrd(54, std::numeric_limits<double>::quiet_NaN());
  EXPECT_TRUE(test3.isnan());
}

TEST(DoubleWord, isinf) {
  auto test = dbl_wrd(std::numeric_limits<double>::infinity());
  EXPECT_TRUE(test.isinf());
  auto test2 = dbl_wrd(std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity());
  EXPECT_TRUE(test2.isinf());
  auto test3 = dbl_wrd(std::numeric_limits<double>::infinity(), 10.0);
  EXPECT_TRUE(test3.isinf());
}

TEST(DoubleWord, isfinite) {
  auto test = dbl_wrd(0.1);
  EXPECT_TRUE(test.isfinite());
  auto test2 = dbl_wrd(std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity());
  EXPECT_FALSE(test2.isfinite());
  auto test3 = dbl_wrd(std::numeric_limits<double>::infinity(), 10.0);
  EXPECT_FALSE(test3.isfinite());
}

TEST(DoubleWord, square) {
  auto out = dbl_wrd::square(10.0);
  EXPECT_EQ(out.hi(), 100.0);
  EXPECT_EQ(out.lo(), 0.0);

  auto out2 = dbl_wrd::square(2.5);
  EXPECT_EQ(out2.hi(), 6.25);
  EXPECT_EQ(out2.lo(), 0.0);

  auto out3 = dbl_wrd::square(73786905926110806016.0);
  EXPECT_EQ(out3.hi(), 5444507486148725985994448939565027688448.0);
  EXPECT_EQ(out3.lo(), -2361182959959845896192.0);
}

TEST(DoubleWord, sqrt) {
  auto out = dbl_wrd::sqrt(4.0);
  EXPECT_EQ(out.hi(), 2.0);
  EXPECT_EQ(out.lo(), 0.0);

  auto out2 = dbl_wrd::sqrt(6.25);
  EXPECT_EQ(out2.hi(), 2.5);
  EXPECT_EQ(out2.lo(), 0.0);

  // square root is very tricky to test. It is also not correctly rounded and the results are not exact. It is an approximation.
  auto out3 = dbl_wrd::sqrt(8.0); // This number is not a perfect square, but it is exactly representable.
  EXPECT_EQ(out3.hi(), 2.828427124746190290949243717477656900882720947265625);
  // EXPECT_EQ(out3.lo(), -0.00000000000000019334586626905826902109399459533895300259633410859538837911486552911810576915740966796875);
  //  TODO: Current value output: -1.93345866269058318324900570908576791235631664282734742954517059843055903911590576171875e-16, which seems wrong.
}

TEST(DoubleWord, assign_op) {
  auto out = dbl_wrd(0, 30);
  out = 10.0;
  EXPECT_EQ(out.hi(), 10.0);
  EXPECT_EQ(out.lo(), 0.0);
}

TEST(DoubleWord, equality_op) {
  auto out = dbl_wrd(10, 1);

  EXPECT_FALSE(out == 10.0);
  EXPECT_FALSE(out == 1.0);
  EXPECT_FALSE(10.0 == out);
  EXPECT_FALSE(2.0 == out);

  auto out2 = dbl_wrd(10, 0);
  EXPECT_TRUE(out2 == 10.0);
  EXPECT_TRUE(10.0 == out2);

  EXPECT_FALSE(out == out2);
  auto right = dbl_wrd(10, 1);
  EXPECT_TRUE(out == right);
  EXPECT_FALSE(out2 == right);
}

TEST(DouleWord, greater_op) {
  auto out = dbl_wrd(10, 1);

  EXPECT_TRUE(out > 10.0);
  EXPECT_FALSE(out > 11.0);

  EXPECT_TRUE(out >= 10.0);
  EXPECT_FALSE(10.0 >= out);
  EXPECT_TRUE(out >= 11.0);
  EXPECT_TRUE(11.0 >= out);
  EXPECT_FALSE(out >= 12.0);
  EXPECT_TRUE(12.0 >= out);

  auto out2 = dbl_wrd(11, 0);
  EXPECT_FALSE(out2 > out);
  EXPECT_FALSE(out > out2);
  EXPECT_TRUE(out2 >= out);
  EXPECT_TRUE(out >= out2);

  auto out3 = dbl_wrd(10, 2);
  EXPECT_TRUE(out3 > out);
  EXPECT_FALSE(out > out3);
  EXPECT_TRUE(out3 >= out);
  EXPECT_FALSE(out >= out3);
}

TEST(DoubleWord, less_op) {
  auto out = dbl_wrd(10, 1);

  EXPECT_TRUE(10.0 < out);
  EXPECT_FALSE(11.0 < out);

  EXPECT_TRUE(10.0 <= out);
  EXPECT_FALSE(out <= 10.0);
  EXPECT_TRUE(11.0 <= out);
  EXPECT_TRUE(out <= 11.0);
  EXPECT_FALSE(12.0 <= out);
  EXPECT_TRUE(out <= 12.0);

  auto out2 = dbl_wrd(11, 0);
  EXPECT_FALSE(out < out2);
  EXPECT_FALSE(out2 < out);
  EXPECT_TRUE(out <= out2);
  EXPECT_TRUE(out2 <= out);

  auto out3 = dbl_wrd(10, 2);
  EXPECT_TRUE(out < out3);
  EXPECT_FALSE(out3 < out);
  EXPECT_TRUE(out <= out3);
  EXPECT_FALSE(out3 <= out);
}

TEST(DoubleWord, neq_op) {
  auto out = dbl_wrd(10, 1);
  EXPECT_TRUE(out != 10.0);
  EXPECT_TRUE(out != 1.0);
  EXPECT_TRUE(10.0 != out);
  EXPECT_TRUE(2.0 != out);

  auto out2 = dbl_wrd(10, 0);
  EXPECT_FALSE(out2 != 10.0);
  EXPECT_FALSE(10.0 != out2);
  EXPECT_TRUE(out != out2);

  auto right = dbl_wrd(10, 1);
  EXPECT_FALSE(out != right);
  EXPECT_TRUE(out2 != right);
}

TEST(DoubleWord, is_neg) {
  auto out = dbl_wrd(-10, 0);

  EXPECT_TRUE(out.is_negative());
  EXPECT_FALSE((-1 * out).is_negative());
}

TEST(DoubleWord, is_pos) {
  auto out = dbl_wrd(10, 0);

  EXPECT_TRUE(out.is_positive());
  EXPECT_FALSE((-1 * out).is_positive());
}

TEST(DoubleWord, is_zero) {
  auto out = dbl_wrd(0, 0);

  EXPECT_TRUE(out.is_zero());
}

TEST(DoubleWord, is_one) {
  auto out = dbl_wrd(1, 0);

  EXPECT_TRUE(out.is_one());
}

TEST(DoubleWord, abs) {
  auto out = dbl_wrd(-1.0, 0.0);

  EXPECT_EQ(abs(out).hi(), 1.0);
  EXPECT_EQ(fabs(out).hi(), 1.0);
  auto out2 = dbl_wrd(10.0, 0.0);
  EXPECT_EQ(abs(out2).hi(), 10.0);
  EXPECT_EQ(fabs(out2).hi(), 10.0);
}

TEST(DoubleWord, nint) {
  // Dependency on add method being correct and returning
  auto test1 = nint(dbl_wrd().add(10.0, 1.123));
  EXPECT_EQ(test1.hi(), 11.0);
  EXPECT_EQ(test1.lo(), 0.0);

  auto test2 = nint(dbl_wrd().add(1.123123, .5));
  EXPECT_EQ(test2.hi(), 2.0);
  EXPECT_EQ(test2.lo(), 0.0);

  auto test3 = nint(dbl_wrd().add(10.0, .5));
  EXPECT_EQ(test3.hi(), 11.0);
  EXPECT_EQ(test3.lo(), 0.0);
}

TEST(DoubleWord, floor) {
  auto test1 = floor(dbl_wrd().add(10.0, 1.123));
  EXPECT_EQ(test1, 11.0);

  auto test2 = floor(dbl_wrd().add(1.123123, .5));
  EXPECT_EQ(test2.hi(), 1.0);
  EXPECT_EQ(test2.lo(), 0.0);

  auto test3 = floor(dbl_wrd().add(10.0, .5));
  EXPECT_EQ(test3.hi(), 10.0);
  EXPECT_EQ(test3.lo(), 0.0);
}

TEST(DoubleWord, ceil) {
  auto test1 = ceil(dbl_wrd().add(10.0, 1.123));
  EXPECT_EQ(test1.hi(), 12.0);

  auto test2 = ceil(dbl_wrd().add(1.123123, .5));
  EXPECT_EQ(test2.hi(), 2.0);
  EXPECT_EQ(test2.lo(), 0.0);

  auto test3 = ceil(dbl_wrd().add(10.0, .5));
  EXPECT_EQ(test3.hi(), 11.0);
  EXPECT_EQ(test3.lo(), 0.0);
}

TEST(DoubleWord, aint) {
  auto test1 = aint(dbl_wrd().add(1.12, .1));
  EXPECT_EQ(test1.hi(), 1.0);
  EXPECT_EQ(test1.lo(), 0.0);

  auto test2 = aint(dbl_wrd().add(-1.01, -.1));
  EXPECT_EQ(test2.hi(), -1.0);
  EXPECT_EQ(test2.lo(), 0.0);
}

TEST(DoubleWord, to_double) {
  auto test1 = to_double(dbl_wrd().add(1.0, .1));
  EXPECT_EQ(test1, 1.1);

  auto test2 = to_double(dbl_wrd(5.0, 1));
  EXPECT_EQ(test2, 5.0);

  auto test3 = to_double(dbl_wrd(-10.1, 1));
  EXPECT_EQ(test3, -10.1);
}

TEST(DoubleWord, to_int) {
  auto test1 = to_int(dbl_wrd(10.0, .1));
  EXPECT_EQ(test1, 10);

  auto test2 = to_int(dbl_wrd(10.1, .1));
  EXPECT_EQ(test2, 10);

  auto test3 = to_int(dbl_wrd(-10.1, .1));
  EXPECT_EQ(test3, -10);
}

TEST(DoubleWord, nroot) {
  auto test1 = nroot(dbl_wrd(4.0, 0.0), 2);
  EXPECT_EQ(test1.hi(), 2.0);

  auto test2 = nroot(dbl_wrd(9.0, 0.0), 2);
  EXPECT_EQ(test2.hi(), 3.0);

  auto test3 = nroot(dbl_wrd(100.0, 0.0), 2);
  EXPECT_EQ(test3.hi(), 10.0);

  // TODO: There might be something wrong with this func.
  // auto test4 = nroot(dbl_wrd(4.0, 0.1), 2);
  // EXPECT_EQ(test4.hi(), 2.0);
  // print(test4);
}

TEST(DoubleWord, npwr) {
  auto test1 = npwr(dbl_wrd(4.0, 0.0), 2);
  EXPECT_EQ(test1.hi(), 16.0);
  EXPECT_EQ(test1.lo(), 0.0);

  auto test2 = npwr(dbl_wrd(2.0, 0.0), 5);
  EXPECT_EQ(test2.hi(), 32.0);
  EXPECT_EQ(test2.lo(), 0.0);

  auto test3 = npwr(dbl_wrd(73786905926110806016.0), 2); // Exactly representable.
  EXPECT_EQ(test3.hi(), 5444507486148725985994448939565027688448.0);
  EXPECT_EQ(test3.lo(), -2361182959959845896192.0);

  auto test4 = npwr(dbl_wrd(73786905926110806016.0), 3); // Exactly representable.
  EXPECT_EQ(test4.hi(), 401733361694462077253844475810836242429334549722284007882752.0);
  EXPECT_EQ(test4.lo(), -871122547779850375498466981448742026084352.0); // There is rounding in this number. It is not exactly representable.

  // NOTES:
  // 24758729950294804892630403839.9963378909160383045673370361328125 is not representable; closest is 24758729950294804946317475840.0
  // $5*2^-106 * 401733361694462076382721928030985866935589934756411627012096 = 24758729950294804892630403839.9963378909160383045673370361328125$
  // 401733361694462076382721928030985866935589934756411627012096  // Exact value; Note the rounding error occurred twice because multiplication
  // happened twice.
  // 401733361694462077253844475810836242429334549722284007882752 // Double precision
}

TEST(DoubleWord, pow) {
  auto test1 = pow(dbl_wrd(4.0, 0.0), 2);
  EXPECT_EQ(test1.hi(), 16.0);
  EXPECT_EQ(test1.lo(), 0.0);

  auto test2 = pow(dbl_wrd(2.0, 0.0), 5);
  EXPECT_EQ(test2.hi(), 32.0);
  EXPECT_EQ(test2.lo(), 0.0);

  auto test3 = pow(dbl_wrd(73786905926110806016.0), 2); // Exactly representable.
  EXPECT_EQ(test3.hi(), 5444507486148725985994448939565027688448.0);
  EXPECT_EQ(test3.lo(), -2361182959959845896192.0);

  auto test4 = pow(dbl_wrd(73786905926110806016.0), 3); // Exactly representable.
  EXPECT_EQ(test4.hi(), 401733361694462077253844475810836242429334549722284007882752.0);
  EXPECT_EQ(test4.lo(), -871122547779850375498466981448742026084352.0); // There is rounding in this number. It is not exactly representable.

  // NOTES:
  // 24758729950294804892630403839.9963378909160383045673370361328125 is not representable; closest is 24758729950294804946317475840.0
  // $5*2^-106 * 401733361694462076382721928030985866935589934756411627012096 = 24758729950294804892630403839.9963378909160383045673370361328125$
  // 401733361694462076382721928030985866935589934756411627012096  // Exact value; Note the rounding error occurred twice because multiplication
  // happened twice.
  // 401733361694462077253844475810836242429334549722284007882752 // Double precision
}

TEST(DoubleWord, log) {
  auto test1 = log(dbl_wrd(1.0, 0.0));
  EXPECT_EQ(test1.hi(), 0.0);
  EXPECT_EQ(test1.lo(), 0.0);

  auto test2 = log(dbl_wrd(1.0, 0.0));
  EXPECT_EQ(test2.hi(), 0.0);
  EXPECT_EQ(test2.lo(), 0.0);
}

TEST(DoubleWord, log10) {
  auto test1 = dbl_wrd(4.0, 0.0);
  log10(test1);
}

TEST(DoubleWord, fmod) {
  auto test1 = fmod(dbl_wrd(4.0, 0.0), 3);
  EXPECT_EQ(test1.hi(), 1.0);
  EXPECT_EQ(test1.lo(), 0.0);

  auto test2 = fmod(dbl_wrd(4.0999999999999996447286321199499070644378662109375,
                            0.0000000000000003552713678800501126570847643143576352932141320696557418301608777255751192569732666015625),
                    3.0); // Closest representation to 4.1.
  EXPECT_EQ(test2.hi(), 1.100000000000000088817841970012523233890533447265625);
  EXPECT_EQ(test2.lo(), -0.0000000000000000888178419700125035123679029219704897067858679303442581698391222744248807430267333984375);
}

TEST(DoubleWord, get_digits) {
  auto test1 = dbl_wrd(4.0, 0);
  EXPECT_NO_THROW(test1.get_digits());
  EXPECT_GE(500, test1.get_digits()); // Just a sanity check. Does not mean anything
}

TEST(DoubleWord, get_hi) {
  auto test1 = dbl_wrd(4.0, 0.0);
  EXPECT_EQ(test1.get_hi(), 4.0);

  auto test2 = dbl_wrd(10.0999999999999996447286321199499070644378662109375, 0.0000000000000003552713678800500929355621337890625);
  EXPECT_EQ(test2.get_hi(), 10.0999999999999996447286321199499070644378662109375);
}

TEST(DoubleWord, get_lo) {
  auto test1 = dbl_wrd(4.0, 0.0);
  EXPECT_EQ(test1.get_lo(), 0.0);

  auto test2 = dbl_wrd(10.0999999999999996447286321199499070644378662109375, 0.0000000000000003552713678800500929355621337890625);
  EXPECT_EQ(test2.get_lo(), 0.0000000000000003552713678800500929355621337890625);
}

TEST(DoubleWord, ddrand) {
  auto test1 = ddrand(-10000, 100000);
  EXPECT_TRUE(test1.hi() < 100000.0);
  EXPECT_TRUE(test1.hi() > -10000.0);
}

TEST(DoubleWord, rand) {
  auto test1 = dbl_wrd::rand(-10000, 100000);
  EXPECT_TRUE(test1.hi() < 100000.0);
  EXPECT_TRUE(test1.hi() > -10000.0);
}

TEST(DoubleWord, ldexp) {
  unsigned int old_cw;
  fpu_fix_start(&old_cw);

  auto test1 = ldexp(dbl_wrd(1), 2);
  EXPECT_EQ(test1.hi(), 4);
  EXPECT_EQ(test1.lo(), 0);
  auto test2 = ldexp(dbl_wrd(1, .1), 1);
  EXPECT_EQ(test2.hi(), 2);
  EXPECT_EQ(test2.lo(), 0.200000000000000011102230246251565404236316680908203125);

  fpu_fix_end(&old_cw);
}

TEST(DoubleWord, sqr) {
  unsigned int old_cw;
  fpu_fix_start(&old_cw);

  auto test1 = sqr(dbl_wrd(4));
  EXPECT_EQ(test1.hi(), 16);
  EXPECT_EQ(test1.lo(), 0);

  auto test2 = sqr(dbl_wrd(10));
  EXPECT_EQ(test2.hi(), 100);
  EXPECT_EQ(test2.lo(), 0);

  fpu_fix_end(&old_cw);
}

TEST(DoubleWord, divrem) {
  unsigned int old_cw;
  fpu_fix_start(&old_cw);

  dbl_wrd rem;
  auto test1 = divrem(dbl_wrd(10), dbl_wrd(2), rem);
  EXPECT_EQ(test1.hi(), 5);
  EXPECT_EQ(test1.lo(), 0);
  EXPECT_EQ(rem.hi(), 0);
  EXPECT_EQ(rem.lo(), 0);

  auto test2 = divrem(dbl_wrd(9), dbl_wrd(2), rem);
  EXPECT_EQ(test2.hi(), 4);
  EXPECT_EQ(test2.lo(), 0);
  EXPECT_EQ(rem.hi(), 1);
  EXPECT_EQ(rem.lo(), 0);

  fpu_fix_end(&old_cw);
}

TEST(DoubleWord, drem) {
  auto test1 = drem(dbl_wrd(10), dbl_wrd(2));
  EXPECT_EQ(test1.hi(), 0);
  EXPECT_EQ(test1.lo(), 0);

  auto test2 = drem(dbl_wrd(9), dbl_wrd(2));
  EXPECT_EQ(test2.hi(), 1);
  EXPECT_EQ(test2.lo(), 0);
}

TEST(DoubleWord, mul_pwr2) {
  unsigned int old_cw;
  fpu_fix_start(&old_cw);

  auto test1 = mul_pwr2(dbl_wrd(10), 2);
  EXPECT_EQ(test1.hi(), 20);
  EXPECT_EQ(test1.lo(), 0.0);

  auto test2 = mul_pwr2(dbl_wrd(9), 8);
  EXPECT_EQ(test2.hi(), 72);
  EXPECT_EQ(test2.lo(), 0.0);

  fpu_fix_end(&old_cw);
}

TEST(DoubleWord, get_eps) {
  auto test1 = dbl_wrd::get_eps();
  // Only verifying that it runs and returns.
}

TEST(DoubleWord, get_max) {
  auto test1 = dbl_wrd::get_max();
  // Only verifying that it runs and returns.
}

TEST(DoubleWord, get_min) {
  auto test1 = dbl_wrd::get_min();
  // Only verifying that it runs and returns.
}

TEST(DoubleWord, accurate_add) {
  unsigned int old_cw;
  fpu_fix_start(&old_cw);

  auto test1 = dbl_wrd::accurate_add(dbl_wrd(1), dbl_wrd(.1));
  EXPECT_EQ(test1.hi(), 1.100000000000000088817841970012523233890533447265625);
  EXPECT_EQ(test1.lo(), -0.0000000000000000832667268468867405317723751068115234375);

  // 9007199254740991/2^54 = 0.499999999999999944488848768742172978818416595458984375
  // 9007199254740987.0/2.0 = 4503599627370493.5
  // 9007199254740991/2^56 = 0.12499999999999998612221219218554324470460414886474609375
  // actual = 4503599627370496.87500000000000006938893903907228377647697925567626953125
  auto test2 = dbl_wrd::accurate_add(dbl_wrd(9007199254740991.0, -0.499999999999999944488848768742172978818416595458984375),
                                     dbl_wrd(-4503599627370493.5, -0.12499999999999998612221219218554324470460414886474609375));
  EXPECT_EQ(test2.hi(), 4503599627370497);
  EXPECT_EQ(test2.lo(), -0.125000000000000055511151231257827021181583404541015625);
  fpu_fix_end(&old_cw);
}

TEST(DoubleWord, sloppy_add) {
  unsigned int old_cw;
  fpu_fix_start(&old_cw);

  auto test1 = dbl_wrd::sloppy_add(dbl_wrd(1), dbl_wrd(.1));
  EXPECT_EQ(test1.hi(), 1.100000000000000088817841970012523233890533447265625);
  EXPECT_EQ(test1.lo(), -0.0000000000000000832667268468867405317723751068115234375);

  auto test2 = dbl_wrd::sloppy_add(dbl_wrd(9007199254740991, -0.499999999999999944488848768742172978818416595458984375),
                                   dbl_wrd(-4503599627370493.5, -0.12499999999999998612221219218554324470460414886474609375));
  // This is a special case where the error is better than accurate_add, which is very odd because these do not have the same sign.
  EXPECT_EQ(test2.hi(), 4503599627370497);
  EXPECT_EQ(test2.lo(), -0.125);

  // actual value = 2^-106. sloppy_add returned 0. The relative error here is about 100%, which demonstrates the issue with
  // using this routine with inputs of opposite signs.
  auto test3 = dbl_wrd::sloppy_add(dbl_wrd(1.0 + pow(2, -50), -pow(2, -53)), dbl_wrd(-1.0 - 6 * pow(2, -53), -pow(2, -53) + pow(2, -106)));
  EXPECT_EQ(test3.hi(), 0);
  EXPECT_EQ(test3.lo(), 0);

  fpu_fix_end(&old_cw);
}

TEST(DoubleWord, add) {
  // This test is a copy from two_sums in basic_core.
  // Testing that the computed result is correctly rounded and that the error is exact.
  unsigned int old_cw;
  fpu_fix_start(&old_cw);

  double l = 4503599627370496.0;
  double r = 1.0;
  auto out = dbl_wrd::add(l, r);
  EXPECT_EQ(out.lo(), 0.0);

  auto out1 = dbl_wrd::add(4503599627370496.0, 10.0);
  EXPECT_EQ(out1.lo(), 0.0);

  auto test1 = dbl_wrd::add(4503599627370496.0, 1000.0);
  EXPECT_EQ(test1.lo(), 0.0);

  double x = 4503599627370496.0;
  auto out2 = dbl_wrd::add(x, 0.5);
  EXPECT_EQ(out2.hi(), 4503599627370496.0); // Round to nearest - tie to even.
  EXPECT_EQ(out2.lo(), 0.5);

  auto out3 = dbl_wrd::add(x, 0.6);
  // .6 is not exactly representable as a double. Nearest number is 0.59999999999999997779553950749686919152736663818359375.
  EXPECT_EQ(out3.hi(), 4503599627370497.0);                                       // Round to nearest - tie to even.
  EXPECT_EQ(out3.lo(), -0.40000000000000002220446049250313080847263336181640625); // 0.4 cannot be exactly represented in binary.
                                                                                  // The err is truncated to the precision of a double.

  auto out4 = dbl_wrd::add(x, 0.75);
  EXPECT_EQ(out4.hi(), 4503599627370497.0); // Round to nearest - tie to even.
  EXPECT_EQ(out4.lo(), -0.25);

  auto out5 = dbl_wrd::add(x, 0.25);
  EXPECT_EQ(out5.hi(), 4503599627370496.0); // Round to nearest - tie to even.
  EXPECT_EQ(out5.lo(), 0.25);

  auto out6 = dbl_wrd::add(x, 0.31250001490116130486995871251565404236316680908203125);
  // 0.31250001490116130486995871251565404236316680908203125 is exactly representable
  EXPECT_EQ(out6.hi(), 4503599627370496.0); // Round to nearest - tie to even.
  EXPECT_EQ(out6.lo(), 0.31250001490116130486995871251565404236316680908203125);
  fpu_fix_end(&old_cw);
}

TEST(DoubleWord, plus_op) {
  unsigned int old_cw;
  fpu_fix_start(&old_cw);

  auto test1 = dbl_wrd(1) + dbl_wrd(.1);
  EXPECT_EQ(test1.hi(), 1.100000000000000088817841970012523233890533447265625);
  EXPECT_EQ(test1.lo(), -0.0000000000000000832667268468867405317723751068115234375);

  // 9007199254740991/2^54 = 0.499999999999999944488848768742172978818416595458984375
  // 9007199254740987.0/2.0 = 4503599627370493.5
  // 9007199254740991/2^56 = 0.12499999999999998612221219218554324470460414886474609375
  // actual = 4503599627370496.87500000000000006938893903907228377647697925567626953125
  auto test2 = dbl_wrd(9007199254740991.0, -0.499999999999999944488848768742172978818416595458984375) +
               dbl_wrd(-4503599627370493.5, -0.12499999999999998612221219218554324470460414886474609375);
  EXPECT_EQ(test2.hi(), 4503599627370497);
  EXPECT_EQ(test2.lo(), -0.125000000000000055511151231257827021181583404541015625);

  auto test3 = dbl_wrd(1) + 2.0;
  EXPECT_EQ(test3.hi(), 3.0);
  EXPECT_EQ(test3.lo(), 0.0);

  auto test4 = dbl_wrd(1, 0.0000000000000001110223024625156417164115227307725716917411674564651613561494514215155504643917083740234375) +
               -0.499999999999999944488848768742172978818416595458984375;
  EXPECT_EQ(test4.hi(), 0.5000000000000002220446049250313080847263336181640625);
  EXPECT_EQ(test4.lo(), -0.000000000000000055511151231257827021181583404541015625);

  auto test5 = 2.0 + dbl_wrd(1);
  EXPECT_EQ(test5.hi(), 3.0);
  EXPECT_EQ(test5.lo(), 0.0);

  auto test6 = dbl_wrd(1, 0.0000000000000001110223024625156417164115227307725716917411674564651613561494514215155504643917083740234375) +
               -0.499999999999999944488848768742172978818416595458984375;
  EXPECT_EQ(test6.hi(), 0.5000000000000002220446049250313080847263336181640625);
  EXPECT_EQ(test6.lo(), -0.000000000000000055511151231257827021181583404541015625);

  fpu_fix_end(&old_cw);
}

TEST(DoubleWord, plus_assign_op) {
  unsigned int old_cw;
  fpu_fix_start(&old_cw);

  auto test1 = dbl_wrd(1);
  test1 += dbl_wrd(.1);
  EXPECT_EQ(test1.hi(), 1.100000000000000088817841970012523233890533447265625);
  EXPECT_EQ(test1.lo(), -0.0000000000000000832667268468867405317723751068115234375);

  // 9007199254740991/2^54 = 0.499999999999999944488848768742172978818416595458984375
  // 9007199254740987.0/2.0 = 4503599627370493.5
  // 9007199254740991/2^56 = 0.12499999999999998612221219218554324470460414886474609375
  // actual = 4503599627370496.87500000000000006938893903907228377647697925567626953125
  auto test2 = dbl_wrd(9007199254740991.0, -0.499999999999999944488848768742172978818416595458984375);
  test2 += dbl_wrd(-4503599627370493.5, -0.12499999999999998612221219218554324470460414886474609375);
  EXPECT_EQ(test2.hi(), 4503599627370497);
  EXPECT_EQ(test2.lo(), -0.125000000000000055511151231257827021181583404541015625);

  auto test3 = dbl_wrd(1);
  test3 += 2.0;
  EXPECT_EQ(test3.hi(), 3.0);
  EXPECT_EQ(test3.lo(), 0.0);

  auto test4 = dbl_wrd(1, 0.0000000000000001110223024625156417164115227307725716917411674564651613561494514215155504643917083740234375);
  test4 += -0.499999999999999944488848768742172978818416595458984375;
  EXPECT_EQ(test4.hi(), 0.5000000000000002220446049250313080847263336181640625);
  EXPECT_EQ(test4.lo(), -0.000000000000000055511151231257827021181583404541015625);

  fpu_fix_end(&old_cw);
}

TEST(DoubleWord, sub) {
  // This test is a copy from two_sums in basic_core.
  // Testing that the computed result is correctly rounded and that the error is exact.
  unsigned int old_cw;
  fpu_fix_start(&old_cw);

  double l = 4503599627370496.0;
  double r = -1.0;
  auto out = dbl_wrd::sub(l, r);
  EXPECT_EQ(out.lo(), 0.0);

  auto out1 = dbl_wrd::sub(4503599627370496.0, -10.0);
  EXPECT_EQ(out1.lo(), 0.0);

  auto test1 = dbl_wrd::sub(4503599627370496.0, -1000.0);
  EXPECT_EQ(test1.lo(), 0.0);

  double x = 4503599627370496.0;
  auto out2 = dbl_wrd::sub(x, -0.5);
  EXPECT_EQ(out2.hi(), 4503599627370496.0); // Round to nearest - tie to even.
  EXPECT_EQ(out2.lo(), 0.5);

  auto out3 = dbl_wrd::sub(x, -0.6);
  // .6 is not exactly representable as a double. Nearest number is 0.59999999999999997779553950749686919152736663818359375.
  EXPECT_EQ(out3.hi(), 4503599627370497.0);                                       // Round to nearest - tie to even.
  EXPECT_EQ(out3.lo(), -0.40000000000000002220446049250313080847263336181640625); // 0.4 cannot be exactly represented in binary.
                                                                                  // The err is truncated to the precision of a double.

  auto out4 = dbl_wrd::sub(x, -0.75);
  EXPECT_EQ(out4.hi(), 4503599627370497.0); // Round to nearest - tie to even.
  EXPECT_EQ(out4.lo(), -0.25);

  auto out5 = dbl_wrd::sub(x, -0.25);
  EXPECT_EQ(out5.hi(), 4503599627370496.0); // Round to nearest - tie to even.
  EXPECT_EQ(out5.lo(), 0.25);

  auto out6 = dbl_wrd::sub(x, -0.31250001490116130486995871251565404236316680908203125);
  // 0.31250001490116130486995871251565404236316680908203125 is exactly representable
  EXPECT_EQ(out6.hi(), 4503599627370496.0); // Round to nearest - tie to even.
  EXPECT_EQ(out6.lo(), 0.31250001490116130486995871251565404236316680908203125);
  fpu_fix_end(&old_cw);
}

TEST(DoubleWord, sub_op) {
  unsigned int old_cw;
  fpu_fix_start(&old_cw);

  auto test1 = dbl_wrd(1) - dbl_wrd(-0.1);
  EXPECT_EQ(test1.hi(), 1.100000000000000088817841970012523233890533447265625);
  EXPECT_EQ(test1.lo(), -0.0000000000000000832667268468867405317723751068115234375);

  // 9007199254740991/2^54 = 0.499999999999999944488848768742172978818416595458984375
  // 9007199254740987.0/2.0 = 4503599627370493.5
  // 9007199254740991/2^56 = 0.12499999999999998612221219218554324470460414886474609375
  // actual = 4503599627370496.87500000000000006938893903907228377647697925567626953125
  auto test2 = dbl_wrd(9007199254740991.0, -0.499999999999999944488848768742172978818416595458984375) -
               dbl_wrd(4503599627370493.5, 0.12499999999999998612221219218554324470460414886474609375);
  EXPECT_EQ(test2.hi(), 4503599627370497);
  EXPECT_EQ(test2.lo(), -0.125000000000000055511151231257827021181583404541015625);

  auto test3 = dbl_wrd(1.0) - (-2.0);
  EXPECT_EQ(test3.hi(), 3.0);
  EXPECT_EQ(test3.lo(), 0.0);

  auto test4 = dbl_wrd(1, 0.0000000000000001110223024625156417164115227307725716917411674564651613561494514215155504643917083740234375) -
               0.499999999999999944488848768742172978818416595458984375;
  EXPECT_EQ(test4.hi(), 0.5000000000000002220446049250313080847263336181640625);
  EXPECT_EQ(test4.lo(), -0.000000000000000055511151231257827021181583404541015625);

  auto test5 = 2.0 - dbl_wrd(-1.0);
  EXPECT_EQ(test5.hi(), 3.0);
  EXPECT_EQ(test5.lo(), 0.0);

  auto test6 = dbl_wrd(1, 0.0000000000000001110223024625156417164115227307725716917411674564651613561494514215155504643917083740234375) -
               0.499999999999999944488848768742172978818416595458984375;
  EXPECT_EQ(test6.hi(), 0.5000000000000002220446049250313080847263336181640625);
  EXPECT_EQ(test6.lo(), -0.000000000000000055511151231257827021181583404541015625);

  fpu_fix_end(&old_cw);
}

TEST(DoubleWord, sub_assign_op) {
  unsigned int old_cw;
  fpu_fix_start(&old_cw);

  auto test1 = dbl_wrd(1);
  test1 -= dbl_wrd(-.1);
  EXPECT_EQ(test1.hi(), 1.100000000000000088817841970012523233890533447265625);
  EXPECT_EQ(test1.lo(), -0.0000000000000000832667268468867405317723751068115234375);

  // 9007199254740991/2^54 = 0.499999999999999944488848768742172978818416595458984375
  // 9007199254740987.0/2.0 = 4503599627370493.5
  // 9007199254740991/2^56 = 0.12499999999999998612221219218554324470460414886474609375
  // actual = 4503599627370496.87500000000000006938893903907228377647697925567626953125
  auto test2 = dbl_wrd(9007199254740991.0, -0.499999999999999944488848768742172978818416595458984375);
  test2 -= dbl_wrd(4503599627370493.5, 0.12499999999999998612221219218554324470460414886474609375);
  EXPECT_EQ(test2.hi(), 4503599627370497);
  EXPECT_EQ(test2.lo(), -0.125000000000000055511151231257827021181583404541015625);

  auto test3 = dbl_wrd(1);
  test3 -= -2.0;
  EXPECT_EQ(test3.hi(), 3.0);
  EXPECT_EQ(test3.lo(), 0.0);

  auto test4 = dbl_wrd(1, 0.0000000000000001110223024625156417164115227307725716917411674564651613561494514215155504643917083740234375);
  test4 -= 0.499999999999999944488848768742172978818416595458984375;
  EXPECT_EQ(test4.hi(), 0.5000000000000002220446049250313080847263336181640625);
  EXPECT_EQ(test4.lo(), -0.000000000000000055511151231257827021181583404541015625);

  fpu_fix_end(&old_cw);
}

TEST(DoubleWord, unary_minus_op) {
  auto test1 = -dbl_wrd(10, 1);
  EXPECT_EQ(test1.hi(), -10);
  EXPECT_EQ(test1.lo(), -1);
}

TEST(DoubleWord, mul) {
  // Testing that the computed result is correctly rounded and that the error is exact.
  unsigned int old_cw;
  fpu_fix_start(&old_cw);
  auto test1 = dbl_wrd::mul(1.0, 2.0);
  EXPECT_EQ(test1.lo(), 0.0);

  auto test2 = dbl_wrd::mul(2.0, 2.0);
  EXPECT_EQ(test2.hi(), 4.0);
  EXPECT_EQ(test2.lo(), 0.0);

  auto out = dbl_wrd::mul(2.3, 2); // 2.3 is not exacctly representable. It is rounded to 2.29999999999999982236431605997495353221893310546875
  EXPECT_EQ(out.hi(), 4.5999999999999996447286321199499070644378662109375);
  EXPECT_EQ(out.lo(), 0.0);

  // Using large numbers makes rounding error more likely because floats are further away from
  // each other.
  auto out2 = dbl_wrd::mul(73786905926110806016.0, 73786905926110806016.0);
  EXPECT_EQ(out2.hi(), 5444507486148725985994448939565027688448.0);
  EXPECT_EQ(out2.lo(), -2361182959959845896192.0); // 17.16015625 cannot be represented exactly as a double.

  auto test3 = dbl_wrd::mul(dbl_wrd(10), 2);
  EXPECT_EQ(test3.hi(), 20);
  EXPECT_EQ(test3.lo(), 0.0);

  auto test4 = dbl_wrd::mul(dbl_wrd(9), 8);
  EXPECT_EQ(test4.hi(), 72);
  EXPECT_EQ(test4.lo(), 0.0);
  // 8595672275350437/2^54 = 0.477155663611308178406744673338835127651691436767578125
  // p1 = 23016644260622007512850956812288
  // p2 = 2426653836559150.5 + 2101817653580456 = 4528471490139606
  auto test5 = dbl_wrd::mul(dbl_wrd(4525788557405064, 0.477155663611308178406744673338835127651691436767578125), 5085664955107621);
  EXPECT_EQ(test5.hi(), 23016644260622012016450584182784.0);
  EXPECT_EQ(test5.lo(), 24871862769110.0);

  // 4232862152422029/2^53 = 0.46994210216833354198939787238487042486667633056640625
  // 2250098448199619/2^52 = 0.4996222209729104246633824004675261676311492919921875
  // p1 = 20347333661531488182604713164800
  // p2 = 225905046906140
  // p3 = 0.2347935168140211914522552660855581052601337432861328125
  auto test6 = dbl_wrd::mul(dbl_wrd(4510026974538724, 0.46994210216833354198939787238487042486667633056640625),
                            dbl_wrd(4511576932111935, 0.4996222209729104246633824004675261676311492919921875));
  EXPECT_EQ(test6.hi(), 20347333661531492686204340535296.0);
  EXPECT_EQ(test6.lo(), 95795060773262);
  fpu_fix_end(&old_cw);
}

TEST(DoubleWord, imp_mul) {
  unsigned int old_cw;
  fpu_fix_start(&old_cw);
  // -900326552954291/(2^54) = -0.049978163438562706044621108958381228148937225341796875
  // p1 = 20326714295214554668633939771392
  // p2 = -2024788993803394.5
  auto test1 = dbl_wrd::imp_mul(dbl_wrd(4505619370757448, -0.049978163438562706044621108958381228148937225341796875), 4511413997183120.0);
  EXPECT_EQ(test1.hi(), 20326714295214554668633939771392.0);
  EXPECT_EQ(test1.lo(), 2024788993803394.5);

  fpu_fix_end(&old_cw);
}

TEST(DoubleWord, multi_op) {
  unsigned int old_cw;
  fpu_fix_start(&old_cw);

  auto test3 = dbl_wrd(10) * 2;
  EXPECT_EQ(test3.hi(), 20);
  EXPECT_EQ(test3.lo(), 0.0);

  auto test4 = dbl_wrd(9) * 8;
  EXPECT_EQ(test4.hi(), 72);
  EXPECT_EQ(test4.lo(), 0.0);
  // -900326552954291/(2^54) = -0.049978163438562706044621108958381228148937225341796875
  // p1 = 20326714295214554668633939771392
  // p2 = -2024788993803394.5
  auto test5 = dbl_wrd::imp_mul(dbl_wrd(4505619370757448, -0.049978163438562706044621108958381228148937225341796875), 4511413997183120.0);
  EXPECT_EQ(test5.hi(), 20326714295214554668633939771392.0);
  EXPECT_EQ(test5.lo(), 2024788993803394.5);

  dbl_wrd test8 = dbl_wrd(4505619370757448, -0.049978163438562706044621108958381228148937225341796875);
  test8 *= 4511413997183120.0;
  EXPECT_EQ(test8.hi(), 20326714295214554668633939771392.0);
  EXPECT_EQ(test8.lo(), 2024788993803394.5);

  auto test1 = 2 * dbl_wrd(10);
  EXPECT_EQ(test1.hi(), 20);
  EXPECT_EQ(test1.lo(), 0.0);

  auto test2 = 8 * dbl_wrd(9);
  EXPECT_EQ(test2.hi(), 72);
  EXPECT_EQ(test2.lo(), 0.0);
  auto test7 = dbl_wrd::imp_mul(dbl_wrd(4505619370757448, -0.049978163438562706044621108958381228148937225341796875), 4511413997183120.0);
  EXPECT_EQ(test7.hi(), 20326714295214554668633939771392.0);
  EXPECT_EQ(test7.lo(), 2024788993803394.5);

  // 4232862152422029/2^53 = 0.46994210216833354198939787238487042486667633056640625
  // 2250098448199619/2^52 = 0.4996222209729104246633824004675261676311492919921875
  // p1 = 20347333661531488182604713164800
  // p2 = 225905046906140
  // p3 = 0.2347935168140211914522552660855581052601337432861328125
  auto test6 = dbl_wrd(4510026974538724, 0.46994210216833354198939787238487042486667633056640625) *
               dbl_wrd(4511576932111935, 0.4996222209729104246633824004675261676311492919921875);
  EXPECT_EQ(test6.hi(), 20347333661531492686204340535296.0);
  EXPECT_EQ(test6.lo(), 95795060773262);

  dbl_wrd test9 = dbl_wrd(4510026974538724, 0.46994210216833354198939787238487042486667633056640625);
  test9 *= dbl_wrd(4511576932111935, 0.4996222209729104246633824004675261676311492919921875);
  EXPECT_EQ(test9.hi(), 20347333661531492686204340535296.0);
  EXPECT_EQ(test9.lo(), 95795060773262);

  fpu_fix_end(&old_cw);
}

TEST(DoubleWord, div) {
  unsigned int old_cw;
  fpu_fix_start(&old_cw);
  auto test1 = dbl_wrd::div(1.0, 2.0);
  EXPECT_EQ(test1.lo(), 0.0);

  auto test2 = dbl_wrd::div(2.0, 2.0);
  EXPECT_EQ(test2.hi(), 1.0);
  EXPECT_EQ(test2.lo(), 0.0);

  auto out = dbl_wrd::div(2.3, 2); // 2.3 is not exacctly representable. It is rounded to 2.29999999999999982236431605997495353221893310546875
  EXPECT_EQ(out.hi(), 1.149999999999999911182158029987476766109466552734375);
  EXPECT_EQ(out.lo(), 0.0);

  auto out2 = dbl_wrd::div(73786905926110806016.0, 9);
  EXPECT_EQ(out2.hi(), 8198545102901200896.0);
  EXPECT_EQ(out2.lo(), -227.5555555555555429236846975982189178466796875); // error is rounded.
  fpu_fix_end(&old_cw);
}

TEST(DoubleWord, sloppy_div) {
  unsigned int old_cw;
  fpu_fix_start(&old_cw);
  auto test1 = dbl_wrd::sloppy_div(dbl_wrd(1.0), dbl_wrd(2.0));
  EXPECT_EQ(test1.hi(), .5);
  EXPECT_EQ(test1.lo(), 0.0);

  auto test2 = dbl_wrd::sloppy_div(dbl_wrd(2.0), dbl_wrd(2.0));
  EXPECT_EQ(test2.hi(), 1.0);
  EXPECT_EQ(test2.lo(), 0.0);

  auto out = dbl_wrd::sloppy_div(
      dbl_wrd(2.3), dbl_wrd(2)); // 2.3 is not exacctly representable. It is rounded to 2.29999999999999982236431605997495353221893310546875
  EXPECT_EQ(out.hi(), 1.149999999999999911182158029987476766109466552734375);
  EXPECT_EQ(out.lo(), 0.0);

  // q1 = 8198545102901200896
  // r = dbl_wrd(73786905926110806016, -2048)
  // s1 = 0
  // s2 = 2048
  // q2 = 227.5555555555555429236846975982189178466796875
  // Error is larger than double-double / double
  auto out2 = dbl_wrd::sloppy_div(dbl_wrd(73786905926110806016.0), dbl_wrd(9));
  EXPECT_EQ(out2.hi(), 8198545102901200896.0);
  EXPECT_EQ(out2.lo(), 227.5555555555555429236846975982189178466796875); // error is rounded.
  fpu_fix_end(&old_cw);
}

TEST(DoubleWord, accurate_div) {
  unsigned int old_cw;
  fpu_fix_start(&old_cw);
  auto test1 = dbl_wrd::accurate_div(dbl_wrd(1.0), dbl_wrd(2.0));
  EXPECT_EQ(test1.hi(), .5);
  EXPECT_EQ(test1.lo(), 0.0);

  auto test2 = dbl_wrd::accurate_div(dbl_wrd(2.0), dbl_wrd(2.0));
  EXPECT_EQ(test2.hi(), 1.0);
  EXPECT_EQ(test2.lo(), 0.0);

  auto out = dbl_wrd::accurate_div(
      dbl_wrd(2.3), dbl_wrd(2)); // 2.3 is not exacctly representable. It is rounded to 2.29999999999999982236431605997495353221893310546875
  EXPECT_EQ(out.hi(), 1.149999999999999911182158029987476766109466552734375);
  EXPECT_EQ(out.lo(), 0.0);

  // q1 = 8198545102901200896
  // r = dbl_wrd(73786905926110806016, -2048)
  // s1 = 0
  // s2 = 2048
  // q2 = 227.5555555555555429236846975982189178466796875
  // Error is larger than double-double / double
  auto out2 = dbl_wrd::accurate_div(dbl_wrd(73786905926110806016.0), dbl_wrd(9));
  EXPECT_EQ(out2.hi(), 8198545102901200896.0);
  EXPECT_EQ(out2.lo(), 227.5555555555555429236846975982189178466796875); // error is rounded.
  fpu_fix_end(&old_cw);
}

TEST(DoubleWrd, div_op) {
  unsigned int old_cw;
  fpu_fix_start(&old_cw);

  auto out2 = dbl_wrd(73786905926110806016.0, 0) / 9;
  EXPECT_EQ(out2.hi(), 8198545102901200896.0);
  EXPECT_EQ(out2.lo(), -227.5555555555555429236846975982189178466796875); // error is rounded.

  // q1 = 8198545102901200896
  // p1 = 73786905926110806016
  // p2 = 2048
  auto out3 = dbl_wrd(73786905926110806016.0, 0.00000000000000016653345369379872461251182259139622218908904915934954260592348873615264892578125) / 9;
  EXPECT_EQ(out3.hi(), 8198545102901200896.0);
  EXPECT_EQ(out3.lo(), -227.5555555555555429236846975982189178466796875); // error is rounded.

  // q1 = 8198545102901200896
  // p1 = 73786905926110806016
  // p2 = 2048
  // s_h = 0
  // s_t = -2048
  // p2 = 4952
  // q2 = 550.22222222222217169473879039287567138671875
  auto out4 = dbl_wrd(73786905926110806016.0, 7000.0) / 9;
  EXPECT_EQ(out4.hi(), 8198545102901201920);
  EXPECT_EQ(out4.lo(), -473.77777777777782830526120960712432861328125);

  auto out5 = 9 / dbl_wrd(73786905926110806016.0, 0.00000000000000016653345369379872461251182259139622218908904915934954260592348873615264892578125);
  EXPECT_EQ(out5.hi(), 8198545102901200896.0);
  EXPECT_EQ(out5.lo(), -227.5555555555555429236846975982189178466796875);

  auto out6 = 9 / dbl_wrd(73786905926110806016.0, 7000.0);
  EXPECT_EQ(out6.hi(), 8198545102901201920);
  EXPECT_EQ(out6.lo(), -473.77777777777782830526120960712432861328125);

  auto test1 = dbl_wrd(1.0) / dbl_wrd(2.0);
  EXPECT_EQ(test1.hi(), .5);
  EXPECT_EQ(test1.lo(), 0.0);

  auto test2 = dbl_wrd(2.0) / dbl_wrd(2.0);
  EXPECT_EQ(test2.hi(), 1.0);
  EXPECT_EQ(test2.lo(), 0.0);

  auto test3 = dbl_wrd(2.3) / dbl_wrd(2); // 2.3 is not exacctly representable. It is rounded to 2.29999999999999982236431605997495353221893310546875
  EXPECT_EQ(test3.hi(), 1.149999999999999911182158029987476766109466552734375);
  EXPECT_EQ(test3.lo(), 0.0);

  // q1 = 8198545102901200896
  // r = dbl_wrd(73786905926110806016, -2048)
  // s1 = 0
  // s2 = 2048
  // q2 = 227.5555555555555429236846975982189178466796875
  // Error is larger than double-double / double
  auto test4 = dbl_wrd(73786905926110806016.0) / dbl_wrd(9);
  EXPECT_EQ(test4.hi(), 8198545102901200896.0);
  EXPECT_EQ(test4.lo(), 227.5555555555555429236846975982189178466796875); // error is rounded.

  auto test5 = dbl_wrd(73786905926110806016.0, 0);
  test5 /= 9;
  EXPECT_EQ(test5.hi(), 8198545102901200896.0);
  EXPECT_EQ(test5.lo(), -227.5555555555555429236846975982189178466796875); // error is rounded.

  // q1 = 8198545102901200896
  // p1 = 73786905926110806016
  // p2 = 2048
  auto test6 = dbl_wrd(73786905926110806016.0, 0.00000000000000016653345369379872461251182259139622218908904915934954260592348873615264892578125);
  test6 /= 9;
  EXPECT_EQ(test6.hi(), 8198545102901200896.0);
  EXPECT_EQ(test6.lo(), -227.5555555555555429236846975982189178466796875); // error is rounded.

  // q1 = 8198545102901200896
  // p1 = 73786905926110806016
  // p2 = 2048
  // s_h = 0
  // s_t = -2048
  // p2 = 4952
  // q2 = 550.22222222222217169473879039287567138671875
  auto test7 = dbl_wrd(73786905926110806016.0, 7000.0);
  test7 /= 9;
  EXPECT_EQ(test7.hi(), 8198545102901201920);
  EXPECT_EQ(test7.lo(), -473.77777777777782830526120960712432861328125);

  auto test8 = dbl_wrd(1.0);
  test8 /= dbl_wrd(2.0);
  EXPECT_EQ(test8.hi(), .5);
  EXPECT_EQ(test8.lo(), 0.0);

  auto test9 = dbl_wrd(2.0);
  test9 /= dbl_wrd(2.0);
  EXPECT_EQ(test9.hi(), 1.0);
  EXPECT_EQ(test9.lo(), 0.0);

  auto test10 = dbl_wrd(2.3);
  test10 /= dbl_wrd(2); // 2.3 is not exacctly representable. It is rounded to 2.29999999999999982236431605997495353221893310546875
  EXPECT_EQ(test10.hi(), 1.149999999999999911182158029987476766109466552734375);
  EXPECT_EQ(test10.lo(), 0.0);

  // q1 = 8198545102901200896
  // r = dbl_wrd(73786905926110806016, -2048)
  // s1 = 0
  // s2 = 2048
  // q2 = 227.5555555555555429236846975982189178466796875
  // Error is larger than double-double / double
  auto test11 = dbl_wrd(73786905926110806016.0);
  test11 /= dbl_wrd(9);
  EXPECT_EQ(test11.hi(), 8198545102901200896.0);
  EXPECT_EQ(test11.lo(), 227.5555555555555429236846975982189178466796875); // error is rounded.

  fpu_fix_end(&old_cw);
}
