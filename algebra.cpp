#include <Python.h>
#include <cmath>
#include <math.h>
#include <stdio.h>
#include <type_traits>

template <typename T>
concept Numeric = std::is_floating_point_v<T> || std::numeric_limits<T>::is_integer;

template <typename Type>
  requires Numeric<Type>
Type sum(Type a, Type b) {
  Type out;
  // Py_BEGIN_ALLOW_THREADS
  out = a + b;
  // Py_END_ALLOW_THREADS
  return out;
}

template <typename Type>
  requires Numeric<Type>
Type multiply(Type a, Type b) {
  Type out;
  // Py_BEGIN_ALLOW_THREADS
  out = a * b;
  // Py_END_ALLOW_THREADS
  return out;
}

template <typename Type>
  requires Numeric<Type>
Type divide(Type a, Type b) {
  Type out;
  // Py_BEGIN_ALLOW_THREADS
  out = a / b;
  // Py_END_ALLOW_THREADS
  return out;
}

template <typename Type>
  requires Numeric<Type>
Type subtract(Type a, Type b) {
  Type out;
  // Py_BEGIN_ALLOW_THREADS
  out = a - b;
  // Py_END_ALLOW_THREADS
  return out;
}

template <typename Type>
  requires Numeric<Type>
Type power(Type a, Type b) {
  Type out;
  // Py_BEGIN_ALLOW_THREADS
  out = a * *b;
  // Py_END_ALLOW_THREADS
  return out;
}

template <typename Type>
  requires Numeric<Type>
Type sum_n(Type *data, int size_of) { // Pass by reference an array of ints that has unbound size.
  // TODO: Implement multithreading to sum.
  int out{0};
  // Py_BEGIN_ALLOW_THREADS
  for (int i = 0; i != size_of; ++i)
    out += data[i];
  // Py_END_ALLOW_THREADS
  return out;
}

template <typename Type>
  requires Numeric<Type>
Type multiply_n(Type *data, int size_of) {
  int out{1};
  // Py_BEGIN_ALLOW_THREADS
  for (int i = 0; i != size_of; ++i)
    out *= data[i];
  // Py_END_ALLOW_THREADS
  return out;
}

template <typename Type>
  requires Numeric<Type>
Type *add(Type *data, int size_of, Type val) {
  // elementwise
  // Py_BEGIN_ALLOW_THREADS
  for (int i = 0; i != size_of; ++i) {
    data[i] += val;
  }
  // Py_END_ALLOW_THREADS
  return data;
}

template <typename Type>
  requires Numeric<Type>
Type *subtract(Type *data, int size_of, Type val) {
  // element-wise
  // Py_BEGIN_ALLOW_THREADS
  for (int i = 0; i != size_of; ++i) {
    data[i] -= val;
  }
  // Py_END_ALLOW_THREADS
  return data;
}

template <typename Type>
  requires Numeric<Type>
Type *multiply(Type *data, int size_of, Type val) {
  // element-wise.
  // Py_BEGIN_ALLOW_THREADS
  for (int i = 0; i != size_of; ++i)
    data[i] *= val;
  // Py_END_ALLOW_THREADS
  return data;
}

template <typename Type>
  requires Numeric<Type>
Type *divide(Type *data, int size_of, Type val) {
  // element-wise.
  // Py_BEGIN_ALLOW_THREADS
  for (int i = 0; i != size_of; ++i)
    data[i] /= val;
  // Py_END_ALLOW_THREADS
  return data;
}

template <typename Type>
  requires Numeric<Type>
Type *power(Type *data, int size_of, Type val) {
  // element-wise.
  // Py_BEGIN_ALLOW_THREADS
  for (int i = 0; i != size_of; ++i)
    data[i] = std::pow(data[i], val);
  // Py_END_ALLOW_THREADS
  return data;
}

int main() {
  int x[5]{1, 2, 3, 4, 5};
  sum_n(x, 5);
  int a{1};
  int b{2};
  sum(a, b);

  return 0;
}
