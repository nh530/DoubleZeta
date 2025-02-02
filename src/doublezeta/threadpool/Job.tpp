#include <future>
#include <iostream>
#include <stdexcept>
#ifndef JOB_TPP
#define JOB_TPP

#include "Job.h"
#include "typing/DTypes.h"

template <Numeric T>
inline Job<T>::Job(T (*func_ptr)(const T *), T *arg1)
    : _func_ptr{new std::packaged_task<T(const T *)>(*func_ptr)}, _args{new T{*arg1}}, _args2{NULL} {
  // int or float Job constructor for 1 parameter functions.
  // _args and _args2 are allocated on Heap memory and the value stored in *arg1 address is copied.
  // This only overloads int and float types.
}
template <Numeric T> inline Job<T>::Job(T (*func_ptr)()) : _func_ptr_no_args{new std::packaged_task<T()>(*func_ptr)}, _args{NULL}, _args2{NULL} {
  // int or float constructor for no parameter functions.
  // _args and _args2 are allocated on Heap memory and the value stored in *arg1 address is copied.
  // This only overloads int and float types.
}
template <Numeric T> inline Job<T>::Job(T (*func_ptr)(const T *, const T *), T *arg1, T *arg2) : _args{new T{*arg1}}, _args2{new T{*arg2}} {
  // Constructor for 2 parameter int or float functions.
  // _args and _args2 are allocated on Heap memory and the value stored in *arg1 address is copied.
  // This only overloads int and float types.
  _func_ptr_2_args = new std::packaged_task<T(const T *, const T *)>([func_ptr](const T *x, const T *y) -> T {
    T t_x = *x;
    T t_y = *y;
    return (func_ptr(&t_x, &t_y));
  });
}
template <Numeric T> inline Job<T>::Job() : _args{NULL}, _args2{NULL}, _func_ptr_no_args{NULL}, _func_ptr{NULL}, _func_ptr_2_args{NULL} {
  // Default constructor for int and float template types.
  // This only overloads int and float types.
}
template <Numeric T> inline void Job<T>::Run() {
  // Executes function and pass in arguments.
  // Null args should silently Run and do nothing.
  // This only overloads int and float types.
  if (_func_ptr_2_args) {
    (*_func_ptr_2_args)(_args, _args2);
  } else if (_func_ptr) {
    (*_func_ptr)(_args);
  } else if (_func_ptr_no_args)
    (*_func_ptr_no_args)();
}

template <Numeric T> inline std::future<T> Job<T>::GetFuture() {
  /* Returns a future object associated with this Job instance.
   * This only overloads int and float types.
   * */
  if (_func_ptr_2_args) {
    return _func_ptr_2_args->get_future();
  } else if (_func_ptr) {
    return _func_ptr->get_future();
  } else if (_func_ptr_no_args)
    return _func_ptr_no_args->get_future();
  else
    throw std::runtime_error("Error! Cannot get std::future<T> object because there is no function to be executed.");
}

template <Numeric N> inline Job<N>::~Job() {
  // Destructor
  // This only overloads int and float types.
  delete _args;
  delete _args2;
  // Don't need to release packaged_task pointers. Forgot why
}

template <Numeric T> inline Job<T>::Job(Job<T> &&other) {
  // Move constructor.
  // This only overloads int and float types.
  if (other._args != NULL)
    _args = new T{*other._args};
  else
    _args = NULL;
  if (other._args2 != NULL)
    _args2 = new T{*other._args2};
  else
    _args2 = NULL;
  if (other._func_ptr != NULL)
    _func_ptr = std::move(other._func_ptr);
  else
    _func_ptr = NULL;
  if (other._func_ptr_no_args != NULL)
    _func_ptr_no_args = std::move(other._func_ptr_no_args);
  else
    _func_ptr_no_args = NULL;
  if (other._func_ptr_2_args != NULL)
    _func_ptr_2_args = std::move(other._func_ptr_2_args);
  else
    _func_ptr_2_args = NULL;

  other._func_ptr = NULL;
  other._func_ptr_2_args = NULL;
  other._func_ptr_no_args = NULL;
}
template <Numeric T> inline Job<T> &Job<T>::operator=(Job<T> &&other) {
  /* Move assignment
   * This only overloads int and float types.
   * */
  delete _args;
  delete _args2;
  if (other._args != NULL)
    _args = new T{*other._args};
  else
    _args = NULL;
  if (other._args2 != NULL)
    _args2 = new T{*other._args2};
  else
    _args2 = NULL;
  if (other._func_ptr != NULL)
    _func_ptr = std::move(other._func_ptr);
  else
    _func_ptr = NULL;
  if (other._func_ptr_no_args != NULL)
    _func_ptr_no_args = std::move(other._func_ptr_no_args);
  else
    _func_ptr_no_args = NULL;
  if (other._func_ptr_2_args != NULL)
    _func_ptr_2_args = std::move(other._func_ptr_2_args);
  else
    _func_ptr_2_args = NULL;

  other._func_ptr = NULL;
  other._func_ptr_2_args = NULL;
  other._func_ptr_no_args = NULL;

  return *this;
}

template <Numeric T> inline T Job<T>::GetArg1() {
  // 0 is returned if there is no value for the first parameter of the Job.
  // The 0 gets casted.
  // This only overloads int and float types.
  if (_args)
    return *_args;
  else
    return 0;
}

template <Numeric T> inline T Job<T>::GetArg2() {
  // 0 is returned if there is no value for the second parameter of the Job.
  // The 0 gets casted to int or float.
  // This only overloads int and float types.
  if (_args2)
    return *_args2;
  else
    return 0;
}

template <Numeric T> inline void Job<T>::GetFuture(std::future<T> &out) {
  /* Returns a future object associated with this Job instance.
   * This only overloads int and float types.
   * */
  if (_func_ptr_2_args) {
    out = _func_ptr_2_args->get_future();
  } else if (_func_ptr) {
    out = _func_ptr->get_future();
  } else if (_func_ptr_no_args)
    out = _func_ptr_no_args->get_future();
  else
    throw std::runtime_error("Error! Cannot get std::future<T> object because there is no function to be executed.");
}

