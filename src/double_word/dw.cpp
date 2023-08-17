#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <random>
#include <string>

#include "double_word/dw.h"

#include "double_word/bsc_core.h"

using std::cerr;
using std::cout;
using std::endl;
using std::ios_base;
using std::istream;
using std::ostream;
using std::setw;
using std::string;

// Wrap around basic_core to isolate core routines.
double dbl_wrd::_fast_two_sum(double a, double b, double &err) { return basic_core::fast_two_sum(a, b, err); }

double dbl_wrd::_fast_two_diff(double a, double b, double &err) { return basic_core::fast_two_diff(a, b, err); }

double dbl_wrd::_two_sum(double a, double b, double &err) { return basic_core::two_sum(a, b, err); }

double dbl_wrd::_two_diff(double a, double b, double &err) { return basic_core::two_diff(a, b, err); }

double dbl_wrd::_two_prod(double a, double b, double &err) { return basic_core::two_prod(a, b, err); }

double dbl_wrd::_two_square(double a, double &err) { return basic_core::two_square(a, err); }

double dbl_wrd::_nint(double d) { return basic_core::nint(d); }

double dbl_wrd::_aint(double d) { return basic_core::aint(d); }

double dbl_wrd::_square(double t) { return basic_core::square(t); }

int dbl_wrd::_to_int(double a) { return basic_core::to_int(a); }

/*********** Additions ************/
dbl_wrd dbl_wrd::add(double a, double b) {
  /* double-double = double + double */
  double s, e;
  s = _two_sum(a, b, e);
  return dbl_wrd(s, e);
}

inline dbl_wrd dbl_wrd::accurate_add(const dbl_wrd &a, const dbl_wrd &b) {
  /* double-double + double-double
   * This is more accurate than the sloppy_add routine and has no limitation on the signs, but is twice as slow.
   *
   * AccurateDWPlusDW
   * Joldes, Muller, and Popescu proved a relative error upper bound of:
   * $3*2^{-2p} + 13 * 2^{-3p}$
   * where p is the precision of double, which is 53.
   */
  double s1, s2, t1, t2;

  s1 = _two_sum(a.x[0], b.x[0], s2);
  t1 = _two_sum(a.x[1], b.x[1], t2);
  s2 += t1;
  s1 = _fast_two_sum(s1, s2, s2);
  s2 += t2;
  s1 = _fast_two_sum(s1, s2, s2);
  return dbl_wrd(s1, s2);
}

inline dbl_wrd dbl_wrd::sloppy_add(const dbl_wrd &a, const dbl_wrd &b) {
  /* This is the less accurate version than accurate_add, but faster.
   * This routine should only be used when the signs on a and b are the same.
   *
   * Dekker proved a relative error of $4u^2$ where u is $1/2 * ulp(1)$.
   */
  double s, e;
  s = _two_sum(a.x[0], b.x[0], e);
  e += (a.x[1] + b.x[1]);
  s = _fast_two_sum(s, e, e);
  return dbl_wrd(s, e);
}

inline dbl_wrd operator+(const dbl_wrd &a, double b) {
  /* double-double + double
   *
   * Joldes, Muller, and Popescu proved a relative error upper bound of:
   * $2 * u^2$,
   * where p is the precision of double, which is 53.
   * */
  double s_h, s_l;
  s_h = dbl_wrd::_two_sum(a.x[0], b, s_l);
  s_l += a.lo();
  s_h = dbl_wrd::_fast_two_sum(s_h, s_l, s_l);
  return dbl_wrd(s_h, s_l);
}

inline dbl_wrd operator+(const dbl_wrd &a, const dbl_wrd &b) {
#ifndef ACCURATE_ADD
  return dbl_wrd::sloppy_add(a, b);
#else
  return dbl_wrd::accurate_add(a, b);
#endif
}

/* double + double-double */
inline dbl_wrd operator+(double a, const dbl_wrd &b) {
  // calls operator+(const dbl_wrd &a, double b)
  return (b + a);
}

/*********** Self-Additions ************/
/* double-double += double */
inline dbl_wrd &dbl_wrd::operator+=(double a) {
  /* double-double + double
   * Joldes, Muller, and Popescu proved a relative error upper bound of:
   * $2 * u^2$,
   * where p is the precision of double, which is 53.
   *
   * Implementation is the same as dbl_wrd operator+(const dbl_wrd &a, double b) overload.
   * */
  double s1, s2;
  s1 = basic_core::two_sum(x[0], a, s2);
  s2 += x[1];
  x[0] = basic_core::fast_two_sum(s1, s2, x[1]);
  return *this;
}

