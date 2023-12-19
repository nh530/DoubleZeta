/*
TODO: Templates classes and functions might not be callable from python because compiler needs to see the template arguments that are used in order to
create the corresponding overloaded func/method/class.
Reference: https://isocpp.org/wiki/faq/templates#templates-defn-vs-decl
*/
#include "typing/DTypes.h"
#include <functional>
#include <future>
#include <stdexcept>
#ifndef JOB_H
#define JOB_H

class BaseJob {
public:
  virtual void Run() = 0;
  template <Numeric T> std::future<T> GetFuture();
  virtual ~BaseJob() { // Do nothing; This is handled in subclasses (derived classes)
  }
};

template <Numeric T> class Job : public BaseJob {
public:
  Job();
  Job(T (*func_ptr)(const T *), T *arg1);
  Job(T (*func_ptr)());
  Job(T (*func_ptr)(const T *, const T *), T *arg1, T *arg2);
  Job(Job<T> &other) = delete;
  Job &operator=(Job<T> &other) = delete;
  Job(Job<T> &&other);
  Job &operator=(Job<T> &&other);
  ~Job();
  void Run();
  std::future<T> GetFuture();
  T GetArg1();
  T GetArg2();
  int _debug_func(); // Method is for testing purposes only; Not for end-users; Used for unit testing

private:
  std::packaged_task<T(const T *, const T *)> *_func_ptr_2_args = NULL;
  std::packaged_task<T(const T *)> *_func_ptr = NULL;
  std::packaged_task<T()> *_func_ptr_no_args = NULL;
  const T *_args = new T;
  const T *_args2 = new T;
};

template <int_or_float T> class JobArr : public BaseJob {
public:
  JobArr();
  JobArr(T (*func_ptr)());  // TODO: not implemented yet.
  JobArr(T *(*func_ptr)()); // TODO: not implemented yet.
  JobArr(T (*func_ptr)(const T *, const int *), T *arg1, int _len_args1);
  JobArr(T *(*func_ptr)(const T *, const int *), T *arg1, int _len_args1);
  JobArr(T (*func_ptr)(const T *, const int *, const int *), T *arg1, int _len_args1, int c);
  JobArr(T *(*func_ptr)(const T *, const int *, const int *), T *arg1, int _len_args1, int c);
  JobArr(T (*func_ptr)(const T *, const int *, const float *), T *arg1, int _len_args1, float c);
  JobArr(T *(*func_ptr)(const T *, const int *, const float *), T *arg1, int _len_args1, float c);
  JobArr(T (*func_ptr)(const T *, const T *, const int *, const int *), T *arg1, T *arg2, int _len_args1, int _len_args2);
  JobArr(T *(*func_ptr)(const T *, const T *, const int *, const int *), T *arg1, T *arg2, int _len_args1, int _len_args2);
  JobArr(JobArr<T> &other) = delete;
  JobArr &operator=(JobArr<T> &other) = delete;
  JobArr(JobArr<T> &&other);
  JobArr &operator=(JobArr<T> &&other);
  ~JobArr();
  void Run();
  std::future<T> GetFuture();
  std::future<T *> GetFuturePtr();
  T *GetArg1();
  T *GetArg2();
  int GetLen1();
  int GetLen2();
  int _debug_func(); // Method is for testing purposes only; Not for end-users. Used for unit testing.

private:
  std::packaged_task<T(const T *, const T *, const int *, const int *)> *_func_ptr_2_args = NULL;
  std::packaged_task<T *(const T *, const T *, const int *, const int *)> *_func_ptr_2_args_arr = NULL;
  std::packaged_task<T(const T *, const int *)> *_func_ptr = NULL;
  std::packaged_task<T *(const T *, const int *)> *_func_ptr_arr = NULL;
  std::packaged_task<T(const T *, const int *, const int *)> *_func_ptr_c_i = NULL;
  std::packaged_task<T *(const T *, const int *, const int *)> *_func_ptr_c_i_arr = NULL;
  std::packaged_task<T(const T *, const int *, const float *)> *_func_ptr_c_f = NULL;
  std::packaged_task<T *(const T *, const int *, const float *)> *_func_ptr_c_f_arr = NULL;
  std::packaged_task<T()> *_func_ptr_no_args = NULL;
  std::packaged_task<T *()> *_func_ptr_no_args_arr = NULL;
  int c_i;
  float c_f;
  T *_args;
  T *_args2;
  int _len1;
  int _len2;
};

template <> class Job<NumericVariant> : public BaseJob {
  // Explicit template specialization
public:
  Job();
  Job(NumericVariant (*func_ptr)(const NumericVariant *), NumericVariant *arg1);
  Job(NumericVariant (*func_ptr)());
  Job(NumericVariant (*func_ptr)(const NumericVariant *, const NumericVariant *), NumericVariant *arg1, NumericVariant *arg2);
  template <int_or_float T> Job(T (*func_ptr)());
  template <int_or_float T> Job(T (*func_ptr)(const T *), T *arg1);
  template <int_or_float T> Job(T (*func_ptr)(const T), T arg1);
  template <int_or_float T> Job(T (*func_ptr)(const T *, const T *), T *arg1, T *arg2);
  template <int_or_float T> Job(T (*func_ptr)(const T, const T), T arg1, T arg2);
  // Deleted copy initailization and copy assignment.
  Job(Job<NumericVariant> &other) = delete;
  Job &operator=(Job<NumericVariant> &other) = delete;
  Job(Job<NumericVariant> &&other);
  Job &operator=(Job<NumericVariant> &&other);
  ~Job();
  void Run();
  std::future<NumericVariant> GetFuture();
  NumericVariant GetArg1();
  NumericVariant GetArg2();
  int _debug_func(); // Method is for testing purposes only; Not for end-users; Used for unit testing.

private:
  std::packaged_task<NumericVariant(const NumericVariant *, const NumericVariant *)> *_func_ptr_2_args = NULL;
  std::packaged_task<NumericVariant(const NumericVariant *)> *_func_ptr = NULL;
  std::packaged_task<NumericVariant()> *_func_ptr_no_args = NULL;
  const NumericVariant *_args;
  const NumericVariant *_args2;
};

class JobFloat : public Job<float> {};
class JobInt : public Job<int> {};
class JobNumv : public Job<NumericVariant> {};

#include "Job.tpp"

#endif
