#include "DTypes.h"
#include <Python.h>
#include <cmath>
#include <iostream>
#include <type_traits>

template <Numeric Type> Type sum(const Type *a, const Type *b) {
  Type out;
  // Py_BEGIN_ALLOW_THREADS
  out = *a + *b;
  // Py_END_ALLOW_THREADS
  return out;
}

template <Numeric Type> Type multiply(const Type *a, const Type *b) {
  Type out;
  // Py_BEGIN_ALLOW_THREADS
  out = *a * *b;
  // Py_END_ALLOW_THREADS
  return out;
}

template <Numeric Type> Type divide(const Type *a, const Type *b) {
  Type out;
  // Py_BEGIN_ALLOW_THREADS
  out = a / b;
  // Py_END_ALLOW_THREADS
  return out;
}

template <Numeric Type> Type subtract(const Type *a, const Type *b) {
  Type out;
  // Py_BEGIN_ALLOW_THREADS
  out = a - b;
  // Py_END_ALLOW_THREADS
  return out;
}

template <Numeric Type> Type power(const Type *a, const Type *b) {
  Type out;
  // Py_BEGIN_ALLOW_THREADS
  out = a * *b;
  // Py_END_ALLOW_THREADS
  return out;
}

template <Numeric Type> Type sum_n(Type *data, int size_of) { // Pass by reference an array of ints that has unbound size.
  // TODO: Implement multithreading to sum.
  int out{0};
  // Py_BEGIN_ALLOW_THREADS
  for (int i = 0; i != size_of; ++i)
    out += data[i];
  // Py_END_ALLOW_THREADS
  return out;
}

template <Numeric Type> Type multiply_n(Type *data, int size_of) {
  int out{1};
  // Py_BEGIN_ALLOW_THREADS
  for (int i = 0; i != size_of; ++i)
    out *= data[i];
  // Py_END_ALLOW_THREADS
  return out;
}

template <Numeric Type> Type *add(Type *data, int size_of, Type val) {
  // elementwise
  // Py_BEGIN_ALLOW_THREADS
  for (int i = 0; i != size_of; ++i) {
    data[i] += val;
  }
  // Py_END_ALLOW_THREADS
  return data;
}

template <Numeric Type> Type *subtract(Type *data, int size_of, Type val) {
  // element-wise
  // Py_BEGIN_ALLOW_THREADS
  for (int i = 0; i != size_of; ++i) {
    data[i] -= val;
  }
  // Py_END_ALLOW_THREADS
  return data;
}

template <Numeric Type> Type *multiply(Type *data, int size_of, Type val) {
  // element-wise.
  // Py_BEGIN_ALLOW_THREADS
  for (int i = 0; i != size_of; ++i)
    data[i] *= val;
  // Py_END_ALLOW_THREADS
  return data;
}

template <Numeric Type> Type *divide(Type *data, int size_of, Type val) {
  // element-wise.
  // Py_BEGIN_ALLOW_THREADS
  for (int i = 0; i != size_of; ++i)
    data[i] /= val;
  // Py_END_ALLOW_THREADS
  return data;
}

template <Numeric Type> Type *power(Type *data, int size_of, Type val) {
  // element-wise.
  // Py_BEGIN_ALLOW_THREADS
  for (int i = 0; i != size_of; ++i)
    data[i] = std::pow(data[i], val);
  // Py_END_ALLOW_THREADS
  return data;
}