/* double-double += double-double */
inline dbl_wrd &dbl_wrd::operator+=(const dbl_wrd &a) {
#ifndef ACCURATE_ADD
  // sloppy_add
  // double s, e;
  // s = basic_core::two_sum(x[0], a.x[0], e);
  // e += x[1];
  // e += a.x[1];
  // x[0] = basic_core::quick_two_sum(s, e, x[1]);
  // TODO: This might not work.
  *this = dbl_wrd::sloppy_add(*this, a);
  return *this;
#else
  // ieee_add
  // double s1, s2, t1, t2;
  // s1 = basic_core::two_sum(x[0], a.x[0], s2);
  // t1 = basic_core::two_sum(x[1], a.x[1], t2);
  // s2 += t1;
  // s1 = basic_core::fast_two_sum(s1, s2, s2);
  // s2 += t2;
  // x[0] = basic_core::fast_two_sum(s1, s2, x[1]);
  *this = dbl_wrd::accurate_add(*this, a);
  return *this;
#endif
}

/*********** Subtractions ************/
inline dbl_wrd dbl_wrd::sub(double a, double b) {
  /* double-double = double - double */
  double s, e;
  s = basic_core::two_diff(a, b, e);
  return dbl_wrd(s, e);
}

inline dbl_wrd operator-(const dbl_wrd &a, double b) {
  /* double-double - double
   * calls the dbl_wrd operator+(const dbl_wrd &a, double b) overload.
   * */
  return (a + -b);
}

inline dbl_wrd operator-(const dbl_wrd &a, const dbl_wrd &b) {
/* double-double - double-double */
#ifndef ACCURATE_ADD
  return dbl_wrd::sloppy_add(a, -b);
#else
  return dbl_wrd::accurate_add(a, -b);
#endif
}

inline dbl_wrd operator-(double a, const dbl_wrd &b) {
  /* double - double-double
   * Calls the dbl_wrd operator-(const dbl_wrd &a, const dbl_wrd &b) overload.
   * */
  return (b - a);
}

/*********** Self-Subtractions ************/
inline dbl_wrd &dbl_wrd::operator-=(double a) {
  /* double-double -= double
   * Calls the dbl_wrd operator+(const dbl_wrd &a, double b) overload.
   * */
  *this += -a; // TODO: Does this work?
  return *this;
}

inline dbl_wrd &dbl_wrd::operator-=(const dbl_wrd &a) {
/* double-double -= double-double */
#ifndef ACCURATE_ADD
  *this = dbl_wrd::sloppy_add(*this, -a);
  return *this;
#else
  *this = dbl_wrd::accurate_add(*this, -a);
  return *this;
#endif
}

/*********** Unary Minus ***********/
inline dbl_wrd dbl_wrd::operator-() const { return dbl_wrd(-x[0], -x[1]); }

/*********** Multiplications ************/
inline dbl_wrd dbl_wrd::mul(double a, double b) {
  /* double-double = double * double */
  double p, e;
  p = basic_core::two_prod(a, b, e);
  return dbl_wrd(p, e);
}

inline dbl_wrd ldexp(const dbl_wrd &a, int exp) {
  /* Multiplies a double-double a by 2 to the power of exp.
   * double-double * (2.0 ^ exp)
   * Overload the std::ldexp function.
   *
   * This multiplication should be exact because we are multiplying by a power of 2 as long as overflow does not occur.
   *
   * If a range error due to overflow occurs, ±HUGE_VAL, ±HUGE_VALF, or ±HUGE_VALL is returned.
   * If a range error due to underflow occurs, the exact result (after rounding) is returned.
   * On many implementations, std::ldexp is less efficient than multiplication or division by a power of two using arithmetic operators.
   * I think implementation here is referring to the computing system and compiler.
   *
   * */
  return dbl_wrd(std::ldexp(a.x[0], exp), std::ldexp(a.x[1], exp));
}

inline dbl_wrd mul_pwr2(const dbl_wrd &a, double b) {
  /*
   * double-double * double, where double is a power of 2.
   * If b is a power of 2, then you can just simply multiply straight away without a special
   * algorithm. It will return an exact result.
   * From the definition of a floating point number, multiplying by a power of 2 has the effect of increasing the exponent.
   * */
  return dbl_wrd(a.x[0] * b, a.x[1] * b);
}

