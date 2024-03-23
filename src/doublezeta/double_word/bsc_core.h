/*
 * This file contains the basic functions used both by the double-word class
 * These are declared as inline functions as
 * they are the smallest building blocks of the double-double.
 * References:
 *  - Handbook of floating point arithmetic by Jean-Michel Muller, et al. Chapter 4.
 *  - Tight and Rigorous error bounds for basic building blocks of double-word arithmetic by Joldes, Muller, and Popescu
 */
#ifndef BSC_CORE_H
#define BSC_CORE_H

#include "config/settings.h"
#include <cmath>
#include <limits>

namespace basic_core {

static const double _dbl_nan = std::numeric_limits<double>::quiet_NaN();
static const double _dbl_inf = std::numeric_limits<double>::infinity();

inline double fast_two_sum(double a, double b, double &err) {
  /* Computes s and err such that s + err = a + b.  Assumes |a| >= |b|.
   * s = RN(a + b)
   * Dekker and Kahan algorithm.
   * */
  double s = a + b;
  err = b - (s - a);
  return s;
}

inline double _fast_two_diff(double a, double b, double &err) {
  /* DEPRECATED
   * Computes s and err such that s + err = a - b. Assumes |a| >= |b|
   * s = RN(a - b)
   * Dekker and Kahan algorithm.
   *
   * Error bound same as fast_two_sum.
   * */
  double s = a - b;
  err = (a - s) - b;
  return s;
}

inline double fast_two_diff(double a, double b, double &err) {
  /* Computes s and err such that s + err = a - b. Assumes |a| >= |b|
   * s = RN(a - b)
   * Dekker and Kahan algorithm.
   *
   * Error bound same as fast_two_sum.
   * */
  return fast_two_sum(a, -b, err);
}

inline double two_sum(double a, double b, double &err) {
  /* Computes float(a+b) and err(a+b).
   * Knuth and Moller algorithm.
   * */
  double s = a + b;
  double bb = s - a;
  err = (a - (s - bb)) + (b - bb);
  return s;
}

inline double _two_diff(double a, double b, double &err) {
  /* DEPRECATED
   * Computes float(a-b) and err(a+b).
   * Knuth and Moller algorithm
   * */
  double s = a - b;
  double bb = s - a;
  err = (a - (s - bb)) - (b + bb);
  return s;
}

inline double two_diff(double a, double b, double &err) {
  /* Computes float(a-b) and err(a+b).
   * Knuth and Moller algorithm
   * */
  return two_sum(a, -b, err);
}

inline void split(double a, double &hi, double &lo) {
  /* If there is no Fused multiply add function, this split function is used intead.
   * Computes high word and lo word of double a
   * */
  double temp;
  if (a > _SPLIT_THRESH || a < -_SPLIT_THRESH) {
    a *= 3.7252902984619140625e-09; // 2^-28
    temp = _SPLITTER * a;
    hi = temp - (temp - a);
    lo = a - hi;
    hi *= 268435456.0; // 2^28
    lo *= 268435456.0; // 2^28
  } else {
    temp = _SPLITTER * a;
    hi = temp - (temp - a);
    lo = a - hi;
  }
}

inline double two_prod(double a, double b, double &err) {
/* Computes fl(a*b) and err(a*b). */
/* if fma supported by hardware, the appropriate FP_FAST_FMA macros are expected to be defined,
 * but many implementations make use of the CPU instruction even when the macros are not defined.
 */
#ifdef __FMA__
  double p = a * b;
  // Note that fma function still works even if there is no fma hardware, but really slowly.
  err = fma(a, b, -p);
  return p;
#else
  double a_hi, a_lo, b_hi, b_lo;
  double p = a * b;
  split(a, a_hi, a_lo);
  split(b, b_hi, b_lo);
  err = ((a_hi * b_hi - p) + a_hi * b_lo + a_lo * b_hi) + a_lo * b_lo;
  return p;
#endif
}

inline double two_square(double a, double &err) {
/* Computes fl(a*a) and err(a*a).  Faster than the above method in the non fma calculation. */
#ifdef __FMA__
  double p = a * a;
  err = fma(a, a, -p);
  return p;
#else
  double hi, lo;
  double q = a * a;
  split(a, hi, lo);
  err = ((hi * hi - q) + 2.0 * hi * lo) + lo * lo;
  return q;
#endif
}

inline double nint(double d) {
  /* Computes the nearest integer to d.
   * The double that is returned is equal to an integer that is of type double.
   *
   * Example:
   *
   * 20.1 + .5 -> 20.000
   * 20.7 + .5 -> 21.000
   * */

  if (d == std::floor(d)) { // already int
    return d;
  } else {
    return std::floor(d + 0.5);
  }
}

inline double aint(double d) {
  /* Computes the truncated integer. It does not compute the nearest integer.
   * It always rounds down, which is equivalent to truncating.
   *
   * Example:
   * 20.123 -> 20.00
   * 55.8921 -> 55.00
   * */
  return (d <= 0.0) ? std::ceil(d) : std::floor(d);
}

inline double square(double t) { return t * t; }

inline double to_double(double a) { return a; }
inline int to_int(double a) { return static_cast<int>(a); }

} // namespace basic_core

#endif /* BSC_CORE_H */