template <Numeric T> inline JobNoParam<T>::JobNoParam(T (*func_ptr)()) : _func_ptr_no_args{new std::packaged_task<T()>(*func_ptr)} {
  // int or float constructor for no parameter functions.
}
template <Numeric T> inline JobNoParam<T>::JobNoParam() : _func_ptr_no_args{NULL} {
  // Default constructor for int and float template types.
}
template <Numeric T> inline void JobNoParam<T>::Run() {
  // Executes function and pass in arguments.
  // Null args should silently Run and do nothing.
  if (_func_ptr_no_args)
    (*_func_ptr_no_args)();
}

template <Numeric T> inline std::future<T> JobNoParam<T>::GetFuture() {
  /* Returns a future object associated with this Job instance.
   * */
  if (_func_ptr_no_args)
    return _func_ptr_no_args->get_future();
  else
    throw std::runtime_error("Error! Cannot get std::future<T> object because there is no function to be executed.");
}

template <Numeric T> inline void JobNoParam<T>::GetFuture(std::future<T> &out) {
  /* Returns a future object associated with this Job instance.
   * */
  if (_func_ptr_no_args)
    out = _func_ptr_no_args->get_future();
  else
    throw std::runtime_error("Error! Cannot get std::future<T> object because there is no function to be executed.");
}

template <Numeric N> inline JobNoParam<N>::~JobNoParam() {
  // Destructor
  // Don't need to release packaged_task pointers. Forgot why
}

template <Numeric T> inline JobNoParam<T>::JobNoParam(JobNoParam<T> &&other) {
  // Move constructor.
  if (other._func_ptr_no_args != NULL)
    _func_ptr_no_args = std::move(other._func_ptr_no_args);
  else
    _func_ptr_no_args = NULL;

  other._func_ptr_no_args = NULL;
}

template <Numeric T> inline JobNoParam<T> &JobNoParam<T>::operator=(JobNoParam<T> &&other) {
  /* Move assignment
   * */
  if (other._func_ptr_no_args != NULL)
    _func_ptr_no_args = std::move(other._func_ptr_no_args);
  else
    _func_ptr_no_args = NULL;

  other._func_ptr_no_args = NULL;
  return *this;
}

template <Numeric T> inline JobOneParam<T>::JobOneParam() : _func_ptr{NULL}, _args{NULL} {}

template <Numeric T>
inline JobOneParam<T>::JobOneParam(T (*func_ptr)(const T *), T arg1)
    : _func_ptr{new std::packaged_task<T(const T *)>(func_ptr)}, _args{new T{arg1}} {}

template <Numeric T> inline JobOneParam<T>::JobOneParam(T (*func_ptr)(const T &), T arg1) : _args{new T{arg1}} {
  _func_ptr = new std::packaged_task<T(const T *)>([func_ptr](const T *x) -> T { return (func_ptr(*x)); });
}

template <Numeric T> inline JobOneParam<T>::JobOneParam(T (*func_ptr)(const T), T arg1) : _args{new T{arg1}} {
  _func_ptr = new std::packaged_task<T(const T *)>([func_ptr](const T *x) -> T { return (func_ptr(*x)); });
}

template <Numeric T> inline JobOneParam<T>::JobOneParam(JobOneParam<T> &&other) {
  // Move constructor.
  if (other._func_ptr)
    _func_ptr = std::move(other._func_ptr);
  else
    _func_ptr = NULL;

  if (other._args)
    _args = other._args;
  else
    _args = NULL;

  other._func_ptr = NULL;
  other._args = NULL;
}

template <Numeric T> inline JobOneParam<T> &JobOneParam<T>::operator=(JobOneParam<T> &&other) {
  // Move assignment;
  delete _args;
  if (other._func_ptr)
    _func_ptr = std::move(other._func_ptr);
  else
    _func_ptr = NULL;

  if (other._args)
    _args = other._args;
  else
    _args = NULL;

  other._func_ptr = NULL;
  other._args = NULL;
  return *this;
}

template <Numeric T> inline JobOneParam<T>::~JobOneParam() {
  // destructor
  delete _args;
}

template <Numeric T> inline void JobOneParam<T>::Run() {
  if (_func_ptr)
    (*_func_ptr)(_args);
}

template <Numeric T> inline std::future<T> JobOneParam<T>::GetFuture() {
  if (_func_ptr)
    return _func_ptr->get_future();
  else
    throw std::runtime_error("Error! Cannot get std::future<T> object because there is no function to be executed.");
}

template <Numeric T> inline void JobOneParam<T>::GetFuture(std::future<T> &out) {
  /* Returns a future object associated with this Job instance.
   * */
  if (_func_ptr)
    out = _func_ptr->get_future();
  else
    throw std::runtime_error("Error! Cannot get std::future<T> object because there is no function to be executed.");
}

template <Numeric T> inline T JobOneParam<T>::GetArg1() {
  if (_args)
    return (*_args);
  else
    return 0;
}

template <Numeric T> inline JobTwoParam<T>::JobTwoParam() : _args{NULL}, _args2{NULL}, _func_ptr_2_args{NULL} {}

template <Numeric T>
inline JobTwoParam<T>::JobTwoParam(T (*func_ptr)(const T *, const T *), T arg1, T arg2)
    : _args{new T{arg1}}, _args2{new T{arg2}}, _func_ptr_2_args{new std::packaged_task<T(const T *, const T *)>(func_ptr)} {}

template <Numeric T>
inline JobTwoParam<T>::JobTwoParam(T (*func_ptr)(const T &, const T &), T arg1, T arg2) : _args{new T{arg1}}, _args2{new T{arg2}} {
  _func_ptr_2_args = new std::packaged_task<T(const T *, const T *)>([func_ptr](const T *x, const T *y) -> T { return (func_ptr(*x, *y)); });
}

