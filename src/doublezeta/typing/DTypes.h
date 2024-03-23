#include <math.h>
#include <variant>
#ifndef DTYPES_H
#define DTYPES_H

// template <typename T>
// concept Numeric = std::is_floating_point_v<T> || std::numeric_limits<T>::is_integer;

template <typename T> constexpr bool is_float() {
  // float, double, long double, std::float16_t, ...
  return std::is_floating_point_v<T>;
}
template <typename T> constexpr bool is_int_like() {
  return std::numeric_limits<T>::is_integer & !std::is_same<T, bool>::value & !std::is_same<T, char>::value & !std::is_same<T, unsigned char>::value &
         !std::is_same<T, signed char>::value;
}
template <typename T> constexpr bool is_char_like() {
	return std::is_same<T, char>::value | std::is_same<T, unsigned char>::value | std::is_same<T, signed char>::value;
}

template <typename T> constexpr bool is_bool() { return std::is_same<T, bool>::value; }

using NumericVariant = std::variant<float, int>;
template <typename T> struct is_numerc_variant : public std::false_type {};
template <> struct is_numerc_variant<NumericVariant> : public std::true_type {};

template <typename T>
concept int_or_float = is_float<T>() || is_int_like<T>();

template <typename T>
concept Numeric = is_float<T>() || is_int_like<T>() || is_numerc_variant<T>::value;

template <typename T>
concept DataType = is_float<T>() || is_int_like<T>() || is_bool<T>(); // Specifies the "matrix data type".

class MeanSquaredError;
class BinaryCrossEntropy;
// Defining what it means be a loss function user defined type.
template <typename T> struct is_loss_function : public std::false_type {};
template <> struct is_loss_function<MeanSquaredError> : public std::true_type {};
template <> struct is_loss_function<BinaryCrossEntropy> : public std::true_type {};

template <typename T>
concept LossFunction = is_loss_function<T>::value;

#endif
