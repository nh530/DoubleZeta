#include "DTypes.h"

#ifndef JOB_TPP
#define JOB_TPP

#include "Job.h"

template <Numeric T> Job<T>::Job(T (*func_ptr)(const T *), T *arg1) : _func_ptr{func_ptr}, _args{arg1}, _args2{NULL} {}
template <Numeric T> Job<T>::Job(T (*func_ptr)()) : _func_ptr_no_args{func_ptr}, _args{NULL}, _args2{NULL} {}
template <Numeric T> Job<T>::Job(T (*func_ptr)(const T *, const T *), T *arg1, T *arg2) : _func_ptr_2_args{func_ptr}, _args{arg1}, _args2{arg2} {}
template <Numeric T>
Job<T>::Job(const Job<T> &other)
    : _args2{new T{*other._args2}}, _func_ptr{other._func_ptr}, _func_ptr_no_args{other._func_ptr_no_args}, _func_ptr_2_args{other._func_ptr_2_args} {
  _args = new T{*other._args};
}
template <Numeric T> Job<T> &Job<T>::operator=(const Job<T> &other) {
  delete _args;
  _args = new T{*other._args};
  delete _args2;
  _args2 = new T{*other._args2};
  _func_ptr = other._func_ptr;
  _func_ptr_2_args = other._func_ptr_2_args;
  _func_ptr_no_args = other._func_ptr_no_args;
}

template <Numeric T> Job<T>::~Job() {
  delete _args;
  delete _args2;
}
template <Numeric T> T Job<T>::Run() {
  // Executes function and pass in arguments.
  if (_args && _args2) {
    return (_func_ptr_2_args)(_args, _args2);
  } else if (_args) {
    return (_func_ptr)(_args);
  } else
    return (_func_ptr_no_args)();
}

Job<NumericVariant>::Job(NumericVariant (*func_ptr)(const NumericVariant *), NumericVariant *arg1) : _func_ptr{func_ptr}, _args{arg1}, _args2{NULL} {}
Job<NumericVariant>::Job(NumericVariant (*func_ptr)()) : _func_ptr_no_args{func_ptr}, _args{NULL}, _args2{NULL} {}
template <int_or_float T> Job<NumericVariant>::Job(T (*func_ptr)()) : _args{NULL}, _args2{NULL} {
  // Handle the conversion from Job<float> to Job<NumericVariant> by creating a new function that outputs NumericVariant.
  std::function<NumericVariant()> temp = [func_ptr]() -> NumericVariant {
    return (func_ptr()); // Output gets casted or wrapped by std::variant<float, int>
  };
  _func_ptr_no_args = temp;
}

template <int_or_float T> Job<NumericVariant>::Job(T (*func_ptr)(const T *), T *arg1) : _args2{NULL} {
  std::function<NumericVariant(const NumericVariant *)> temp = [func_ptr](const NumericVariant *x) -> NumericVariant {
    T temp = std::get<T>(*x);
    return (func_ptr(&temp));
  };
  _func_ptr = temp;
  _args = new NumericVariant{*arg1};
}

template <int_or_float T> Job<NumericVariant>::Job(T (*func_ptr)(const T), T arg1) : _args2{NULL} {
  std::function<NumericVariant(const NumericVariant *)> temp = [func_ptr](const NumericVariant *x) -> NumericVariant {
    T temp = std::get<T>(*x);
    return (func_ptr(temp));
  };
  _func_ptr = temp;
  _args = new NumericVariant{arg1};
}

template <int_or_float T> Job<NumericVariant>::Job(T (*func_ptr)(const T *, const T *), T *arg1, T *arg2) {
  std::function<NumericVariant(const NumericVariant *, const NumericVariant *)> temp = [func_ptr](const NumericVariant *x,
                                                                                                  const NumericVariant *y) -> NumericVariant {
    T t_x = std::get<T>(*x);
    T t_y = std::get<T>(*y);
    return (func_ptr(&t_x, &t_y));
  };
  _func_ptr_2_args = temp;
  _args = new NumericVariant{*arg1};
  _args2 = new NumericVariant{*arg2};
}

template <int_or_float T> Job<NumericVariant>::Job(T (*func_ptr)(const T, const T), T arg1, T arg2) {
  std::function<NumericVariant(const NumericVariant *, const NumericVariant *)> temp = [func_ptr](const NumericVariant *x,
                                                                                                  const NumericVariant *y) -> NumericVariant {
    T t_x = std::get<T>(*x);
    T t_y = std::get<T>(*y);
    return (func_ptr(t_x, t_y));
  };
  _func_ptr_2_args = temp;
  _args = new NumericVariant{arg1};
  _args2 = new NumericVariant{arg2};
}

Job<NumericVariant>::Job(Job<float> other) {
  // TODO: Access the private function pointer.
}
Job<NumericVariant>::Job(const Job<NumericVariant> &other)
    : _args2{new NumericVariant{*other._args2}}, _func_ptr{other._func_ptr}, _func_ptr_no_args{other._func_ptr_no_args}, _func_ptr_2_args{
                                                                                                                             other._func_ptr_2_args} {
  _args = new NumericVariant{*other._args};
}
Job<NumericVariant> &Job<NumericVariant>::operator=(const Job &other) {
  delete _args;
  _args = new NumericVariant{*other._args};
  delete _args2;
  _args2 = new NumericVariant{*other._args2};
  _func_ptr = other._func_ptr; // Copy assignment.
  _func_ptr_2_args = other._func_ptr_2_args;
  _func_ptr_no_args = other._func_ptr_no_args;

  return *this;
}
Job<NumericVariant>::~Job() {
  delete _args;
  delete _args2;
}
NumericVariant Job<NumericVariant>::Run() {
  // Executes function and pass in arguments.
  if (_args && _args2) {
    return (_func_ptr_2_args)(_args, _args2);
  } else if (_args) {
    return (_func_ptr)(_args);
  } else
    return (_func_ptr_no_args)();
}

#endif
