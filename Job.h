#include "DTypes.h"
#include <functional>
#ifndef JOB_H
#define JOB_H

template <Numeric T> class Job {
public:
  Job(T (*func_ptr)(const T *), T *arg1); // Function takes a single pointer of any type T and returns the same type.
  Job(T (*func_ptr)());                   // Function takes no argument and returns type T.
  Job(T (*func_ptr)(const T *, const T *), T *arg1, T *arg2);
  Job(const Job<T> &other);
  Job &operator=(const Job<T> &a);
  ~Job();
  virtual T Run();

private:
  std::function<T(const T *, const T *)> _func_ptr_2_args;
  std::function<T(const T *)> _func_ptr;
  std::function<T()> _func_ptr_no_args;
  const T *_args;
  const T *_args2;
};

template <> class Job<NumericVariant> {
public:
  Job(NumericVariant (*func_ptr)(const NumericVariant *), NumericVariant *arg1);
  Job(NumericVariant (*func_ptr)());
  Job(NumericVariant (*func_ptr)(const NumericVariant), NumericVariant *arg1, NumericVariant *arg2);
  template <int_or_float T> Job(T (*func_ptr)());
  template <int_or_float T> Job(T (*func_ptr)(const T *), T *arg1);
  template <int_or_float T> Job(T (*func_ptr)(const T), T arg1);
  template <int_or_float T> Job(T (*func_ptr)(const T *, const T *), T *arg1, T *arg2);
  template <int_or_float T> Job(T (*func_ptr)(const T, const T), T arg1, T arg2);

  //Job(float (*func_ptr)());
  //Job(float (*func_ptr)(const float *), float *arg1);
  //Job(float (*func_ptr)(const float), float arg1);
  //Job(float (*func_ptr)(const float *, const float *), float *arg1, float *arg2);
  //Job(float (*func_ptr)(const float, const float), float arg1, float arg2);
  //Job(int (*func_ptr)(const int *), int *arg1);
  //Job(int (*func_ptr)(const int), int arg1);
  //Job(int (*func_ptr)(const int *, const int *), int *arg1, int *arg2);
  //Job(int (*func_ptr)(const int, const int), int arg1, int arg2);
  Job(Job<float> other);
  Job(const Job<NumericVariant> &other);
  Job &operator=(const Job &other);
  ~Job();
  NumericVariant Run();

private:
  std::function<NumericVariant(const NumericVariant *, const NumericVariant *)> _func_ptr_2_args;
  std::function<NumericVariant(const NumericVariant *)> _func_ptr;
  std::function<NumericVariant()> _func_ptr_no_args;
  const NumericVariant *_args;
  const NumericVariant *_args2;
};

#include "Job.tpp"

#endif
