/* The goal of these user defined types is to create a multi-threaded, parallel and concurrent, framework that can process
 * commonly found algebraic operations in mathematics. Fundamental algebra operations can be found in in the Algebra header file.
 * From which, more complex mathematical equations can be represented in a manner that supports parallel processing.
 *
 * Notes: It is difficult to implement the parallel computing of a single math equation. What is easily doable is to implement the
 * parallel computing of many equations.
 */

#include "threadpool/Job.h"
#include "typing/DTypes.h"
#include <future>
#include <iostream>
#include <queue>
#include <thread>
#include <variant>

#ifndef ZETASESSION_TPP
#define ZETASESSION_TPP

#include "threadpool/ZetaSession.h"

void ThreadPool::ThreadLoop() {
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

void ThreadPool::Start(int num_threads) {
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

void ThreadPool::QueueJob(BaseJob &job) {
  // Add a new job to the pool; use a lock so that there isn't a data race.
  // thread_pool->QueueJob([] { /* ... */ });
  {
    std::unique_lock<std::mutex> lock(queue_mutex);
    jobs.push(&job); // Passing in address
  }
  mutex_condition.notify_one();
}

bool ThreadPool::Busy() {
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

void ThreadPool::Stop() {
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

ZetaSession::ZetaSession(int thread_cnt) : num_threads{thread_cnt} {
  pool.Start(thread_cnt); // Start thread pool
}

ZetaSession::~ZetaSession() { pool.Stop(); } // Releases resources

Status<NumericVariant> *ZetaSession::SubmitTask(NumericVariant (*func)()) {
  Job<NumericVariant> *task = new Job<NumericVariant>{func};
  pool.QueueJob(*task);
  Status<NumericVariant> *out = new Status<NumericVariant>{._data = task};
  return out;
}

template <Numeric T> Status<T> *ZetaSession::SubmitTask(T (*func)()) {
  /* This method is used to submit a 0 parameter function to the thread pool. A status object is created on the Heap, and a
   * pointer to this object is returned to the caller.
   *
   * Notes:
   * - Cannot return a copy of this object. This will create object on the stack instead and create an issue for the queue, which takes in
   *   pointers only. i.e. Job object resource will be released before completion.
   * */
  Job<T> *task = new Job<T>{func};
  pool.QueueJob(*task);
  Status<T> *out = new Status<T>{._data = task};
  return out;
}

template <Numeric T> Status<T> *ZetaSession::SubmitTask(T (*func)(const T *), T *arg1) {
  /* This method is used to submit a 1 parameter function to the thread pool. A status object is created on the Heap, and a
   * pointer to this object is returned to the caller.
   *
   * Notes:
   * - Cannot return a copy of this object. This will create object on the stack instead and create an issue for the queue, which takes in
   *   pointers only. i.e. Job object resource will be released before completion.
   * */
  Job<T> *task = new Job<T>{func, arg1};
  pool.QueueJob(*task);
  Status<T> *out = new Status<T>{._data = task};
  return out;
}

template <Numeric T> Status<T> *ZetaSession::SubmitTask(T (*func)(const T *, const T *), T *arg1, T *arg2) {
  /* This method is used to submit a 2 parameter function to the thread pool. A status object is created on the Heap, and a
   * pointer to this object is returned to the caller.
   *
   * Notes:
   * - Cannot return a copy of this object. This will create object on the stack instead and create an issue for the queue, which takes in
   *   pointers only. i.e. Job object resource will be released before completion.
   * */
  Job<T> *task = new Job<T>{func, arg1, arg2};
  pool.QueueJob(*task);
  Status<T> *out = new Status<T>{._data = task};
  return out;
}

template <Numeric T> T Status<T>::GetResults() {
  std::future<T> out;
  _data->GetFuture(out);
  return std::get<T>(out);
}

template <Numeric T> std::future<T> Status<T>::GetFuture() {
  std::future<T> out{};
  _data->GetFuture(out);
  return out;
}

template <Numeric T> std::future<T *> Status<T>::GetFuturePtr() {
  std::future<T *> out{};
  _data->GetFuture(out);
  return out;
}

template <Numeric T> T *Status<T>::GetResultsPtr() {
  std::future<T *> out;
  _data->GetFuture(out);
  return std::get<T *>(out);
}

template <Numeric T> Status<T>::~Status() { delete _data; }

void ZetaSession::SubmitTask(BaseJob &task) { pool.QueueJob(task); }
bool ZetaSession::Busy() { return pool.Busy(); }
void ZetaSession::StartPool() { pool.Start(); }
void ZetaSession::ShutdownPool() { pool.Stop(); }
int ZetaSession::Size() { return num_threads; }

#endif
