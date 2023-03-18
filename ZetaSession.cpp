#include <any>
#include <iostream>
#include <queue>
#include <thread>
#include <variant>
#include <vector>

template <typename Typ>
concept Numeric = std::is_floating_point_v<Typ> || std::numeric_limits<Typ>::is_integer;

struct Result {
  std::variant<int, float> value;
  const char *val_type{typeid(value).name()}; // type of value object. i.e. 'i' or 'f'.
};

class Job {

public:
  Job(int (*func_ptr)(const void *), void *args);
  Job(int (*func_ptr)());
  ~Job();
  virtual int Run();

private:
  int (*_func_ptr)(const void *);
  int (*_func_ptr_no_args)();
  const void *_args;
};

Job::Job(int (*func_ptr)(const void *), void *args) : _func_ptr{func_ptr}, _args{args} {}
Job::Job(int (*func_ptr)()) : _func_ptr_no_args{func_ptr}, _args{NULL} {}
Job::~Job() {}
int Job::Run() {
  // Executes function as pass in arguments.
  if (_args)
    return (*_func_ptr)(_args);
  else
    return (*_func_ptr_no_args)();
}

class ThreadPool {

public:
  void Start(int num_threads);
  template <typename arg_type>
    requires Numeric<arg_type>
  void QueueJob(const std::function<std::variant<int, float>(arg_type a, arg_type b)> job);
  void QueueJob(std::function<void()> task);
  void QueueJob(Job job);
  void Stop();
  bool Busy();

private:
  void ThreadLoop();
  bool should_terminate = false;           // Tells threads to stop looking for jobs
  std::mutex queue_mutex;                  // Prevents data races to the job queue
  std::condition_variable mutex_condition; // Allows threads to wait on new jobs or termination
  std::vector<std::thread> thread_pool;
  // std::queue<std::function<std::variant<int, float>()>> jobs;
  std::queue<Job> jobs;
};

void ThreadPool::ThreadLoop() {
  /*
  The infinite loop function. This is a while (true) loop waiting for the task queue to open up.
        Each thread should be running its own infinite loop, constantly waiting for new tasks to grab and run.
        */
  while (true) {
    // std::function<std::variant<int, float>()> job;
    Job *job;
    int results;
    {
      std::unique_lock<std::mutex> lock(queue_mutex);
      mutex_condition.wait(lock, [this] { return !jobs.empty() || should_terminate; });
      std::cout << std::this_thread::get_id() << '\n';
      if (should_terminate) {
        return;
      }
      job = &jobs.front();
      jobs.pop();
    }
    std::cout << "At execution!" << '\n';
    results = job->Run(); // Execute job and add output to output queue.
    std::cout << results << '\n';
  }
}

void ThreadPool::Start(int num_threads = -1) {
  int threads;
  if (num_threads == -1)                           // assign default number of threads of none is passed as argument.
    threads = std::thread::hardware_concurrency(); // Max # of threads the system supports
  else
    threads = num_threads;
  for (int i = 0; i < num_threads; i++) {
    // Each execution thread is running the ThreadLoop member function.
    thread_pool.push_back(std::thread(&ThreadPool::ThreadLoop,
                                      this)); // Implicitly, member function's first argument is a pointer that refers to itself or some
                                              // instance of the same class type.
  }
}

template <typename arg_type>
  requires Numeric<arg_type>
void ThreadPool::QueueJob(const std::function<std::variant<int, float>(arg_type a, arg_type b)> job) {
  // Add a new job to the pool; use a lock so that there isn't a data race.
  // thread_pool->QueueJob([] { /* ... */ });
  {
    std::unique_lock<std::mutex> lock(queue_mutex);
    jobs.push(job);
  }
  mutex_condition.notify_one();
}

void ThreadPool::QueueJob(std::function<void()> task) {}

void ThreadPool::QueueJob(Job job) {
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
  ~ZetaSession() { pool.Stop(); }
  bool Busy();
  void SubmitTask(std::function<void()> task);
  void SubmitTask(Job task);
  int Size() { return num_threads; }
  void StartPool();
  void ShutdownPool();

private:
  int num_threads;
  ThreadPool pool;
};

void ZetaSession::SubmitTask(std::function<void()> task) { pool.QueueJob(task); }
void ZetaSession::SubmitTask(Job task) { pool.QueueJob(task); }
bool ZetaSession::Busy() { return pool.Busy(); }
void ZetaSession::StartPool() { pool.Start(); }
void ZetaSession::ShutdownPool() { pool.Stop(); }

void some_task(int value, int *to_return) { *to_return = value + 10; }
int testing_task() {
  std::cout << "Hi!" << '\n';
  return 0;
}

int main() {
  ZetaSession newZeta{10};
  int *out;
  some_task(10, out);
  Job task{&testing_task};
  newZeta.SubmitTask(task);
  while (true) {
    if (newZeta.Busy()) {
      std::cout << "Still busy!" << '\n';
      continue;
    } else {
      break;
    }
  }
  newZeta.ShutdownPool();
  std::cout << "Success!" << '\n';
  return 0;
}
