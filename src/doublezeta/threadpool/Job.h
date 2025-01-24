/*
TODO: Templates classes and functions might not be callable from python because compiler needs to see the template arguments that are used in order to
create the corresponding overloaded func/method/class.
Reference: https://isocpp.org/wiki/faq/templates#templates-defn-vs-decl

This OOP approach is more lengthy, combersome, and tedious than using c style callback function with callback arguments.
*/
#include "typing/DTypes.h"
#include <functional>
#include <future>
#include <stdexcept>
#ifndef JOB_H
#define JOB_H

class BaseJob {
  /* Note that the pure virtual function is defined in all sub-types (derived classes).
   * However, not all optinal virtual methods are defined in the derived classes and only the one for the respective template type
   * is required and defined. For instance, a `Job<int>` sub-class will have the `void GetFuture(std::future<int> &out)` defined, and
   * all other optinal virtual methods are given the default definition of raising a runtime error. Of course, this pattern might not hold
   * for all derived classes as it is possible that they implement more than one of these optional virtual methods.
   *
   * This was needed to achieve polymorphism for the interface.
   * */
public:
  virtual void Run() = 0;
  virtual void GetFuture(std::future<int> &out) { std::runtime_error("Not implemented! Please don't use this method."); }
  virtual void GetFuture(std::future<float> &out) { std::runtime_error("Not implemented! Please don't use this method."); }
  virtual void GetFuture(std::future<NumericVariant> &out) { std::runtime_error("Not inplemented! Please don't use this method."); }
  virtual void GetFuture(std::future<int *> &out) { std::runtime_error("Not inplemented! Please don't use this method."); }
  virtual void GetFuture(std::future<float *> &out) { std::runtime_error("Not implemented! Please don't use this method."); }
  virtual void GetFuture(std::future<NumericVariant *> &out) { std::runtime_error("Not inplemented! Please don't use this method."); }
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
  ~Job() override;
  void Run() override;
  std::future<T> GetFuture();
  void GetFuture(std::future<T> &out) override;
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

template <Numeric T> class JobNoParam : public BaseJob {
public:
  JobNoParam();
  JobNoParam(T (*func_ptr)());
  JobNoParam(JobNoParam<T> &other) = delete;
  JobNoParam &operator=(Job<T> &other) = delete;
  JobNoParam(JobNoParam<T> &&other);
  JobNoParam &operator=(JobNoParam<T> &&other);
  ~JobNoParam() override;
  void Run() override;
  void GetFuture(std::future<T> &out) override;
  std::future<T> GetFuture();

private:
  std::packaged_task<T()> *_func_ptr_no_args = NULL;
};

template <Numeric T> class JobOneParam : public BaseJob {
public:
  JobOneParam();
  JobOneParam(T (*func_ptr)(const T *), T arg1);
  JobOneParam(T (*func_ptr)(const T &), T arg1);
  JobOneParam(T (*func_ptr)(const T), T arg1);
  JobOneParam(JobOneParam<T> &other) = delete;
  JobOneParam &operator=(JobOneParam<T> &other) = delete;
  JobOneParam(JobOneParam<T> &&other);
  JobOneParam &operator=(JobOneParam<T> &&other);
  ~JobOneParam() override;
  void Run() override;
  std::future<T> GetFuture();
  void GetFuture(std::future<T> &out) override;
  T GetArg1();

private:
  std::packaged_task<T(const T *)> *_func_ptr = NULL;
  const T *_args = new T;
};

template <Numeric T> class JobTwoParam : public BaseJob {
public:
  JobTwoParam();
  JobTwoParam(T (*func_ptr)(const T *, const T *), T arg1, T arg2);
  JobTwoParam(T (*func_ptr)(const T &, const T &), T arg1, T arg2);
  JobTwoParam(T (*func_ptr)(const T, const T), T arg1, T arg2);
  JobTwoParam(JobTwoParam<T> &other) = delete;
  JobTwoParam &operator=(JobTwoParam<T> &other) = delete;
  JobTwoParam(JobTwoParam<T> &&other);
  JobTwoParam &operator=(JobTwoParam<T> &&other);
  ~JobTwoParam() override;
  void Run() override;
  std::future<T> GetFuture();
  void GetFuture(std::future<T> &out) override;
  T GetArg1();
  T GetArg2();

private:
  std::packaged_task<T(const T *, const T *)> *_func_ptr_2_args = NULL;
  const T *_args = new T;
  const T *_args2 = new T;
};

template <int_or_float T> class JobTwoArray : public BaseJob {
public:
  JobTwoArray();
  JobTwoArray(T *(*func_ptr)(const T *, const T *, const int, const int), T const *arg1, T const *arg2, int len_args1, int len_args2);
  JobTwoArray(T *(*func_ptr)(const T *, T **, const int, const int), T const *arg1, T **arg2, int len_args1, int len_args2);
  JobTwoArray(JobTwoArray<T> &other) = delete;
  JobTwoArray &operator=(JobTwoArray<T> &other) = delete;
  JobTwoArray(JobTwoArray<T> &&other);
  JobTwoArray &operator=(JobTwoArray<T> &&other);
  ~JobTwoArray() override;
  void Run() override;
  std::future<T *> GetFuture();
  void GetFuture(std::future<T *> &out) override;
  T const *GetArg1();
  T const *GetArg2();
  int GetLen1();
  int GetLen2();

private:
  std::packaged_task<T *(const T *, const T *, const int, const int)> *_func_ptr_2_args_arr = NULL;
  std::packaged_task<T *(const T *, T **, const int, const int)> *_func_ptr_2 = NULL;
  T const *_args;
  T const *_args2;
  T **_arg2_p;
  int _len1;
  int _len2;
};

template <int_or_float T> class JobOneArray : public BaseJob {
public:
  JobOneArray();
  JobOneArray(T *(*func_ptr)(const T *, const int), T const *arg1, int len_args1);
  JobOneArray(JobOneArray<T> &other) = delete;
  JobOneArray &operator=(JobOneArray<T> &other) = delete;
  JobOneArray(JobOneArray<T> &&other);
  JobOneArray &operator=(JobOneArray<T> &&other);
  ~JobOneArray() override;
  void Run() override;
  void GetFuture(std::future<T *> &out) override;
  std::future<T *> GetFuture();
  T const *GetArg1();
  int GetLen1();

private:
  std::packaged_task<T *(const T *, const int)> *_func_ptr = NULL;
  T const *_args;
  int _len1;
};

template <int_or_float T> class JobOneArrayC : public BaseJob {
public:
  JobOneArrayC();
  JobOneArrayC(T *(*func_ptr)(const T *, const int, const T), T const *arg1, int len_args1, T c);
  JobOneArrayC(JobOneArrayC<T> &other) = delete;
  JobOneArrayC &operator=(JobOneArrayC<T> &other) = delete;
  JobOneArrayC(JobOneArrayC<T> &&other);
  JobOneArrayC &operator=(JobOneArrayC<T> &&other);
  ~JobOneArrayC() override;
  void Run() override;
  void GetFuture(std::future<T *> &out) override;
  T *GetArg1();
  int GetLen1();
  T GetScalar();

private:
  std::packaged_task<T *(const T *, const int, const T)> *_func_ptr = NULL;
  T const *_args;
  int _len1;
  T _cons;
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
  ~JobArr() override;
  void Run() override;
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
  T *_args = NULL;
  T *_args2 = NULL;
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
  ~Job() override;
  void Run() override;
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
