#ifndef ZETASESSION_H
#define ZETASESSION_H
#include "threadpool/Job.h"
#include "typing/DTypes.h"
#include <future>
#include <queue>
#include <thread>
#include <variant>
#include <vector>

class ThreadPool {

public:
  void Start(int num_threads = -1);
  void QueueJob(BaseJob &job);
  void Stop();
  bool Busy();

private:
  void ThreadLoop();
  bool should_terminate = false;           // Tells threads to stop looking for jobs
  std::mutex queue_mutex;                  // Prevents data races to the job queue; must aquire lock to interact with queue.
  std::condition_variable mutex_condition; // Allows threads to wait on new jobs or termination
  std::vector<std::thread> thread_pool;
  std::queue<BaseJob *> jobs;
};

template <Numeric T> class Status {
  /* This user defined type is to provide the end user a controlled way to check when the job is done.
   * The future object can be returned directly, however, there will be no way to correctly achieve the deletion of Job objects because
   * DoubleZeta to know when the end-user is done with the results or when the end-user collects the results.
   * Therefore, freeing up the resource consumed by the Job is left up to the user via deletion of this Status object.
   *
   * Notes:
   * - Something like shared pointers can be used to manage the memory of the Job object. However, it was decided to keep the application as lean
   *   as possible.
   * - If the submitted function is suppose to return a pointer, then use the GetResultsPtr or GetFuturePtr methods to get the finished results.
   * - If the submitted function just returns a value, then use the GetResults or GetFuture methods to get the finished results.
   * */
public:
  T GetResults();
  T *GetResultsPtr();
  std::future<T> GetFuture();
  std::future<T *> GetFuturePtr();
  Status(BaseJob *);
  Status();
  Status(Status<T> &&other);
  Status<T> &operator=(Status<T> &&other);
  Status(Status<T> &other) = delete;
  Status<T> &operator=(Status<T> &other) = delete;
  ~Status();

private:
  BaseJob *_data;
};

class ZetaSession {
public:
  ZetaSession(int thread_cnt);
  ~ZetaSession();
  bool Busy();
  Status<NumericVariant> SubmitTask(NumericVariant (*func)());
  void SubmitTask(BaseJob &task);
  template <Numeric T> Status<T> SubmitTask(T (*func)());
  template <Numeric T> Status<T> SubmitTask(T (*func)(const T), T arg1);
  template <Numeric T> Status<T> SubmitTask(T (*func)(const T, const T), T arg1, T arg2);
  template <Numeric T> Status<T> SubmitTask(T *(*func)(const T *, const int, const T), T const *arg1, int len1, T cons);
  template <Numeric T> Status<T> SubmitTask(T *(*func)(const T *, const int), T const *arg1, int len1);
  template <Numeric T> Status<T> SubmitTask(T *(*func)(const T *, const T *, const int, const int), T const *arg1, T const *arg2, int len1, int len2);
  template <Numeric T> Status<T> SubmitTask(T *(*func)(const T *, T **, const int, const int), T *arg1, T **arg2, int len1, int len2);
  int Size();
  void StartPool();
  void ShutdownPool();

private:
  int num_threads;
  ThreadPool pool;
};

#include "threadpool/ZetaSession.tpp"
#endif
