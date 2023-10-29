#ifndef DW_H
#define DW_H

#include "config/settings.h"
#include <cmath>
#include <double_word/fpu_fix.h>
#include <limits>
#include <string>

class dbl_wrd {
  // hi word is 53-bit representation
  // lo word is the actual - 53-bit representation.
  double x[2];

public:
  dbl_wrd(double hi, double lo) {
    x[0] = hi;
    x[1] = lo;
  }
  dbl_wrd() {
    x[0] = 0.0;
    x[1] = 0.0;
  }
  dbl_wrd(double h) {
    x[0] = h;
    x[1] = 0.0;
  }
  dbl_wrd(int h) {
    x[0] = (static_cast<double>(h));
    x[1] = 0.0;
  }

  dbl_wrd(const double *d) { // Construct dbl_wrd from an array of 2 doubles.
    x[0] = d[0];
    x[1] = d[1];
  }
  double hi() const { return x[0]; }
  double lo() const { return x[1]; }
  bool isnan() const { return ISNAN(x[0]) || ISNAN(x[1]); }
  bool isfinite() const { return ISFINITE(x[0]); }
  bool isinf() const { return ISINF(x[0]); }
  dbl_wrd &operator+=(double a);
  dbl_wrd &operator+=(const dbl_wrd &a);
  dbl_wrd &operator-=(double a);
  dbl_wrd &operator-=(const dbl_wrd &a);
  dbl_wrd operator-() const;
  dbl_wrd &operator*=(double a);
  dbl_wrd &operator*=(const dbl_wrd &a);
  dbl_wrd &operator/=(double a);
  dbl_wrd &operator/=(const dbl_wrd &a);
  dbl_wrd &operator=(double &a);
  dbl_wrd operator^(int n);
  static dbl_wrd square(double d); // square
  static dbl_wrd sqrt(double a);   // squareroot
  bool is_zero() const;
  bool is_one() const;
  bool is_positive() const;
  bool is_negative() const;
  static dbl_wrd rand(double l_bound, double u_bound);
  static const double get_eps();
  static const dbl_wrd get_max();
  static const double get_min();
  static const int get_digits();
  double get_hi() const;
  double get_lo() const;
  static const dbl_wrd _e;
  static const dbl_wrd _log2;
  static const dbl_wrd _log10;
  static const dbl_wrd _nan;
  static const dbl_wrd _inf;
  static const double _eps;
  static const double _min_normalized;
  static const dbl_wrd _max;
  static dbl_wrd accurate_add(const dbl_wrd &a, const dbl_wrd &b);
  static dbl_wrd sloppy_add(const dbl_wrd &a, const dbl_wrd &b);
  static dbl_wrd imp_mul(const dbl_wrd &a, double b);
  static dbl_wrd mul(double a, double b);
  static dbl_wrd mul(const dbl_wrd &a, double b);
  static dbl_wrd mul(const dbl_wrd &a, const dbl_wrd &b);
  static dbl_wrd add(double a, double b);
  static dbl_wrd sub(double a, double b);
  static dbl_wrd div(double a, double b);
  static dbl_wrd sloppy_div(const dbl_wrd &a, const dbl_wrd &b);
  static dbl_wrd accurate_div(const dbl_wrd &a, const dbl_wrd &b);
  static void error(const char *msg);

private:
  static double _fast_two_sum(double a, double b, double &err);
  static double _fast_two_diff(double a, double b, double &err);
  static double _two_sum(double a, double b, double &err);
  static double _two_diff(double a, double b, double &err);
  static double _two_prod(double a, double b, double &err);
  static double _two_square(double a, double &err);
  static double _nint(double d);
  static double _aint(double d);
  static double _square(double t);
  static int _to_int(double a);
  static const int _digits;
  friend dbl_wrd operator+(const dbl_wrd &a, double b);
  friend dbl_wrd operator+(double a, const dbl_wrd &b);
  friend dbl_wrd operator+(const dbl_wrd &a, const dbl_wrd &b);
  friend dbl_wrd operator-(const dbl_wrd &a, double b);
  friend dbl_wrd operator-(double a, const dbl_wrd &b);
  friend dbl_wrd operator-(const dbl_wrd &a, const dbl_wrd &b);
  friend dbl_wrd operator*(const dbl_wrd &a, double b);
  friend dbl_wrd operator*(double a, const dbl_wrd &b);
  friend dbl_wrd operator*(const dbl_wrd &a, const dbl_wrd &b);
  friend dbl_wrd operator/(const dbl_wrd &a, double b);
  friend dbl_wrd operator/(double a, const dbl_wrd &b);
  friend dbl_wrd operator/(const dbl_wrd &a, const dbl_wrd &b);
  friend bool operator==(const dbl_wrd &a, const double &b);
  friend bool operator==(const double &a, const dbl_wrd &b);
  friend bool operator==(const dbl_wrd &a, const dbl_wrd &b);
  friend bool operator<=(const dbl_wrd &a, double b);
  friend bool operator<=(double a, const dbl_wrd &b);
  friend bool operator<=(const dbl_wrd &a, const dbl_wrd &b);
  friend bool operator>=(const dbl_wrd &a, double b);
  friend bool operator>=(double a, const dbl_wrd &b);
  friend bool operator>=(const dbl_wrd &a, const dbl_wrd &b);

  friend bool operator<(const dbl_wrd &a, double b);
  friend bool operator<(double a, const dbl_wrd &b);
  friend bool operator<(const dbl_wrd &a, const dbl_wrd &b);

