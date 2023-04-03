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

#endif