inline dbl_wrd operator*(const dbl_wrd &a, double b) { return dbl_wrd::imp_mul(a, b); }

inline dbl_wrd dbl_wrd::mul(const dbl_wrd &a, double b) {
  /* double-double * double
   * Relative error is upper bounded by $3*u^2$.
   *
   * Reference:
   * Algorithm 8 in the paper Tight and rigorous error bounds for basic building blocks of double-word arithmetic
   * by Joldes, Muller, and Popescu.
   * */
  double p1, p2;
  p1 = dbl_wrd::_two_prod(a.x[0], b, p2);
  p2 += (a.x[1] * b);
  p1 = dbl_wrd::_fast_two_sum(p1, p2, p2);
  return dbl_wrd(p1, p2);
}

inline dbl_wrd dbl_wrd::imp_mul(const dbl_wrd &a, double b) {
  /* double-double * double
   * This routine is more accurate than the mul routine and is slightly faster.
   *
   * Relative error is upper bounded by $2*u^2$.
   *
   * Reference:
   * Algorithm 9 in the paper Tight and rigorous error bounds for basic building blocks of double-word arithmetic
   * by Joldes, Muller, and Popescu.
   * */
  double p1, p2;
  p1 = dbl_wrd::_two_prod(a.x[0], b, p2);
#ifdef __FMA__
  p2 = fma(a.x[1], b, p2);
#else
  // TODO: Make sure this is correct
  double a_hi, a_lo, b_hi, b_lo;
  basic_core::split(a.x[1], a_hi, a_lo);
  basic_core::split(b, b_hi, b_lo);
  p2 = ((a_hi * b_hi - p2) + a_hi * b_lo + a_lo * b_hi) + a_lo * b_lo;
#endif
  p1 = dbl_wrd::_fast_two_sum(p1, p2, p2);
  return dbl_wrd(p1, p2);
}

inline dbl_wrd dbl_wrd::mul(const dbl_wrd &a, const dbl_wrd &b) {
  /* double-double * double-double
   *
   * Relative error is upper bounded by $5^2$.
   *
   * Reference:
   * Algorithm 12 in the paper Tight and rigorous error bounds for basic building blocks of double-word arithmetic
   * by Joldes, Muller, and Popescu.
   * */

  double p1, p2, p3;
  p1 = dbl_wrd::_two_prod(a.x[0], b.x[0], p2);
  p3 = a.x[1] * b.x[1];
  p3 = fma(a.x[0], b.x[1], p3);
  p3 = fma(a.x[1], b.x[0], p3);
  p3 += p2;
  p1 = dbl_wrd::_fast_two_sum(p1, p3, p2);
  return dbl_wrd(p1, p2);
}

inline dbl_wrd operator*(const dbl_wrd &a, const dbl_wrd &b) {
  /* double-double * double-double
   *
   * Relative error is upper bounded by $5^2$.
   *
   * Reference:
   * Algorithm 12 in the paper Tight and rigorous error bounds for basic building blocks of double-word arithmetic
   * by Joldes, Muller, and Popescu.
   * */
  return dbl_wrd::mul(a, b);
}

inline dbl_wrd operator*(double a, const dbl_wrd &b) {
  /* double * double-double */
  // Calls the overloaded declaration dbl_wrd operator*(const dbl_wrd &a, double b)
  return (b * a);
}

/*********** Self-Multiplications ************/
inline dbl_wrd &dbl_wrd::operator*=(double a) {
  /* double-double *= double */
  *this = dbl_wrd::imp_mul(*this, a);
  return *this;
}

inline dbl_wrd &dbl_wrd::operator*=(const dbl_wrd &a) {
  /* double-double *= double-double */
  *this = dbl_wrd::mul(*this, a);
  return *this;
}

/*********** Divisions ************/
inline dbl_wrd dbl_wrd::div(double a, double b) {
  double q1, q2;
  double p1, p2;
  double s, e;

  q1 = a / b;

  /* Compute  a - q1 * b */
  p1 = dbl_wrd::_two_prod(q1, b, p2);
  s = dbl_wrd::_two_diff(a, p1, e);
  e -= p2;

  /* get next approximation */
  q2 = (s + e) / b;

  s = dbl_wrd::_fast_two_sum(q1, q2, e);

  return dbl_wrd(s, e);
}

