#include "double_word/dw.h"
#include "double_word/bsc_core.h"
#include <limits>


const dbl_wrd dbl_wrd::_e = dbl_wrd(2.718281828459045091e+00, 1.445646891729250158e-16);      // e^1
const dbl_wrd dbl_wrd::_log2 = dbl_wrd(6.931471805599452862e-01, 2.319046813846299558e-17);   // ln(2)
const dbl_wrd dbl_wrd::_log10 = dbl_wrd(2.302585092994045901e+00, -2.170756223382249351e-16); // ln(10)
const dbl_wrd dbl_wrd::_nan = dbl_wrd(basic_core::_dbl_nan, basic_core::_dbl_nan);
const dbl_wrd dbl_wrd::_inf = dbl_wrd(basic_core::_dbl_inf, basic_core::_dbl_inf);

// TODO: There is not much in the literature on what we should set for these values.
const double dbl_wrd::_eps = 4.93038065763132e-32; // $2^{-104}$ This is arbitrarily determined as double words do not have a set precision. It
                                                     // wobbles. Reference page 515 in Handbook of floating point arithmetic by Muller et al.
const double dbl_wrd::_min_normalized = std::numeric_limits<double>::min();
const dbl_wrd dbl_wrd::_max = dbl_wrd(std::numeric_limits<double>::max(), std::numeric_limits<double>::max()); // double maximum is 1.79769e+308
const int dbl_wrd::_digits = 31; // This represents the approximate precision in base 10 numbers.
