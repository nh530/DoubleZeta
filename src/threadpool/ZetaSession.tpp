/* The goal of these user defined types is to create a multi-threaded, parallel and concurrent, framework that can process
 * commonly found algebraic operations in mathematics. Fundamental algebra operations can be found in in the Algebra header file.
 * From which, more complex mathematical equations can be represented in a manner that supports parallel processing.
 *
 * Notes: It is difficult to implement the parallel computing of a single math equation. What is easily doable is to implement the
 * parallel computing of many equations.
 */

#include <future>
#include <iostream>
#include <queue>
#include <thread>
#include <variant>

#ifndef ZETASESSION_TPP
#define ZETASESSION_TPP
#include "threadpool/Job.h"
#include "typing/DTypes.h"

#include "threadpool/ZetaSession.h"

inline void ThreadPool::ThreadLoop() {
  /*
  The infinite loop function. This is a while (true) loop waiting for the task queue to open up.
        Each thread should be running its own infinite loop, constantly waiting for new tasks to grab and run.
        */
  while (true) {
    BaseJob *job;
    {
      std::unique_lock<std::mutex> lock(queue_mutex);
      mutex_condition.wait(lock, [this] { return !jobs.empty() || should_terminate; });
      if (should_terminate && jobs.empty()) {
        return;
      }
      job = jobs.front();
      jobs.pop();
    }
    // Execute job
    job->Run();
  }
}

inline void ThreadPool::Start(int num_threads) {
  int threads;
  if (num_threads == -1) // assign default number of threads if none is passed as argument.
    threads = std::thread::hardware_concurrency() - 2;
  else
    threads = num_threads;
  for (int i = 0; i < threads; i++) {
    // Each execution thread is running the ThreadLoop member function.
    thread_pool.push_back(std::thread(&ThreadPool::ThreadLoop,
                                      this)); // Implicitly, member function's first argument is a pointer that refers to itself or some
                                              // instance of the same class type.
  }
}

inline void ThreadPool::QueueJob(BaseJob &job) {
  // Add a new job to the pool; use a lock so that there isn't a data race.
  // thread_pool->QueueJob([] { /* ... */ });
  {
    std::unique_lock<std::mutex> lock(queue_mutex);
    jobs.push(&job); // Passing in address
  }
  mutex_condition.notify_one();
}

inline bool ThreadPool::Busy() {
  /*
        The busy() function can be used in a while loop, such that the main thread can wait the threadpool to complete all
  the tasks before calling the threadpool destructor.
           */
  bool poolbusy;
  {
    std::unique_lock<std::mutex> lock(queue_mutex);
    poolbusy = jobs.empty();
  }
  return !poolbusy;
}

inline void ThreadPool::Stop() {
  // Stop the pool.
  {
    std::unique_lock<std::mutex> lock(queue_mutex);
    should_terminate = true;
  }
  mutex_condition.notify_all();
  for (std::thread &active_thread : thread_pool) {
    active_thread.join();
  }
  thread_pool.clear();
}

inline ZetaSession::ZetaSession(int thread_cnt) : num_threads{thread_cnt} {
  pool.Start(thread_cnt); // Start thread pool
}

inline ZetaSession::~ZetaSession() { pool.Stop(); } // Releases resources

inline Status<NumericVariant> ZetaSession::SubmitTask(NumericVariant (*func)()) {
  Job<NumericVariant> *task = new Job<NumericVariant>{func};
  pool.QueueJob(*task);
  return Status<NumericVariant>(task);
}

template <Numeric T> inline Status<T> ZetaSession::SubmitTask(T (*func)()) {
  /* This method is used to submit a 0 parameter function to the thread pool. A status object is created on the Heap, and a
   * pointer to this object is returned to the caller.
   *
   * Notes:
   * - Cannot return a copy of this object. This will create object on the stack instead and create an issue for the queue, which takes in
   *   pointers only. i.e. Job object resource will be released before completion.
   * */
  JobNoParam<T> *task = new JobNoParam<T>{func};
  pool.QueueJob(*task);
  return Status<T>(task);
}

template <Numeric T> inline Status<T> ZetaSession::SubmitTask(T (*func)(const T), T arg1) {
  /* This method is used to submit a 1 parameter function to the thread pool. A status object is created on the Heap, and a
   * pointer to this object is returned to the caller.
   *
   * Notes:
   * - Cannot return a copy of this object. This will create object on the stack instead and create an issue for the queue, which takes in
   *   pointers only. i.e. Job object resource will be released before completion.
   * */
  JobOneParam<T> *task = new JobOneParam<T>{func, arg1};
  pool.QueueJob(*task);
  return Status<T>(task);
}

