#include "typing/DTypes.h"

#ifndef ALGEBRA_H
#define ALGEBRA_H

template <Numeric T> T sin(const T *val);
template <Numeric T> T cos(const T *val);
template <Numeric T> T tan(const T *val);
template <Numeric T> T base10_log(const T *val);
template <Numeric T> T natrl_log(const T *val);
template <Numeric T> T add(const T *a, const T *b);
template <Numeric T> T multiply(const T *a, const T *b);
template <Numeric T> T divide(const T *a, const T *b);
template <Numeric T> T subtract(const T *a, const T *b);
template <Numeric T> T power(const T *a, const T *b);


//The following functions are out of scope.
template <Numeric T> T sum_n(const T data[], const T *size_of);
template <Numeric T> T multply_n(const T *data[], const T *size_of);
template <Numeric T> T *add(const T *data, const int *size_of, const T *val);
template <Numeric T> T *subject(const T *data, const int *size_of, const T *val);
template <Numeric T> T *multiply(const T *data, const int *size_of, const T *val);
template <Numeric T> T *divide(const T *data, const int *size_of, const T *val);
template <Numeric T> T *power(const T *data, const int *size_of, const T *val);

#include "Algebra.tpp"

#endif
