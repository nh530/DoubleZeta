#include <math.h>
#include <variant>
#ifndef DTYPES_H
#define DTYPES_H

// template <typename T>
// concept Numeric = std::is_floating_point_v<T> || std::numeric_limits<T>::is_integer;

template <typename T> constexpr bool is_float() { return std::is_floating_point_v<T>; }
template <typename T> constexpr bool is_int() { return std::numeric_limits<T>::is_integer; }

using NumericVariant = std::variant<float, int>;
template <typename T> struct is_numerc_variant : public std::false_type {};
template <> struct is_numerc_variant<NumericVariant> : public std::true_type {};

template <typename T>
concept int_or_float = is_float<T>() || is_int<T>();

template <typename T>
concept Numeric = is_float<T>() || is_int<T>() || is_numerc_variant<T>::value;

class MeanSquaredError;
class BinaryCrossEntropy;
// Defining what it means be a loss function user defined type.
template<typename T> struct is_loss_function : public std::false_type {};
template <> struct is_loss_function<MeanSquaredError> : public std::true_type {};
template <> struct is_loss_function<BinaryCrossEntropy> : public std::true_type {};

template <typename T>
concept LossFunction = is_loss_function<T>::value;

#endif