template <Numeric T> inline JobTwoParam<T>::JobTwoParam(T (*func_ptr)(const T, const T), T arg1, T arg2) : _args{new T{arg1}}, _args2{new T{arg2}} {
  _func_ptr_2_args = new std::packaged_task<T(const T *, const T *)>([func_ptr](const T *x, const T *y) -> T { return (func_ptr(*x, *y)); });
}

template <Numeric T> inline JobTwoParam<T>::JobTwoParam(JobTwoParam<T> &&other) {
  if (other._func_ptr_2_args)
    _func_ptr_2_args = other._func_ptr_2_args;
  else
    _func_ptr_2_args = NULL;
  if (other._args)
    _args = other._args;
  else
    _args = NULL;

  if (other._args2)
    _args2 = other._args2;
  else
    _args2 = NULL;

  other._func_ptr_2_args = NULL;
  other._args = NULL;
  other._args2 = NULL;
}

template <Numeric T> inline JobTwoParam<T>::~JobTwoParam() {
  delete _args;
  delete _args2;
}

template <Numeric T> inline JobTwoParam<T> &JobTwoParam<T>::operator=(JobTwoParam<T> &&other) {
  delete _args;
  delete _args2;

  if (other._func_ptr_2_args)
    _func_ptr_2_args = other._func_ptr_2_args;
  else
    _func_ptr_2_args = NULL;
  if (other._args)
    _args = other._args;
  else
    _args = NULL;
  if (other._args2)
    _args2 = other._args2;
  else
    _args2 = NULL;

  other._func_ptr_2_args = NULL;
  other._args = NULL;
  other._args2 = NULL;
  return *this;
}

template <Numeric T> inline void JobTwoParam<T>::Run() {
  if (_func_ptr_2_args)
    (*_func_ptr_2_args)(_args, _args2);
}

template <Numeric T> inline std::future<T> JobTwoParam<T>::GetFuture() {
  if (_func_ptr_2_args)
    return _func_ptr_2_args->get_future();
  else
    throw std::runtime_error("Error! Cannot get std::future<T> object because there is no function to be executed.");
}

template <Numeric T> inline void JobTwoParam<T>::GetFuture(std::future<T> &out) {
  /* Returns a future object associated with this Job instance.
   * */
  if (_func_ptr_2_args)
    out = _func_ptr_2_args->get_future();
  else
    throw std::runtime_error("Error! Cannot get std::future<T> object because there is no function to be executed.");
}

template <Numeric T> inline T JobTwoParam<T>::GetArg1() {
  if (_args)
    return (*_args);
  else
    return 0;
}

template <Numeric T> inline T JobTwoParam<T>::GetArg2() {
  if (_args2)
    return (*_args2);
  else
    return 0;
}

inline NumericVariant Job<NumericVariant>::GetArg1() {
  // 0 is returned if there is no value for the first parameter of the Job.
  // The 0 gets casted.
  if (_args)
    return *_args;
  else
    return 0;
}

inline NumericVariant Job<NumericVariant>::GetArg2() {
  // 0 is returned if there is no value for the second parameter of the Job.
  // The 0 gets casted no NumericVariant.
  if (_args2) {
    return *_args2;
  } else if (_args) {
    if (std::holds_alternative<int>(*_args)) {
      return 0; // int variant
    } else if (std::holds_alternative<float>(*_args)) {
      return 0.0f; // float variant
    } else {
      return 0; // int variant; return int 0 if _args pointer is null.
    }
  } else {
    // Returns int 0 if null pointer if _args2 pointer is null.
    return 0; // int variant
  }
}

inline Job<NumericVariant>::Job(NumericVariant (*func_ptr)(const NumericVariant *), NumericVariant *arg1)
    : _func_ptr{new std::packaged_task<NumericVariant(const NumericVariant *)>(*func_ptr)}, _args{new NumericVariant{*arg1}}, _args2{NULL} {
  /* Job constructor for NumericVariant type, 1 parameter functions.
   * _args and _args2 are allocated on Heap memory and the value stored in *arg1 address is copied.
   * */
}
inline Job<NumericVariant>::Job(NumericVariant (*func_ptr)())
    : _func_ptr_no_args{new std::packaged_task<NumericVariant()>(*func_ptr)}, _args{NULL}, _args2{NULL} {
  /* Job constructor for NumericVariant type, 0 parameter functions.
   * _args and _args2 are allocated on Heap memory and the value stored in *arg1 address is copied.
   * */
}
inline Job<NumericVariant>::Job(NumericVariant (*func_ptr)(const NumericVariant *, const NumericVariant *), NumericVariant *arg1,
                                NumericVariant *arg2)
    : _func_ptr_2_args{new std::packaged_task<NumericVariant(const NumericVariant *, const NumericVariant *)>(*func_ptr)},
      _args{new NumericVariant{*arg1}}, _args2{new NumericVariant{*arg2}} {
  /* Job constructor for NumericVariant type, 2 parameter functions.
   * _args and _args2 are allocated on Heap memory and the value stored in *arg1 address is copied.
   * */
}
inline Job<NumericVariant>::Job() : _args{NULL}, _args2{NULL}, _func_ptr_no_args{NULL}, _func_ptr{NULL}, _func_ptr_2_args{NULL} {
  // Default constructor for NumericVariant type Job.
}

template <int_or_float T> inline Job<NumericVariant>::Job(T (*func_ptr)()) : _args{NULL}, _args2{NULL} {
  /* Job constructor for 0 parameter function. Changing the function signature to be NumericVariant.
   * Enables compatibility with int or float functions.
   * */
  // Handle the conversion from Job<float> to Job<NumericVariant> by creating a new function that outputs NumericVariant.
  _func_ptr_no_args = new std::packaged_task<NumericVariant()>([func_ptr]() -> NumericVariant {
    return (func_ptr()); // Output gets casted or wrapped by std::variant<float, int>
  });
}