  friend bool operator>(const dbl_wrd &a, double b);
  friend bool operator>(double a, const dbl_wrd &b);
  friend bool operator>(const dbl_wrd &a, const dbl_wrd &b);

  friend bool operator!=(const dbl_wrd &a, double b);
  friend bool operator!=(double a, const dbl_wrd &b);
  friend bool operator!=(const dbl_wrd &a, const dbl_wrd &b);

  friend dbl_wrd mul_pwr2(const dbl_wrd &dd, double d);
  friend dbl_wrd log(const dbl_wrd &a);
  friend dbl_wrd exp(const dbl_wrd &a);
  friend dbl_wrd inv(const dbl_wrd &a); // computes the inverse of a number.
  friend dbl_wrd drem(const dbl_wrd &a, const dbl_wrd &b);
  friend dbl_wrd divrem(const dbl_wrd &a, const dbl_wrd &b, dbl_wrd &r);
  friend dbl_wrd pow(const dbl_wrd &a, int n);
  friend dbl_wrd pow(const dbl_wrd &a, const dbl_wrd &b);
  friend dbl_wrd npwr(const dbl_wrd &a, int n);
  friend dbl_wrd sqr(const dbl_wrd &a);  // square
  friend dbl_wrd sqrt(const dbl_wrd &a); // squareroot
  friend dbl_wrd nroot(const dbl_wrd &a, int n);
  friend dbl_wrd ldexp(const dbl_wrd &a, int exp);
  friend dbl_wrd nint(const dbl_wrd &a);
  friend dbl_wrd floor(const dbl_wrd &a);
  friend dbl_wrd ceil(const dbl_wrd &a);
  friend dbl_wrd aint(const dbl_wrd &a);
  friend dbl_wrd fabs(const dbl_wrd &a);
  friend dbl_wrd abs(const dbl_wrd &a);
};

namespace std {
// Adding the template dbl_wrd overload for numeric_limits class.
template <> class numeric_limits<dbl_wrd> : public numeric_limits<double> {
  inline static const int _digits = dbl_wrd::get_digits();

public:
  inline static double epsilon() { return dbl_wrd::get_eps(); }
  inline static dbl_wrd max() { return dbl_wrd::get_max(); }
  inline static dbl_wrd safe_max() { return dbl_wrd::get_max(); }
  inline static double min() { return dbl_wrd::get_min(); }
  inline static const int digits = 104;       // Mantisse length: 53 - 1 + 53 -1.
                                              // Ignoring the redundant bit.
  inline static const int digits10 = _digits; // Precision in base 10.
};
} // namespace std

// OPERATOR OVERLOAD

dbl_wrd operator+(const dbl_wrd &a, double b);
dbl_wrd operator+(double a, const dbl_wrd &b);
dbl_wrd operator+(const dbl_wrd &a, const dbl_wrd &b);

dbl_wrd operator-(const dbl_wrd &a, double b);
dbl_wrd operator-(double a, const dbl_wrd &b);
dbl_wrd operator-(const dbl_wrd &a, const dbl_wrd &b);

dbl_wrd operator*(const dbl_wrd &a, double b);
dbl_wrd operator*(double a, const dbl_wrd &b);
dbl_wrd operator*(const dbl_wrd &a, const dbl_wrd &b);

dbl_wrd operator/(const dbl_wrd &a, double b);
dbl_wrd operator/(double a, const dbl_wrd &b);
dbl_wrd operator/(const dbl_wrd &a, const dbl_wrd &b);

bool operator==(const dbl_wrd &a, const double &b);
bool operator==(const double &a, const dbl_wrd &b);
bool operator==(const dbl_wrd &a, const dbl_wrd &b);

bool operator<=(const dbl_wrd &a, double b);
bool operator<=(double a, const dbl_wrd &b);
bool operator<=(const dbl_wrd &a, const dbl_wrd &b);

bool operator>=(const dbl_wrd &a, double b);
bool operator>=(double a, const dbl_wrd &b);
bool operator>=(const dbl_wrd &a, const dbl_wrd &b);

bool operator<(const dbl_wrd &a, double b);
bool operator<(double a, const dbl_wrd &b);
bool operator<(const dbl_wrd &a, const dbl_wrd &b);

bool operator>(const dbl_wrd &a, double b);
bool operator>(double a, const dbl_wrd &b);
bool operator>(const dbl_wrd &a, const dbl_wrd &b);

bool operator!=(const dbl_wrd &a, double b);
bool operator!=(double a, const dbl_wrd &b);
bool operator!=(const dbl_wrd &a, const dbl_wrd &b);

// FUNCTION OVERLOADS

inline bool isnan(const dbl_wrd &a) { return a.isnan(); }
inline bool isfinite(const dbl_wrd &a) { return a.isfinite(); }
inline bool isinf(const dbl_wrd &a) { return a.isinf(); }

/* Computes  dd * d  where d is known to be a power of 2. */

dbl_wrd ddrand(double l_bound, double u_bound);

double to_double(const dbl_wrd &a);
int to_int(const dbl_wrd &a);

dbl_wrd fabs(const dbl_wrd &a);
dbl_wrd abs(const dbl_wrd &a); /* same as fabs */
dbl_wrd log10(const dbl_wrd &a);
dbl_wrd fmod(const dbl_wrd &a, const dbl_wrd &b);

void print(const dbl_wrd &a);

#endif
