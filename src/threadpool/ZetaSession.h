#include "threadpool/Job.h"
#include "typing//DTypes.h"
#include <future>
#include <queue>
#include <thread>
#include <variant>
#include <vector>

#ifndef ZETASESSION_H
#define ZETASESSION_H

class ThreadPool {

public:
  void Start(int num_threads = -1);
  void QueueJob(Job<NumericVariant> job);
  void Stop();
  bool Busy();

private:
  void ThreadLoop();
  bool should_terminate = false;           // Tells threads to stop looking for jobs
  std::mutex queue_mutex;                  // Prevents data races to the job queue; must aquire lock to interact with queue.
  std::condition_variable mutex_condition; // Allows threads to wait on new jobs or termination
  std::vector<std::thread> thread_pool;
  std::queue<Job<NumericVariant>> jobs;
};

class ZetaSession {
public:
  ZetaSession(int thread_cnt);
  ~ZetaSession(); // Releases resources
  bool Busy();
  std::future<NumericVariant> SubmitTask(NumericVariant (*func)());
  void SubmitTask(Job<NumericVariant> task);
  template <int_or_float T> std::future<NumericVariant> SubmitTask(T (*func)());
  template <int_or_float T> std::future<NumericVariant> SubmitTask(T (*func)(const T *), T *arg1);
  template <int_or_float T> std::future<NumericVariant> SubmitTask(T (*func)(const T *, const T *), T *arg1, T *arg2);
  int Size();
  void StartPool();
  void ShutdownPool();

private:
  int num_threads;
  ThreadPool pool;
};

#include "threadpool/ZetaSession.tpp"
#endif