template <int_or_float T> inline Job<NumericVariant>::Job(T (*func_ptr)(const T *), T *arg1) : _args2{NULL} {
  /* Job constructor for 1 parameter function. Changing the function signature to be NumericVariant.
   * Enables compatibility with int or float functions.
   * */
  _func_ptr = new std::packaged_task<NumericVariant(const NumericVariant *)>([func_ptr](const NumericVariant *x) -> NumericVariant {
    T temp = std::get<T>(*x);
    return (func_ptr(&temp));
  });
  _args = new NumericVariant{*arg1};
}

template <int_or_float T> inline Job<NumericVariant>::Job(T (*func_ptr)(const T), T arg1) : _args2{NULL} {
  /* Job constructor for 1 parameter function. Changing the function signature to be NumericVariant.
   * Enables compatibility with int or float functions.
   * */
  _func_ptr = new std::packaged_task<NumericVariant(const T)>([func_ptr](const NumericVariant *x) -> NumericVariant {
    T temp = std::get<T>(*x);
    return (func_ptr(temp));
  });
  _args = new NumericVariant{arg1};
}

template <int_or_float T> inline Job<NumericVariant>::Job(T (*func_ptr)(const T *, const T *), T *arg1, T *arg2) {
  /* Job constructor for 2 parameter function. Changing the function signature to be NumericVariant.
   * Enables compatibility with int or float functions.
   * */
  _func_ptr_2_args = new std::packaged_task<NumericVariant(const NumericVariant *, const NumericVariant *)>(
      [func_ptr](const NumericVariant *x, const NumericVariant *y) -> NumericVariant {
        T t_x = std::get<T>(*x);
        T t_y = std::get<T>(*y);
        return (func_ptr(&t_x, &t_y));
      });
  _args = new NumericVariant{*arg1};
  _args2 = new NumericVariant{*arg2};
}

template <int_or_float T> inline Job<NumericVariant>::Job(T (*func_ptr)(const T, const T), T arg1, T arg2) {
  /* Job constructor for 2 parameter function. Changing the function signature to be NumericVariant.
   * */
  _func_ptr_2_args =
      new std::packaged_task<NumericVariant(const T, const T)>([func_ptr](const NumericVariant *x, const NumericVariant *y) -> NumericVariant {
        T t_x = std::get<T>(*x);
        T t_y = std::get<T>(*y);
        return (func_ptr(t_x, t_y));
      });
  _args = new NumericVariant{arg1};
  _args2 = new NumericVariant{arg2};
}

inline Job<NumericVariant>::Job(Job<NumericVariant> &&other) {
  // Move constructor.
  if (other._args != NULL)
    _args = new NumericVariant{*other._args};
  else
    _args = NULL;
  if (other._args2 != NULL)
    _args2 = new NumericVariant{*other._args2};
  else
    _args2 = NULL;
  if (other._func_ptr != NULL)
    _func_ptr = std::move(other._func_ptr);
  else
    _func_ptr = NULL;
  if (other._func_ptr_no_args != NULL)
    _func_ptr_no_args = std::move(other._func_ptr_no_args);
  else
    _func_ptr_no_args = NULL;
  if (other._func_ptr_2_args != NULL)
    _func_ptr_2_args = std::move(other._func_ptr_2_args);
  else
    _func_ptr_2_args = NULL;

  other._func_ptr = NULL;
  other._func_ptr_2_args = NULL;
  other._func_ptr_no_args = NULL;
}

inline Job<NumericVariant> &Job<NumericVariant>::operator=(Job<NumericVariant> &&other) {
  /* Move assignment
   * */
  delete _args;
  delete _args2;
  if (other._args != NULL)
    _args = new NumericVariant{*other._args};
  else
    _args = NULL;
  if (other._args2 != NULL)
    _args2 = new NumericVariant{*other._args2};
  else
    _args2 = NULL;
  if (other._func_ptr != NULL)
    _func_ptr = std::move(other._func_ptr);
  else
    _func_ptr = NULL;
  if (other._func_ptr_no_args != NULL)
    _func_ptr_no_args = std::move(other._func_ptr_no_args);
  else
    _func_ptr_no_args = NULL;
  if (other._func_ptr_2_args != NULL)
    _func_ptr_2_args = std::move(other._func_ptr_2_args);
  else
    _func_ptr_2_args = NULL;

  other._func_ptr = NULL;
  other._func_ptr_2_args = NULL;
  other._func_ptr_no_args = NULL;

  return *this;
}

inline Job<NumericVariant>::~Job() {
  delete _args;
  delete _args2;
  // Don't need to release packaged_task pointers. Forgot why.
}

inline void Job<NumericVariant>::Run() {
  // Executes function and pass in arguments.
  if (_func_ptr_2_args) {
    (*_func_ptr_2_args)(_args, _args2);
  } else if (_func_ptr) {
    (*_func_ptr)(_args);
  } else if (_func_ptr_no_args)
    (*_func_ptr_no_args)();
}

inline std::future<NumericVariant> Job<NumericVariant>::GetFuture() {
  /* Returns a future object associated with this Job instance.
   * */
  if (_func_ptr_2_args) {
    return _func_ptr_2_args->get_future();
  } else if (_func_ptr) {
    return _func_ptr->get_future();
  } else if (_func_ptr_no_args)
    return _func_ptr_no_args->get_future();
  else
    throw std::runtime_error("Error! Cannot get std::future<T> object because there is no function to be executed.");
}

template <int_or_float N>
inline JobArr<N>::JobArr(N (*func_ptr)(const N *, const int *), N *arg1, const int _len_args1)
    : _args{arg1}, _args2{NULL}, _len1{_len_args1}, _len2{-1}, _func_ptr{new std::packaged_task<N(const N *, const int *)>(*func_ptr)} {
  // constructor for functions with array parameter.
  // _args and _args2 are allocated on Heap memory and the value stored in *arg1 address is copied.
  // _len1 is allocated on the stack. -1 is sentinal value for Null.
}

template <int_or_float N>
inline JobArr<N>::JobArr(N *(*func_ptr)(const N *, const int *), N *arg1, const int _len_args1)
    : _args{arg1}, _args2{NULL}, _len1{_len_args1}, _len2{-1}, _func_ptr_arr{new std::packaged_task<N *(const N *, const int *)>(*func_ptr)} {
  // constructor for functions with array parameter.
  // _args and _args2 are allocated on Heap memory and the value stored in *arg1 address is copied.
  // _len1 is allocated on the stack. -1 is sentinal value for Null.
}