/* double-double / double */
inline dbl_wrd operator/(const dbl_wrd &a, double b) {

  double q1, q2;
  double p1, p2;
  double s, e;
  dbl_wrd r;

  q1 = a.hi() / b; /* approximate quotient. */

  /* Compute this - q1 * d */
  p1 = dbl_wrd::_two_prod(q1, b, p2);
  s = dbl_wrd::_two_diff(a.x[0], p1, e);
  e += a.x[1];
  e -= p2;

  /* get next approximation. */
  q2 = (s + e) / b;

  /* renormalize */
  double temp;
  r.set_hi(dbl_wrd::_fast_two_sum(q1, q2, temp));
  r.set_lo(temp);
  return r;
}

inline dbl_wrd dbl_wrd::sloppy_div(const dbl_wrd &a, const dbl_wrd &b) {
  double s1, s2;
  double q1, q2;
  dbl_wrd r;

  q1 = a.x[0] / b.x[0]; /* approximate quotient */

  /* compute  this - q1 * dd */
  r = b * q1;
  s1 = dbl_wrd::_two_diff(a.x[0], r.x[0], s2);
  s2 -= r.x[1];
  s2 += a.x[1];

  /* get next approximation */
  q2 = (s1 + s2) / b.x[0];

  /* renormalize */
  r.x[0] = dbl_wrd::_fast_two_sum(q1, q2, r.x[1]);
  return r;
}

inline dbl_wrd dbl_wrd::accurate_div(const dbl_wrd &a, const dbl_wrd &b) {
  double q1, q2, q3;
  dbl_wrd r;

  q1 = a.x[0] / b.x[0]; /* approximate quotient */

  r = a - q1 * b;

  q2 = r.x[0] / b.x[0];
  r -= (q2 * b);

  q3 = r.x[0] / b.x[0];

  q1 = dbl_wrd::_fast_two_sum(q1, q2, q2);
  r = dbl_wrd(q1, q2) + q3;
  return r;
}

/* double-double / double-double */
inline dbl_wrd operator/(const dbl_wrd &a, const dbl_wrd &b) {
#ifdef QD_SLOPPY_DIV
  return dbl_wrd::sloppy_div(a, b);
#else
  return dbl_wrd::accurate_div(a, b);
#endif
}

/* double / double-double */
inline dbl_wrd operator/(double a, const dbl_wrd &b) { return dbl_wrd(a) / b; }

inline dbl_wrd inv(const dbl_wrd &a) { return 1.0 / a; }

/*********** Self-Divisions ************/
/* double-double /= double */
inline dbl_wrd &dbl_wrd::operator/=(double a) {
  *this = *this / a;
  return *this;
}

/* double-double /= double-double */
inline dbl_wrd &dbl_wrd::operator/=(const dbl_wrd &a) {
  *this = *this / a;
  return *this;
}

/********** Remainder **********/
inline dbl_wrd drem(const dbl_wrd &a, const dbl_wrd &b) {
  // Returns only the remainder
  dbl_wrd n = nint(a / b);
  return (a - n * b);
}

inline dbl_wrd divrem(const dbl_wrd &a, const dbl_wrd &b, dbl_wrd &r) {
  // Return the quotient n and the remainder r.
  dbl_wrd n = nint(a / b);
  r = a - n * b;
  return n;
}

/*********** Squaring **********/
inline dbl_wrd sqr(const dbl_wrd &a) { return dbl_wrd::mul(a, a); }

inline dbl_wrd dbl_wrd::square(double a) {
  double p1, p2;
  p1 = dbl_wrd::_two_square(a, p2);
  return dbl_wrd(p1, p2);
}

/********** Exponentiation **********/
inline dbl_wrd dbl_wrd::operator^(int n) { return npwr(*this, n); }

/*********** Assignments ************/
/* double-double = double */
inline dbl_wrd &dbl_wrd::operator=(double a) {
  x[0] = a;
  x[1] = 0.0;
  return *this;
}

/*********** Equality Comparisons ************/
/* double-double == double */
inline bool operator==(const dbl_wrd &a, double b) { return (a.x[0] == b && a.x[1] == 0.0); }

/* double-double == double-double */
inline bool operator==(const dbl_wrd &a, const dbl_wrd &b) { return (a.x[0] == b.x[0] && a.x[1] == b.x[1]); }