template <Numeric T> inline Status<T> ZetaSession::SubmitTask(T (*func)(const T, const T), T arg1, T arg2) {
  /* This method is used to submit a 2 parameter function to the thread pool. A status object is created on the Heap, and a
   * pointer to this object is returned to the caller.
   *
   * Notes:
   * - Cannot return a copy of this object. This will create object on the stack instead and create an issue for the queue, which takes in
   *   pointers only. i.e. Job object resource will be released before completion.
   * */
  JobTwoParam<T> *task = new JobTwoParam<T>{func, arg1, arg2};
  pool.QueueJob(*task);
  return Status<T>(task);
}

template <Numeric T> inline Status<T> ZetaSession::SubmitTask(T *(*func)(const T *, const int), T const *arg1, int len1) {
  JobOneArray<T> *task = new JobOneArray<T>{func, arg1, len1};
  pool.QueueJob(*task);
  return Status<T>(task);
}

template <Numeric T>
inline Status<T> ZetaSession::SubmitTask(T *(*func)(const T *, const T *, const int, const int), T const *arg1, T const *arg2, int len1, int len2) {
  /* This method is used to submit a 2 parameter array function to the thread pool. A status object is created on the Heap, and a
   * pointer to this object is returned to the caller.
   *
   * Notes:
   * - Cannot return a copy of this object. This will create object on the stack instead and create an issue for the queue, which takes in
   *   pointers only. i.e. Job object resource will be released before completion.
   * */

  JobTwoArray<T> *task = new JobTwoArray<T>{func, arg1, arg2, len1, len2};
  pool.QueueJob(*task);
  return Status<T>(task);
}

template <Numeric T>
inline Status<T> ZetaSession::SubmitTask(T *(*func)(const T *, T **, const int, const int), T *arg1, T **arg2, int len1, int len2) {
  /* OVERLOAD
   * This method is used to submit a 2 parameter array function to the thread pool. A status object is created on the Heap, and a
   * pointer to this object is returned to the caller.
   *
   * Notes:
   * - Cannot return a copy of this object. This will create object on the stack instead and create an issue for the queue, which takes in
   *   pointers only. i.e. Job object resource will be released before completion.
   * - arg2 is a pointer to a pointer to an array.
   * */
  JobTwoArray<T> *task = new JobTwoArray<T>{func, arg1, arg2, len1, len2};
  pool.QueueJob(*task);
  return Status<T>(task);
}

template <Numeric T> inline Status<T> ZetaSession::SubmitTask(T *(*func)(const T *, const int, const T), T const *arg1, int len1, T cons) {
  JobOneArrayC<T> *task = new JobOneArrayC<T>(func, arg1, len1, cons);
  pool.QueueJob(*task);
  return Status<T>(task);
}

template <Numeric T> inline T Status<T>::GetResults() {
  std::future<T> out;
  _data->GetFuture(out);
  return out.get();
}

template <Numeric T> inline std::future<T> Status<T>::GetFuture() {
  std::future<T> out{};
  _data->GetFuture(out);
  return std::move(out);
}

template <Numeric T> inline std::future<T *> Status<T>::GetFuturePtr() {
  std::future<T *> out{};
  _data->GetFuture(out);
  return std::move(out);
}

template <Numeric T> inline T *Status<T>::GetResultsPtr() {
  std::future<T *> out;
  _data->GetFuture(out);
  return out.get();
}

template <Numeric T> inline Status<T>::Status(BaseJob *job) : _data{job} {}

template <Numeric T> inline Status<T>::Status(Status<T> &&other) {
  _data = other._data;
  other._data = NULL;
}

template <Numeric T> inline Status<T> &Status<T>::operator=(Status<T> &&other) {
  delete _data;
  _data = other._data;
  other._data = NULL;
  return *this;
}

template <Numeric T> inline Status<T>::Status() { _data = NULL; }

template <Numeric T> inline Status<T>::~Status() { delete _data; }

inline void ZetaSession::SubmitTask(BaseJob &task) { pool.QueueJob(task); }
inline bool ZetaSession::Busy() { return pool.Busy(); }
inline void ZetaSession::StartPool() { pool.Start(); }
inline void ZetaSession::ShutdownPool() { pool.Stop(); }
inline int ZetaSession::Size() { return num_threads; }

#endif