template <int_or_float N>
inline JobArr<N>::JobArr(N (*func_ptr)(const N *, const N *, const int *, const int *), N *arg1, N *arg2, const int _len_args1, const int _len_args2)
    : _args{arg1}, _args2{arg2}, _len1{_len_args1}, _len2{_len_args2},
      _func_ptr_2_args{new std::packaged_task<N(const N *, const N *, const int *, const int *)>(*func_ptr)} {
  // constructor for functions with array parameter.
  // _args and _args2 are allocated on Heap memory and the value stored in *arg1 address is copied.
  // _len1 is allocated on the stack.
}

template <int_or_float N>
inline JobArr<N>::JobArr(N *(*func_ptr)(const N *, const N *, const int *, const int *), N *arg1, N *arg2, const int _len_args1, const int _len_args2)
    : _args{arg1}, _args2{arg2}, _len1{_len_args1}, _len2{_len_args2},
      _func_ptr_2_args_arr{new std::packaged_task<N *(const N *, const N *, const int *, const int *)>(*func_ptr)} {
  // constructor for functions with array parameter.
  // _args and _args2 are allocated on Heap memory and the value stored in *arg1 address is copied.
  // _len1 is allocated on the stack.
}

template <int_or_float T>
inline JobArr<T>::JobArr() : _args{NULL}, _args2{NULL}, _func_ptr_no_args{NULL}, _func_ptr{NULL}, _func_ptr_2_args{NULL}, _len1{-1}, _len2{-1} {
  // Default constructor for int and float template types.
}

template <int_or_float N> inline JobArr<N>::~JobArr() {
  // Destructor
  // Don't need to release packaged_task pointers. Forgot why
}

template <int_or_float T>
inline JobArr<T>::JobArr(T (*func_ptr)(const T *, const int *, const int *), T *arg1, int _len_args1, int c)
    : _func_ptr_c_i{new std::packaged_task<T(const T *, const int *, const int *)>(*func_ptr)}, _args{arg1}, _args2{NULL}, _len1{_len_args1},
      _len2{-1}, c_i{c}, c_f{0} {}

template <int_or_float T>
inline JobArr<T>::JobArr(T *(*func_ptr)(const T *, const int *, const int *), T *arg1, int _len_args1, int c)
    : _func_ptr_c_i_arr{new std::packaged_task<T *(const T *, const int *, const int *)>(*func_ptr)}, _args{arg1}, _args2{NULL}, _len1{_len_args1},
      _len2{-1}, c_i{c}, c_f{0} {}

template <int_or_float T>
inline JobArr<T>::JobArr(T (*func_ptr)(const T *, const int *, const float *), T *arg1, int _len_args1, float c)
    : _func_ptr_c_f{new std::packaged_task<T(const T *, const int *, const float *)>(*func_ptr)}, _args{arg1}, _args2{NULL}, _len1{_len_args1},
      _len2{-1}, c_f{c}, c_i{0} {}

template <int_or_float T>
inline JobArr<T>::JobArr(T *(*func_ptr)(const T *, const int *, const float *), T *arg1, int _len_args1, float c)
    : _func_ptr_c_f_arr{new std::packaged_task<T *(const T *, const int *, const float *)>(*func_ptr)}, _args{arg1}, _args2{NULL}, _len1{_len_args1},
      _len2{-1}, c_f{c}, c_i{0} {}

template <int_or_float T> inline JobArr<T>::JobArr(JobArr<T> &&other) : _len1{other._len1}, _len2{other._len2}, c_i{other.c_i}, c_f{other.c_f} {
  // Move constructor.
  if (other._args != NULL) {
    _args = new T[other._len1];
    for (int i = 0; i < other._len1; i++) {
      _args[i] = other._args[i];
    }
  } else
    _args = NULL;
  if (other._args2 != NULL) {
    _args2 = new T[other._len2];
    for (int i = 0; i < other._len2; i++) {
      _args2[i] = other._args2[i];
    }
  } else
    _args2 = NULL;
  if (other._func_ptr)
    _func_ptr = std::move(other._func_ptr);
  else
    _func_ptr = NULL;
  if (other._func_ptr_no_args)
    _func_ptr_no_args = std::move(other._func_ptr_no_args);
  else
    _func_ptr_no_args = NULL;
  if (other._func_ptr_2_args)
    _func_ptr_2_args = std::move(other._func_ptr_2_args);
  else
    _func_ptr_2_args = NULL;
  if (other._func_ptr_c_i)
    _func_ptr_c_i = std::move(other._func_ptr_c_i);
  else
    _func_ptr_c_i = NULL;
  if (other._func_ptr_c_f)
    _func_ptr_c_f = std::move(other._func_ptr_c_f);
  else
    _func_ptr_c_f = NULL;
  if (other._func_ptr_arr)
    _func_ptr_arr = std::move(other._func_ptr_arr);
  else
    _func_ptr_arr = NULL;
  if (other._func_ptr_2_args_arr)
    _func_ptr_2_args_arr = std::move(other._func_ptr_2_args_arr);
  else
    _func_ptr_2_args_arr = NULL;
  if (other._func_ptr_c_i_arr)
    _func_ptr_c_i_arr = std::move(other._func_ptr_c_i_arr);
  else
    _func_ptr_c_i_arr = NULL;
  if (other._func_ptr_c_f_arr)
    _func_ptr_c_f_arr = std::move(other._func_ptr_c_f_arr);
  else
    _func_ptr_c_f_arr = NULL;
  if (other._func_ptr_no_args_arr)
    _func_ptr_no_args_arr = std::move(other._func_ptr_no_args_arr);
  else
    _func_ptr_no_args_arr = NULL;

  other._func_ptr = NULL;
  other._func_ptr_2_args = NULL;
  other._func_ptr_no_args = NULL;
  other._func_ptr_c_i = NULL;
  other._func_ptr_c_f = NULL;
  other._func_ptr_arr = NULL;
  other._func_ptr_2_args_arr = NULL;
  other._func_ptr_c_i_arr = NULL;
  other._func_ptr_c_f_arr = NULL;
  other._func_ptr_no_args_arr = NULL;
}

