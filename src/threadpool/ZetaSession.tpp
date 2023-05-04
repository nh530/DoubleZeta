/* The goal of these user defined types is to create a multi-threaded, parallel and concurrent, framework that can process
 * commonly found algebraic operations in mathematics. Fundamental algebra operations can be found in in the Algebra header file.
 * From which, more complex mathematical equations can be represented in a manner that supports parallel processing.
 *
 * Notes: It is difficult to implement the parallel computing of a single math equation. What is easily doable is to implement the
 * parallel computing of many equations.
 */

#include "Job.h"
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
    {
      std::unique_lock<std::mutex> lock(queue_mutex);
      mutex_condition.wait(lock, [this] { return !jobs.empty() || should_terminate; });
      if (should_terminate && jobs.empty()) {
        return;
      }
      Job<NumericVariant> job = std::move(jobs.front());
      jobs.pop();
      job.Run(); // Execute job
    }
  }
}

void ThreadPool::Start(int num_threads) {
  int threads;
  if (num_threads == -1) // assign default number of threads if none is passed as argument.
    threads = std::thread::hardware_concurrency() - 2;
  else
    threads = num_threads;
  for (int i = 0; i < num_threads; i++) {
    // Each execution thread is running the ThreadLoop member function.
    thread_pool.push_back(std::thread(&ThreadPool::ThreadLoop,
                                      this)); // Implicitly, member function's first argument is a pointer that refers to itself or some
                                              // instance of the same class type.
  }
}

void ThreadPool::QueueJob(Job<NumericVariant> job) {
  // Add a new job to the pool; use a lock so that there isn't a data race.
  // thread_pool->QueueJob([] { /* ... */ });
  {
    std::unique_lock<std::mutex> lock(queue_mutex);
    jobs.push(std::move(job));
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

std::future<NumericVariant> ZetaSession::SubmitTask(NumericVariant (*func)()) {
  Job<NumericVariant> task{func};
  std::future<NumericVariant> out = task.GetFuture();
  pool.QueueJob(std::move(task));
  return out;
}
template <int_or_float T> std::future<NumericVariant> ZetaSession::SubmitTask(T (*func)()) {
  Job<NumericVariant> task{func};
  std::future<NumericVariant> out = task.GetFuture();
  pool.QueueJob(std::move(task));
  return out;
}
template <int_or_float T> std::future<NumericVariant> ZetaSession::SubmitTask(T (*func)(const T *), T *arg1) {
  Job<NumericVariant> task{func, arg1};
  std::future<NumericVariant> out = task.GetFuture();
  pool.QueueJob(std::move(task));
  return out;
}
template <int_or_float T> std::future<NumericVariant> ZetaSession::SubmitTask(T (*func)(const T *, const T *), T *arg1, T *arg2) {
  Job<NumericVariant> task{func, arg1, arg2};
  std::future<NumericVariant> out = task.GetFuture();
  pool.QueueJob(std::move(task));
  return out;
}

void ZetaSession::SubmitTask(Job<NumericVariant> task) { pool.QueueJob(std::move(task)); }
bool ZetaSession::Busy() { return pool.Busy(); }
void ZetaSession::StartPool() { pool.Start(); }
void ZetaSession::ShutdownPool() { pool.Stop(); }
int ZetaSession::Size() { return num_threads; }

#endif