/* double == double-double */
inline bool operator==(double a, const dbl_wrd &b) { return (a == b.x[0] && b.x[1] == 0.0); }

/*********** Greater-Than Comparisons ************/
/* double-double > double */
inline bool operator>(const dbl_wrd &a, double b) { return (a.x[0] > b || (a.x[0] == b && a.x[1] > 0.0)); }

/* double-double > double-double */
inline bool operator>(const dbl_wrd &a, const dbl_wrd &b) { return (a.x[0] > b.x[0] || (a.x[0] == b.x[0] && a.x[1] > b.x[1])); }

/* double > double-double */
inline bool operator>(double a, const dbl_wrd &b) { return (a > b.x[0] || (a == b.x[0] && b.x[1] < 0.0)); }

/*********** Less-Than Comparisons ************/
/* double-double < double */
inline bool operator<(const dbl_wrd &a, double b) { return (a.x[0] < b || (a.x[0] == b && a.x[1] < 0.0)); }

/* double-double < double-double */
inline bool operator<(const dbl_wrd &a, const dbl_wrd &b) { return (a.x[0] < b.x[0] || (a.x[0] == b.x[0] && a.x[1] < b.x[1])); }

/* double < double-double */
inline bool operator<(double a, const dbl_wrd &b) { return (a < b.x[0] || (a == b.x[0] && b.x[1] > 0.0)); }

/*********** Greater-Than-Or-Equal-To Comparisons ************/
/* double-double >= double */
inline bool operator>=(const dbl_wrd &a, double b) { return (a.x[0] > b || (a.x[0] == b && a.x[1] >= 0.0)); }

/* double-double >= double-double */
inline bool operator>=(const dbl_wrd &a, const dbl_wrd &b) {
  // Can't compare a.x[0] + a.x[1] > b.x[0] + b.x[1] because the sum is not exactly representable.
  return (a.x[0] > b.x[0] || (a.x[0] == b.x[0] && a.x[1] >= b.x[1]));
}

/* double >= double-double */
inline bool operator>=(double a, const dbl_wrd &b) { return (b <= a); }

/*********** Less-Than-Or-Equal-To Comparisons ************/
/* double-double <= double */
inline bool operator<=(const dbl_wrd &a, double b) { return (a.x[0] < b || (a.x[0] == b && a.x[1] <= 0.0)); }

/* double-double <= double-double */
inline bool operator<=(const dbl_wrd &a, const dbl_wrd &b) { return (a.x[0] < b.x[0] || (a.x[0] == b.x[0] && a.x[1] <= b.x[1])); }

/* double <= double-double */
inline bool operator<=(double a, const dbl_wrd &b) { return (b >= a); }

/*********** Not-Equal-To Comparisons ************/
/* double-double != double */
inline bool operator!=(const dbl_wrd &a, double b) { return (a.x[0] != b || a.x[1] != 0.0); }

/* double-double != double-double */
inline bool operator!=(const dbl_wrd &a, const dbl_wrd &b) { return (a.x[0] != b.x[0] || a.x[1] != b.x[1]); }

/* double != double-double */
inline bool operator!=(double a, const dbl_wrd &b) { return (a != b.x[0] || b.x[1] != 0.0); }

/*********** Micellaneous ************/
/*  this == 0 */
inline bool dbl_wrd::is_zero() const { return (x[0] == 0.0); }

/*  this == 1 */
inline bool dbl_wrd::is_one() const { return (x[0] == 1.0 && x[1] == 0.0); }

/*  this > 0 */
inline bool dbl_wrd::is_positive() const {
  // magnitude of x[0] is always greater than x[1]
  return (x[0] > 0.0);
}

/* this < 0 */
inline bool dbl_wrd::is_negative() const { return (x[0] < 0.0); }

inline dbl_wrd abs(const dbl_wrd &a) {
  /* Absolute value */
  return (a.x[0] < 0.0) ? -a : a;
}

inline dbl_wrd fabs(const dbl_wrd &a) {
  // This is calling std::abs.
  return abs(a);
}

