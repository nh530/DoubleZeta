#include <any>
#include <iostream>
#include <queue>
#include <thread>
#include <unistd.h>
#include <variant>
#include <vector>

template <typename T> constexpr bool is_float() { return std::is_floating_point_v<T>; }
template <typename T> constexpr bool is_int() { return std::numeric_limits<T>::is_integer; }
using NumericVariant = std::variant<float, int>;
template <typename T> struct is_numerc_variant : public std::false_type {};
template <> struct is_numerc_variant<NumericVariant> : public std::true_type {};

template <typename T>
concept Numeric = is_float<T>() || is_int<T>() || is_numerc_variant<T>::value;

template <Numeric T> class Job {

public:
  Job(T (*func_ptr)(const T *), T *arg1); // Function takes a single pointer of any type T and returns the same type.
  Job(T (*func_ptr)());                   // Function takes no argument and returns type T.
  Job(T (*func_ptr)(const T *, const T *), T *arg1, T *arg2);
  ~Job();
  virtual T Run();

private:
  std::function<T(const T *, const T *)> _func_ptr_2_args;
  std::function<T(const T *)> _func_ptr;
  std::function<T()> _func_ptr_no_args;
  const T *_args;
  const T *_args2;
};

template <Numeric T> Job<T>::Job(T (*func_ptr)(const T *), T *arg1) : _func_ptr{func_ptr}, _args{arg1}, _args2{NULL} {}
template <Numeric T> Job<T>::Job(T (*func_ptr)()) : _func_ptr_no_args{func_ptr}, _args{NULL} {}
template <Numeric T> Job<T>::Job(T (*func_ptr)(const T *, const T *), T *arg1, T *arg2) : _func_ptr_2_args{func_ptr}, _args{arg1}, _args2{arg2} {}
template <Numeric T> Job<T>::~Job() {}
template <Numeric T> T Job<T>::Run() {
  // Executes function and pass in arguments.
  if (_args && _args2) {
    return (_func_ptr_2_args)(_args, _args2);
  } else if (_args) {
    return (_func_ptr)(_args);
  } else
    return (_func_ptr_no_args)();
}

template <> class Job<NumericVariant> {
public:
  Job(NumericVariant (*func_ptr)(const NumericVariant *), NumericVariant *arg1) : _func_ptr{func_ptr}, _args{arg1}, _args2{NULL} {}
  Job(NumericVariant (*func_ptr)()) : _func_ptr_no_args{func_ptr}, _args{NULL}, _args2{NULL} {}
  Job(NumericVariant (*func_ptr)(const NumericVariant), NumericVariant *arg1, NumericVariant *arg2);
  Job(float (*func_ptr)()) : _args{NULL}, _args2{NULL} {
    // Handle the conversion from Job<float> to Job<NumericVariant> by creating a new function that outputs NumericVariant.
    std::function<NumericVariant()> temp = [func_ptr]() -> NumericVariant { return (func_ptr()); };
    _func_ptr_no_args = temp;
  }
  Job(float (*func_ptr)(const float *), float *arg1) : _args2{NULL} {
    std::function<NumericVariant(const NumericVariant *)> temp = [func_ptr](const NumericVariant *x) -> NumericVariant {
      float temp = std::get<float>(*x);
      return (func_ptr(&temp));
    };
    _func_ptr = temp;
    _args = new NumericVariant{*arg1};
  }
  Job(float (*func_ptr)(const float *, const float *), float *arg1, float *arg2) {
    std::function<NumericVariant(const NumericVariant *, const NumericVariant *)> temp = [func_ptr](const NumericVariant *x,
                                                                                                    const NumericVariant *y) -> NumericVariant {
      float t_x = std::get<float>(*x);
      float t_y = std::get<float>(*y);
      return (func_ptr(&t_x, &t_y));
    };
    _func_ptr_2_args = temp;
    _args = new NumericVariant{*arg1};
    _args2 = new NumericVariant{*arg2};
  }

  Job(Job<float> other) {
    // TODO: Access the private function pointer.
  }
  ~Job() {}
  NumericVariant Run() {
    // Executes function and pass in arguments.
    if (_args && _args2) {
      return (_func_ptr_2_args)(_args, _args2);
    } else if (_args) {
      return (_func_ptr)(_args);
    } else
      return (_func_ptr_no_args)();
  }

private:
  std::function<NumericVariant(const NumericVariant *, const NumericVariant *)> _func_ptr_2_args;
  std::function<NumericVariant(const NumericVariant *)> _func_ptr;
  std::function<NumericVariant()> _func_ptr_no_args;
  const NumericVariant *_args;
  const NumericVariant *_args2;
};

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
    Job<NumericVariant> *job;
    {
      std::unique_lock<std::mutex> lock(queue_mutex);
      mutex_condition.wait(lock, [this] { return !jobs.empty() || should_terminate; });
      if (should_terminate && jobs.empty()) {
        return;
      }
      job = &jobs.front();
      jobs.pop();
      std::cout << "At execution!" << '\n';
      auto results = job->Run(); // Execute job and add output to output queue.
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
    std::cout << i << '\n';
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

float testing_task2(const float *x, const float *a) {
  std::cout << *x << '\n';
  std::cout << a << '\n';
  std::cout << "Hi!" << '\n';
  return 0;
}

NumericVariant testing_task(const NumericVariant *x) {
  std::cout << std::get<float>(*x) << '\n';
  std::cout << "Hi!" << '\n';
  return 0;
}

int main() {
  ZetaSession newZeta{10};
  sleep(0);
  // NumericVariant *constt = new NumericVariant{float{20}};
  float constt = float(10);
  float consta = float(1313);
  Job<NumericVariant> task{&testing_task2, &constt, &consta}; // deletes when main function ends.
  newZeta.SubmitTask(task);
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
