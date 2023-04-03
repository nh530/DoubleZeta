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

#include "Job.tpp"

#endif