inline dbl_wrd nint(const dbl_wrd &a) {
  /* Round to Nearest integer */
  double hi = dbl_wrd::_nint(a.x[0]);
  double lo;

  if (hi == a.x[0]) {
    /* High word is an integer already.  Round the low word.*/
    lo = dbl_wrd::_nint(a.x[1]);

    /* Renormalize. This is needed if x[0] = some integer, x[1] = 1/2. This will
     * cause lo to be an integer.*/
    hi = dbl_wrd::_fast_two_sum(hi, lo, lo);
  } else {
    /* High word, a.x[0], is not an integer. */
    lo = 0.0;
    if (std::abs(hi - a.x[0]) == 0.5 && a.x[1] < 0.0) {
      /* There is a tie in the high word, consult the low word
         to break the tie. */
      hi -= 1.0; /* NOTE: This does not cause INEXACT. */
    }
  }

  return dbl_wrd(hi, lo);
}

inline dbl_wrd floor(const dbl_wrd &a) {
  double hi = std::floor(a.x[0]);
  double lo = 0.0;

  if (hi == a.x[0]) {
    /* High word is integer already.  Round the low word. */
    lo = std::floor(a.x[1]);
    hi = dbl_wrd::_fast_two_sum(hi, lo, lo);
  }

  return dbl_wrd(hi, lo);
}

inline dbl_wrd ceil(const dbl_wrd &a) {
  double hi = std::ceil(a.x[0]);
  double lo = 0.0;

  if (hi == a.x[0]) {
    /* High word is integer already.  Round the low word. */
    lo = std::ceil(a.x[1]);
    hi = dbl_wrd::_fast_two_sum(hi, lo, lo); // Renormalize in the event that lo is an integer.
  }

  return dbl_wrd(hi, lo);
}

inline dbl_wrd aint(const dbl_wrd &a) {
  // This does round towards 0 when rounding a float to an integer.
  return (a.x[0] >= 0.0) ? floor(a) : ceil(a);
}

inline double to_double(const dbl_wrd &a) {
  /* Cast to double by ignoring the lo number. This number represents the rounding error to the nearest floating point number and that the number when
   * computed with infinite precision is not representatble*/
  return a.hi();
}

inline int to_int(const dbl_wrd &a) {
  /* Cast to int. */
  return static_cast<int>(a.hi());
}

inline dbl_wrd dbl_wrd::rand() {
  /* Random number generator */
  return ddrand();
}

void dbl_wrd::error(const char *msg) {
  /* This routine is called whenever a fatal error occurs. */
  if (msg) {
    cerr << "ERROR " << msg << endl;
  }
}

dbl_wrd sqrt(const dbl_wrd &a) {
  /* Computes the square root of the double-double number dd.
   * NOTE: a must be a non-negative number. We will not handle imaginary numbers.
   *
   * If x is an approximation
   * to sqrt(a), then $sqrt(a) = a*x + [a - (a*x)^2] * x / 2$.
   * This uses the Newton Raphson Method of Linear approximation with $1/x^{2} - a$ as the $f(x)$.
   * Each iteration of the Newton Raphson Method roughly doubles the number of corrrect decimal places. For example, if at iteration i, $x_i$ is
   * accurate to $10^{-5}$, then $x_{i+1}$ will accurate to $10^{-10}$. Reminder that it is approximately doubles.
   *
   * Karp and Markstein proved that the multiplication (a*x) and [-]*x can be done with
   * only half the precision. The result should be the same if it was with the corresponding
   * dbl_wrd operations.
   *
   * Reference:
   *  - High Precision Division and Square Root by Alan Karp and Peter Markstein.
   *  - Newton Raphson Method by Richard Anstee
   */

  if (a.is_zero())
    return 0.0;

  if (a.is_negative()) {
    dbl_wrd::error("(dbl_wrd::sqrt): Negative argument.");
    return dbl_wrd::_nan;
  }

  double x = 1.0 / std::sqrt(a.x[0]); // Initial approximation, which is correct to the double precision.
                                      // It takes one more iteration to get it accurate to the dbl_wrd precision.
  double ax = a.x[0] * x;             // This is equivalent to $a * (1/a^{1/2})$, which equals $a^{1/2}$.
  return dbl_wrd::add(ax, (a - dbl_wrd::square(ax)).x[0] * (x * 0.5));
}

dbl_wrd dbl_wrd::sqrt(double d) {
  /* Computes the square root of a double in double-double precision.
     NOTE: d must not be negative.                                   */
  return ::sqrt(dbl_wrd(d));
}