template <int_or_float T> inline JobArr<T> &JobArr<T>::operator=(JobArr<T> &&other) {
  /* Move assignment
   * */
  delete[] _args;
  delete[] _args2;
  // Override original _len1 and _len2
  _len1 = other._len1;
  _len2 = other._len2;
  c_i = other.c_i;
  c_f = other.c_f;

  if (other._args != NULL) {
    _args = new T[other._len1];
    for (int i = 0; i < other._len1; i++) {
      _args[i] = other._args[i];
    }
  } else
    _args = NULL;
  if (other._args2 != NULL) {
    _args2 = new T[other._len2];
    for (int i = 0; i < other._len2; i++) {
      _args2[i] = other._args2[i];
    }
  } else
    _args2 = NULL;
  if (other._func_ptr != NULL)
    _func_ptr = std::move(other._func_ptr);
  else
    _func_ptr = NULL;
  if (other._func_ptr_no_args != NULL)
    _func_ptr_no_args = std::move(other._func_ptr_no_args);
  else
    _func_ptr_no_args = NULL;
  if (other._func_ptr_2_args != NULL)
    _func_ptr_2_args = std::move(other._func_ptr_2_args);
  else
    _func_ptr_2_args = NULL;
  if (other._func_ptr_c_i)
    _func_ptr_c_i = std::move(other._func_ptr_c_i);
  else
    _func_ptr_c_i = NULL;
  if (other._func_ptr_c_f)
    _func_ptr_c_f = std::move(other._func_ptr_c_f);
  else
    _func_ptr_c_f = NULL;
  if (other._func_ptr_arr)
    _func_ptr_arr = std::move(other._func_ptr_arr);
  else
    _func_ptr_arr = NULL;
  if (other._func_ptr_2_args_arr)
    _func_ptr_2_args_arr = std::move(other._func_ptr_2_args_arr);
  else
    _func_ptr_2_args_arr = NULL;
  if (other._func_ptr_c_i_arr)
    _func_ptr_c_i_arr = std::move(other._func_ptr_c_i_arr);
  else
    _func_ptr_c_i_arr = NULL;
  if (other._func_ptr_c_f_arr)
    _func_ptr_c_f_arr = std::move(other._func_ptr_c_f_arr);
  else
    _func_ptr_c_f_arr = NULL;
  if (other._func_ptr_no_args_arr)
    _func_ptr_no_args_arr = std::move(other._func_ptr_no_args_arr);
  else
    _func_ptr_no_args_arr = NULL;

  other._func_ptr = NULL;
  other._func_ptr_2_args = NULL;
  other._func_ptr_no_args = NULL;
  other._func_ptr_c_f = NULL;
  other._func_ptr_c_i = NULL;
  other._func_ptr_arr = NULL;
  other._func_ptr_2_args_arr = NULL;
  other._func_ptr_c_i_arr = NULL;
  other._func_ptr_c_f_arr = NULL;
  other._func_ptr_no_args_arr = NULL;

  return *this;
}

template <int_or_float T> inline T *JobArr<T>::GetArg1() {
  // 0 is returned if there is no value for the first parameter of the Job.
  // The 0 gets casted.
  // This only overloads int and float types.
  if (_args)
    return _args;
  else
    return 0;
}

template <int_or_float T> inline T *JobArr<T>::GetArg2() {
  // 0 is returned if there is no value for the second parameter of the Job.
  // The 0 gets casted to int or float.
  // This only overloads int and float types.
  if (_args2)
    return _args2;
  else
    return 0;
}

template <int_or_float T> inline int JobArr<T>::GetLen1() { return _len1; }

template <int_or_float T> inline int JobArr<T>::GetLen2() { return _len2; }

template <int_or_float T> inline void JobArr<T>::Run() {
  // Executes function and pass in arguments.
  if (_func_ptr_2_args) {
    (*_func_ptr_2_args)(_args, _args2, &_len1, &_len2);
  } else if (_func_ptr) {
    (*_func_ptr)(_args, &_len1);
  } else if (_func_ptr_no_args) {
    (*_func_ptr_no_args)();
  } else if (_func_ptr_c_i) {
    (*_func_ptr_c_i)(_args, &_len1, &c_i);
  } else if (_func_ptr_c_f) {
    (*_func_ptr_c_f)(_args, &_len1, &c_f);
  } else if (_func_ptr_arr) {
    (*_func_ptr_arr)(_args, &_len1);
  } else if (_func_ptr_2_args_arr) {
    (*_func_ptr_2_args_arr)(_args, _args2, &_len1, &_len2);
  } else if (_func_ptr_c_i_arr) {
    (*_func_ptr_c_i_arr)(_args, &_len1, &c_i);
  } else if (_func_ptr_c_f_arr) {
    (*_func_ptr_c_f_arr)(_args, &_len1, &c_f);
  } else if (_func_ptr_no_args_arr) {
    (*_func_ptr_no_args_arr)();
  }
}

template <int_or_float T> inline std::future<T> JobArr<T>::GetFuture() {
  /* Returns a future object associated with this Job instance.
   * */
  if (_func_ptr_2_args) {
    return _func_ptr_2_args->get_future();
  } else if (_func_ptr) {
    return _func_ptr->get_future();
  } else if (_func_ptr_no_args) {
    return _func_ptr_no_args->get_future();
  } else if (_func_ptr_c_i) {
    return _func_ptr_c_i->get_future();
  } else if (_func_ptr_c_f) {
    return _func_ptr_c_f->get_future();
  } else
    throw std::runtime_error("Error! Cannot get std::future<T> object because there is no function to be executed.");
}

