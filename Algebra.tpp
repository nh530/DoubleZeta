#include "DTypes.h"
#include <Python.h>
#include <cmath>
#include <iostream>
#include <type_traits>

#ifndef ALGEBRA_TPP
#define ALGEBRA_TPP

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
  out = *a / *b;
  // Py_END_ALLOW_THREADS
  return out;
}

template <Numeric Type> Type subtract(const Type *a, const Type *b) {
  Type out;
  // Py_BEGIN_ALLOW_THREADS
  out = *a - *b;
  // Py_END_ALLOW_THREADS
  return out;
}

template <Numeric Type> Type power(const Type *base, const Type *exp) {
  // This function can be used to calculate roots.
  if constexpr (is_numerc_variant<Type>::value) {
    try {
      float _base = std::get<float>(*exp);
      float _exp = std::get<float>(*base);
      return std::pow(*base, *exp);
    } catch (const std::bad_variant_access &ex) {
      int _base = std::get<int>(*exp);
      int _exp = std::get<int>(*base);
      return std::pow(*base, *exp);
    }
  }
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

template <Numeric T> T sin(const T *val) {
  if constexpr (is_numerc_variant<T>::value) {
    try {
      return std::sin(std::get<float>(*val));
    } catch (const std::bad_variant_access &ex) {
      return std::sin(std::get<int>(*val));
    }
  }
  return std::sin(*val);
}

template <Numeric T> T cos(const T *val) {
  if constexpr (is_numerc_variant<T>::value) {
    try {
      return std::cos(std::get<float>(*val));
    } catch (const std::bad_variant_access &ex) {
      return std::cos(std::get<int>(*val));
    }
  }
  return std::cos(*val);
}

template <Numeric T> T tan(const T *val) {
  if constexpr (is_numerc_variant<T>::value) {
    try {
      return std::tan(std::get<float>(*val));
    } catch (const std::bad_variant_access &ex) {
      return std::tan(std::get<int>(*val));
    }
  }
  return std::tan(*val);
}

template <Numeric T> T natrl_log(const T *val) {
  // Natural log (i.e. base e log)
  // The template constraints it to Numeric Type, but in actuality, float is always returned.
  if constexpr (is_numerc_variant<T>::value) {
    try {
      return std::log(std::get<float>(*val));
    } catch (const std::bad_variant_access &ex) {
      return std::log(std::get<int>(*val));
    }
  }
  return std::log(*val);
}

template <Numeric T> T base10_log(const T *val) {
  // Natural log (i.e. base e log)
  // The template constraints it to Numeric Type, but in actuality, float is always returned.
  if constexpr (is_numerc_variant<T>::value) {
    try {
      return std::log10(std::get<float>(*val));
    } catch (const std::bad_variant_access &ex) {
      return std::log10(std::get<int>(*val));
    }
  }
  return std::log10(*val);
}

template <Numeric Type> Type multiply_n(Type *data, int size_of) {
  int out{1};
  // Py_BEGIN_ALLOW_THREADS
  for (int i = 0; i != size_of; ++i)
    out *= data[i];
  // Py_END_ALLOW_THREADS
  return out;
}

template <Numeric T> T sum_n(T *data[], T *size_of) { // Pass by reference an array of ints that has unbound size.
  // TODO: Implement multithreading to sum.
  T out{0};
  int size;
  if constexpr (is_numerc_variant<T>::value) {
    try {
      int size{std::get<float>(*size_of)}; // down cast
    } catch (const std::bad_variant_access &ex) {
      int size{std::get<int>(*size_of)};
    }
  } else {
    int size{*size_of};
  }

  // Py_BEGIN_ALLOW_THREADS
  for (int i = 0; i != size; ++i)
    out += data[i];
  // Py_END_ALLOW_THREADS
  return out;
}

#endif
