#include "Algebra.h"
#include "DTypes.h"
#include "Job.h"
#include <any>
#include <iostream>
#include <queue>
#include <thread>
#include <unistd.h>
#include <variant>
#include <vector>

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
      Job<NumericVariant> job = jobs.front();
      jobs.pop();
      auto results = job.Run(); // Execute job and add output to output queue.
      // std::cout << results << '\n';
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
    jobs.push(job);
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

class ZetaSession {
public:
  ZetaSession(int thread_cnt) : num_threads{thread_cnt} {
    pool.Start(thread_cnt); // Start thread pool
  }
  ~ZetaSession() { pool.Stop(); } // Releases resources
  bool Busy();
  void SubmitTask(NumericVariant (*func)());
  void SubmitTask(Job<NumericVariant> task);
  void SubmitTask(float (*func)());
  void SubmitTask(int (*func)());
  void SubmitTask(float (*func)(const float *), float *arg1);
  int Size() { return num_threads; }
  void StartPool();
  void ShutdownPool();

private:
  int num_threads;
  ThreadPool pool;
};

void ZetaSession::SubmitTask(NumericVariant (*func)()) {
  Job<NumericVariant> task{func};
  pool.QueueJob(task);
}

void ZetaSession::SubmitTask(float (*func)()) {
  Job<float> task{func};
  pool.QueueJob(task);
}

void ZetaSession::SubmitTask(float (*func)(const float *), float *arg1) {
  Job<float> task{func, arg1};
  pool.QueueJob(task);
}

void ZetaSession::SubmitTask(Job<NumericVariant> task) { pool.QueueJob(task); }

bool ZetaSession::Busy() { return pool.Busy(); }

void ZetaSession::StartPool() { pool.Start(); }
void ZetaSession::ShutdownPool() { pool.Stop(); }

void some_task(int value, int *to_return) { *to_return = value + 10; }

int main() {
  ZetaSession newZeta{10};
  sleep(0);
  // NumericVariant *constt = new NumericVariant{float{20}};
  int constt = 10;
  int consta = 1313;
  Job<NumericVariant> task2{&subtract, &constt, &consta};
  newZeta.SubmitTask(task2);

  while (true) {
    if (newZeta.Busy()) {
      std::cout << "Still Busy!" << '\n';
      continue;
    } else {
      break;
    }
  }
  newZeta.ShutdownPool();
  return 0;
}
