#include "double_word/dw.h"
#include "double_word/bsc_core.h"
#include <limits>


const double_wd double_wd::_e = double_wd(2.718281828459045091e+00, 1.445646891729250158e-16);      // e^1
const double_wd double_wd::_log2 = double_wd(6.931471805599452862e-01, 2.319046813846299558e-17);   // ln(2)
const double_wd double_wd::_log10 = double_wd(2.302585092994045901e+00, -2.170756223382249351e-16); // ln(10)
const double_wd double_wd::_nan = double_wd(basic_core::_dbl_nan, basic_core::_dbl_nan);
const double_wd double_wd::_inf = double_wd(basic_core::_dbl_inf, basic_core::_dbl_inf);

// TODO: There is not much in the literature on what we should set for these values.
const double double_wd::_eps = 4.93038065763132e-32; // $2^{-104}$ This is arbitrarily determined as double words do not have a set precision. It
                                                     // wobbles. Reference page 515 in Handbook of floating point arithmetic by Muller et al.
const double double_wd::_min_normalized = std::numeric_limits<double>::min();
const double_wd double_wd::_max = double_wd(std::numeric_limits<double>::max(), std::numeric_limits<double>::max()); // double maximum is 1.79769e+308
const int double_wd::_digits = 31; // This represents the approximate precision in base 10 numbers.