template <int_or_float T> inline std::future<T *> JobArr<T>::GetFuturePtr() {
  /* Returns a future object associated with this Job instance.
   * */
  if (_func_ptr_2_args_arr) {
    return _func_ptr_2_args_arr->get_future();
  } else if (_func_ptr_arr) {
    return _func_ptr_arr->get_future();
  } else if (_func_ptr_no_args_arr) {
    return _func_ptr_no_args_arr->get_future();
  } else if (_func_ptr_c_i_arr) {
    return _func_ptr_c_i_arr->get_future();
  } else if (_func_ptr_c_f_arr) {
    return _func_ptr_c_f_arr->get_future();
  } else
    throw std::runtime_error("Error! Cannot get std::future<T> object because there is no function to be executed.");
}

template <int_or_float T> inline int JobArr<T>::_debug_func() {
  if (_func_ptr_2_args) {
    return 1;
  } else if (_func_ptr) {
    return 2;
  } else if (_func_ptr_no_args) {
    return 3;
  } else if (_func_ptr_c_i) {
    return 4;
  } else if (_func_ptr_c_f) {
    return 5;
  } else if (_func_ptr_2_args_arr) {
    return 6;
  } else if (_func_ptr_arr) {
    return 7;
  } else if (_func_ptr_no_args_arr) {
    return 8;
  } else if (_func_ptr_c_i_arr) {
    return 9;
  } else if (_func_ptr_c_f_arr) {
    return 10;
  } else
    return 0;
}

template <Numeric T> inline int Job<T>::_debug_func() {
  if (_func_ptr_2_args) {
    return 1;
  } else if (_func_ptr) {
    return 2;
  } else if (_func_ptr_no_args) {
    return 3;
  } else
    return 0;
}

inline int Job<NumericVariant>::_debug_func() {
  if (_func_ptr_2_args) {
    return 1;
  } else if (_func_ptr) {
    return 2;
  } else if (_func_ptr_no_args) {
    return 3;
  } else
    return 0;
}

template <int_or_float N>
inline JobTwoArray<N>::JobTwoArray(N *(*func_ptr)(const N *, const N *, const int, const int), N const *arg1, N const *arg2, const int len_args1,
                                   const int len_args2)
    : _args{arg1}, _args2{arg2}, _len1{len_args1}, _len2{len_args2},
      _func_ptr_2_args_arr{new std::packaged_task<N *(const N *, const N *, const int, const int)>(*func_ptr)}, _func_ptr_2{NULL} {
  // constructor for functions with array parameter.
  // arg1 and arg2 should be pointers that are allocated on heap or stack.
  // _len1 is allocated on the stack.
}

template <int_or_float T>
inline JobTwoArray<T>::JobTwoArray(T *(*func_ptr)(const T *, T **, const int, const int), T const *arg1, T **arg2, int len_args1, int len_args2)
    : _args{arg1}, _args2{NULL}, _arg2_p{arg2}, _len1{len_args1}, _len2{len_args2},
      _func_ptr_2{new std::packaged_task<T *(const T *, T **, const int, const int)>(*func_ptr)}, _func_ptr_2_args_arr{NULL} {}

template <int_or_float T>
inline JobTwoArray<T>::JobTwoArray() : _args{NULL}, _args2{NULL}, _arg2_p{NULL}, _func_ptr_2_args_arr{NULL}, _func_ptr_2{NULL}, _len1{-1}, _len2{-1} {
  // Default constructor for int and float template types.
}

template <int_or_float N> inline JobTwoArray<N>::~JobTwoArray() {
  // Destructor
  // Don't need to release packaged_task pointers. Forgot why
}

template <int_or_float T> inline JobTwoArray<T>::JobTwoArray(JobTwoArray<T> &&other) {
  // Move constructor.
  _len1 = other._len1;
  _len2 = other._len2;
  if (other._args != NULL) {
    _args = other._args;
  } else
    _args = NULL;
  if (other._args2 != NULL) {
    _args2 = other._args2;
  } else
    _args2 = NULL;
  if (other._arg2_p != NULL) {
    _arg2_p = other._arg2_p;
  } else
    _arg2_p = NULL;

  if (other._func_ptr_2_args_arr)
    _func_ptr_2_args_arr = std::move(other._func_ptr_2_args_arr);
  else
    _func_ptr_2_args_arr = NULL;
  if (other._func_ptr_2)
    _func_ptr_2 = std::move(other._func_ptr_2);
  else
    _func_ptr_2 = NULL;

  other._func_ptr_2_args_arr = NULL;
  other._func_ptr_2 = NULL;
  other._args = NULL;
  other._args2 = NULL;
}

template <int_or_float T> inline JobTwoArray<T> &JobTwoArray<T>::operator=(JobTwoArray<T> &&other) {
  // Move assignment.
  delete[] _args;
  delete[] _args2;

  _len1 = other._len1;
  _len2 = other._len2;

  if (other._args != NULL) {
    _args = other._args;
  } else
    _args = NULL;
  if (other._args2 != NULL) {
    _args2 = other._args2;
  } else
    _args2 = NULL;
  if (other._arg2_p != NULL) {
    _arg2_p = other._arg2_p;
  } else
    _arg2_p = NULL;
  if (other._func_ptr_2_args_arr)
    _func_ptr_2_args_arr = std::move(other._func_ptr_2_args_arr);
  else
    _func_ptr_2_args_arr = NULL;
  if (other._func_ptr_2)
    _func_ptr_2 = std::move(other._func_ptr_2);
  else
    _func_ptr_2 = NULL;

  other._func_ptr_2_args_arr = NULL;
  other._func_ptr_2 = NULL;
  other._args = NULL;
  other._args2 = NULL;
  return *this;
}

template <int_or_float T> inline T const *JobTwoArray<T>::GetArg1() { return _args; }

template <int_or_float T> inline T const *JobTwoArray<T>::GetArg2() { return _args2; }

template <int_or_float T> inline int JobTwoArray<T>::GetLen1() { return _len1; }

template <int_or_float T> inline int JobTwoArray<T>::GetLen2() { return _len2; }