dbl_wrd nroot(const dbl_wrd &a, int n) {
  /* Computes the n-th root of the double-double number a.
   * NOTE: n must be a positive integer.
   * NOTE: If n is even, then a must not be negative.
   *
   * Use Newton iteration for the function $f(x) = x^(-n) - a$
   * to find its root, $a^{-1/n}$.  The iteration is thus $x_n = x_{n-1} + x_{n-1} * (1 - a * x_{n-1}^n) / n$.
   * We can then find $a^{1/n}$ by taking the reciprocal. Taking the reciprocal is faster than multiplying by $a^n$.
   * */

  if (n <= 0) {
    dbl_wrd::error("(dbl_wrd::nroot): N must be positive.");
    return dbl_wrd::_nan;
  }

  if (n % 2 == 0 && a.is_negative()) {
    dbl_wrd::error("(dbl_wrd::nroot): Negative argument.");
    return dbl_wrd::_nan;
  }

  if (n == 1) {
    return a;
  }
  if (n == 2) {
    return sqrt(a);
  }

  if (a.is_zero())
    return 0.0;

  /* Note  a^{-1/n} = exp(-log(a)/n) */
  dbl_wrd r = abs(a);
  dbl_wrd x = std::exp(-std::log(r.x[0]) / n); // We are casting a double into a dbl_wrd;
                                               // The lo variable is 0.

  /* Perform One step Newton's Raphson Method. */
  x += x * (1.0 - r * npwr(x, n)) / static_cast<double>(n);
  if (a.x[0] < 0.0)
    x = -x;
  return 1 / x;
}

dbl_wrd npwr(const dbl_wrd &a, int n) {
  /* Computes the n-th power of a double-double number.
   * NOTE:  0^0 causes an error.
   */

  if (n == 0) {
    if (a.is_zero()) {
      dbl_wrd::error("(dbl_wrd::npwr): Invalid argument. Parameter a is 0.");
      return dbl_wrd::_nan;
    }
    return 1.0;
  }

  dbl_wrd r = a;
  dbl_wrd s = 1.0;
  int N = std::abs(n);

  if (N > 1) {
    /* Use binary exponentiation */
    while (N > 0) {
      if (N % 2 == 1) {
        s *= r;
      }
      N /= 2; // N is int, so it cannot hold fraction.
      if (N > 0)
        r = sqr(r);
    }
  } else {
    s = r;
  }

  // Alternative binary exponentiation algorithm
  // Reference: https://cp-algorithms.com/algebra/binary-exp.html#algorithm
  //
  // while (N > 0) {
  //	if (N & 1)
  //		s = s * r;
  // r = sqr(r);
  // N >>= 1;
  //}

  /* Compute the reciprocal if n is negative. */
  if (n < 0)
    return (1.0 / s);

  return s;
}

dbl_wrd pow(const dbl_wrd &a, int n) {
  // Alias for npwr
  return npwr(a, n);
}

dbl_wrd pow(const dbl_wrd &a, const dbl_wrd &b) {
  // Computes a^b. log function is natural log and is defined below.
  return exp(b * log(a));
}

static const int n_inv_fact = 15;
static const double inv_fact[n_inv_fact][2] = {
    // Inverse factorial
    {1.66666666666666657e-01, 9.25185853854297066e-18},  {4.16666666666666644e-02, 2.31296463463574266e-18},  // 1/3! , 1/4!
    {8.33333333333333322e-03, 1.15648231731787138e-19},  {1.38888888888888894e-03, -5.30054395437357706e-20}, // 1/5!, 1/6!
    {1.98412698412698413e-04, 1.72095582934207053e-22},  {2.48015873015873016e-05, 2.15119478667758816e-23},  // 1/7!, 1/8!
    {2.75573192239858925e-06, -1.85839327404647208e-22}, {2.75573192239858883e-07, 2.37677146222502973e-23},  // 1/9!, 1/10!
    {2.50521083854417202e-08, -1.44881407093591197e-24}, {2.08767569878681002e-09, -1.20734505911325997e-25}, // 1/11!, 1/12!
    {1.60590438368216133e-10, 1.25852945887520981e-26},  {1.14707455977297245e-11, 2.06555127528307454e-28},  // 1/13!, 1/14!
    {7.64716373181981641e-13, 7.03872877733453001e-30},  {4.77947733238738525e-14, 4.39920548583408126e-31},  // 1/15!, 1/16!
    {2.81145725434552060e-15, 1.65088427308614326e-31}};                                                      // 1/17!