template <int_or_float T> inline std::future<T *> JobTwoArray<T>::GetFuture() {
  if (_func_ptr_2_args_arr) {
    return _func_ptr_2_args_arr->get_future();
  } else if (_func_ptr_2) {
    return _func_ptr_2->get_future();
  } else
    throw std::runtime_error("Error! Cannot get std::future<T> object because there is no function to be executed.");
}

template <int_or_float T> inline void JobTwoArray<T>::GetFuture(std::future<T *> &out) {
  if (_func_ptr_2_args_arr) {
    out = _func_ptr_2_args_arr->get_future();
  } else if (_func_ptr_2) {
    out = _func_ptr_2->get_future();
  } else
    throw std::runtime_error("Error! Cannot get std::future<T> object because there is no function to be executed.");
}

template <int_or_float T> inline void JobTwoArray<T>::Run() {
  if (_func_ptr_2_args_arr) {
    (*_func_ptr_2_args_arr)(_args, _args2, _len1, _len2);
  } else if (_func_ptr_2) {
    (*_func_ptr_2)(_args, _arg2_p, _len1, _len2);
  }
}

template <int_or_float N>
inline JobOneArray<N>::JobOneArray(N *(*func_ptr)(const N *, const int), N const *arg1, const int len_args1)
    : _args{arg1}, _len1{len_args1}, _func_ptr{new std::packaged_task<N *(const N *, const int)>(*func_ptr)} {
  // constructor for functions with array parameter.
  // arg1 should be allocated on stack or heap.
  // _len1 is allocated on the stack.
}

template <int_or_float T> inline JobOneArray<T>::JobOneArray() : _args{NULL}, _func_ptr{NULL}, _len1{-1} {
  // Default constructor for int and float template types.
}

template <int_or_float N> inline JobOneArray<N>::~JobOneArray() {
  // Destructor
  // Don't need to release packaged_task pointers. Forgot why
}

template <int_or_float T> inline JobOneArray<T>::JobOneArray(JobOneArray<T> &&other) {
  // Move constructor.
  _len1 = other._len1;

  if (other._args != NULL) {
    _args = other._args;
  } else
    _args = NULL;
  if (other._func_ptr)
    _func_ptr = std::move(other._func_ptr);
  else
    _func_ptr = NULL;

  other._func_ptr = NULL;
  other._args = NULL;
}

template <int_or_float T> inline JobOneArray<T> &JobOneArray<T>::operator=(JobOneArray<T> &&other) {
  // Move assignment.
  delete[] _args;

  _len1 = other._len1;

  if (other._args != NULL) {
    _args = other._args;
  } else
    _args = NULL;
  if (other._func_ptr)
    _func_ptr = std::move(other._func_ptr);
  else
    _func_ptr = NULL;

  other._func_ptr = NULL;
  other._args = NULL;
  return *this;
}

template <int_or_float T> inline T const *JobOneArray<T>::GetArg1() { return _args; }

template <int_or_float T> inline int JobOneArray<T>::GetLen1() { return _len1; }

template <int_or_float T> inline std::future<T *> JobOneArray<T>::GetFuture() {
  if (_func_ptr) {
    return _func_ptr->get_future();
  } else
    throw std::runtime_error("Error! Cannot get std::future<T> object because there is no function to be executed.");
}

template <int_or_float T> inline void JobOneArray<T>::Run() {
  if (_func_ptr)
    (*_func_ptr)(_args, _len1);
}

template <int_or_float T> inline void JobOneArray<T>::GetFuture(std::future<T *> &out) {
  if (_func_ptr) {
    out = _func_ptr->get_future();
  } else
    throw std::runtime_error("Error! Cannot get std::future<T> object because there is no function to be executed.");
}

template <int_or_float T>
inline JobOneArrayC<T>::JobOneArrayC(T *(*func_ptr)(const T *, const int, const T), T const *arg1, int len_args1, T c)
    : _args{arg1}, _len1{len_args1}, _func_ptr{new std::packaged_task<T *(const T *, const int, const T)>(*func_ptr)}, _cons{c} {
  // constructor for functions with array parameter.
  // _len1 is allocated on the stack.
}

template <int_or_float T> inline JobOneArrayC<T>::JobOneArrayC() : _args{NULL}, _func_ptr{NULL}, _len1{-1}, _cons{0} {}

template <int_or_float T> inline JobOneArrayC<T>::~JobOneArrayC() {
  // Destructor.
  // Don't need to release packaged_task pointers.
}

template <int_or_float T> inline JobOneArrayC<T>::JobOneArrayC(JobOneArrayC<T> &&other) {
  // Move constructor.
  _len1 = other._len1;

  if (other._args != NULL) {
    _args = other._args;
  } else
    _args = NULL;

  if (other._func_ptr)
    _func_ptr = std::move(other._func_ptr);
  else
    _func_ptr = NULL;

  other._func_ptr = NULL;
  other._args = NULL;
}

template <int_or_float T> inline JobOneArrayC<T> &JobOneArrayC<T>::operator=(JobOneArrayC<T> &&other) {
  // Move assignment
  delete[] _args;

  _len1 = other._len1;

  if (other._args) {
    _args = other._args;
  } else
    _args = NULL;

  if (other._func_ptr) {
    _func_ptr = other._func_ptr;
  } else
    _func_ptr = NULL;

  other._func_ptr = NULL;
  other._args = NULL;
}

template <int_or_float T> inline T *JobOneArrayC<T>::GetArg1() { return _args; }

template <int_or_float T> inline int JobOneArrayC<T>::GetLen1() { return _len1; }

template <int_or_float T> inline T JobOneArrayC<T>::GetScalar() { return _cons; }

template <int_or_float T> inline void JobOneArrayC<T>::GetFuture(std::future<T *> &out) {
  if (_func_ptr)
    out = _func_ptr->get_future();
  else
    throw std::runtime_error("Error! Cannot get std::future<T> object because there is no function to be executed.");
}

template <int_or_float T> inline void JobOneArrayC<T>::Run() {
  if (_func_ptr)
    (*_func_ptr)(_args, _len1, _cons);
}

#endif