dbl_wrd exp(const dbl_wrd &a) {
  /* Exponential.  Computes exp(x) in double-double precision.
   *
   * Strategy:  We first reduce the size of x by noting that
   * $exp(kr + m * log(2)) = 2^m * exp(r)^k$
   * where m and k are integers.  By choosing m appropriately
   * we can make $abs(kr) <= log(2) / 2 = 0.347$.  Then $exp(r)$ is
   * evaluated using the familiar Taylor series.  Reducing the
   * argument substantially speeds up the convergence due to a property of the
   * Maclaurin/Taylor serie - The closer r is to 0, the faster the convergence.
   *
   * Reference:
   * Maclaurin series for computing $exp(x)$ for x in the set of all real numbers.
   * Library for Double-Double and Quad-Double Arithmetic by Hida, Li, and Bailey
   * (https://www.davidhbailey.com/dhbpapers/qd.pdf)
   */

  const double k = 512.0;
  const double inv_k = 1.0 / k;

  if (a.x[0] <= -709.0)
    return 0.0;

  if (a.x[0] >= 709.0)
    return dbl_wrd::_inf;

  if (a.is_zero())
    return 1.0;

  if (a.is_one())
    return dbl_wrd::_e;

  double m = std::floor(a.x[0] / dbl_wrd::_log2.x[0] + 0.5); // Round up if number is above .5
  dbl_wrd r = mul_pwr2(a - dbl_wrd::_log2 * m, inv_k);       // inv_k is $2^{-9}$.
  dbl_wrd s, t, p;

  // Start of the Maclaurin series approximation for exp(r).
  p = sqr(r);
  s = 1 + r + mul_pwr2(p, 0.5);
  p *= r;
  t = p * dbl_wrd(inv_fact[0][0], inv_fact[0][1]);
  int i = 0;
  do {
    s += t;
    p *= r;
    ++i;
    t = p * dbl_wrd(inv_fact[i][0], inv_fact[i][1]);
  } while (std::abs(to_double(t)) > inv_k * dbl_wrd::_eps && i < 5);

  s += t;

  // binary exponentiation to calculate $exp(r)^k$.
  // Since 2^9 = 512, we iterate 9 times.
  // TODO: This mul_pwr2(s, 2.0) is used to prevent underflowing.
  s = mul_pwr2(s, 2.0) + sqr(s);
  s = mul_pwr2(s, 2.0) + sqr(s);
  s = mul_pwr2(s, 2.0) + sqr(s);
  s = mul_pwr2(s, 2.0) + sqr(s);
  s = mul_pwr2(s, 2.0) + sqr(s);
  s = mul_pwr2(s, 2.0) + sqr(s);
  s = mul_pwr2(s, 2.0) + sqr(s);
  s = mul_pwr2(s, 2.0) + sqr(s);
  s = mul_pwr2(s, 2.0) + sqr(s);

  return ldexp(s, static_cast<int>(m));
}

dbl_wrd log(const dbl_wrd &a) {
  /* Logarithm.  Computes log(x) in double-double precision.
   * This is a natural logarithm (i.e., base e).
   * This is using the Newton Raphson method with x being the initial guess from std::log.
   * The choice of $f(x)$ is $e^{X} - a$, and the initial estimate of $log(a)$, $x$, is the most accurate
   * estimate for a double precision. That means that we only need 1 more interation to achieve dbl_wrd
   * accuracy.
   *
   *
   * */
  if (a.is_one()) {
    return 0.0;
  }

  if (a.x[0] <= 0.0) {
    dbl_wrd::error("(dbl_wrd::log): Non-positive argument.");
    return dbl_wrd::_nan;
  }

  dbl_wrd x = std::log(a.x[0]); /* Initial approximation of $log(a)$ */

  x = x + a * exp(-x) - 1.0; // One iteration of Newton-Raphson method.
  return x;
}

dbl_wrd log10(const dbl_wrd &a) {
  /* This is simply the change of base formula to convert log bases.*/
  return log(a) / dbl_wrd::_log10;
}

dbl_wrd fmod(const dbl_wrd &a, const dbl_wrd &b) {
  /*  Returns the remainder of a divided by b.
   *  Remainder is what is left over after the maximum number of b is divided out of a.
   * */
  dbl_wrd n = aint(a / b); // n is an integer.
  return (a - b * n);
}

dbl_wrd ddrand(double l_bound, double u_bound) {
  std::uniform_real_distribution<double> unif(l_bound, u_bound);
  std::default_random_engine re;
  dbl_wrd out = unif(re) + unif(re);
  return out;
}
