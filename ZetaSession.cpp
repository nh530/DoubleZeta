#include <any>
#include <iostream>
#include <queue>
#include <thread>
#include <variant>

template <typename Typ>
concept Numeric = std::is_floating_point_v<Typ> || std::numeric_limits<Typ>::is_integer;

struct Result {
  std::variant<int, float> value;
  const char *val_type{typeid(value).name()}; // type of value object. i.e. 'i' or 'f'.
};

class ThreadPool {

public:
  void Start(int num_threads);
  template <typename arg_type>
    requires Numeric<arg_type>
  void QueueJob(const std::function<std::variant<int, float>(arg_type)> &job);
  void Stop();
  bool Busy();

private:
  void ThreadLoop();
  bool should_terminate = false;           // Tells threads to stop looking for jobs
  std::mutex queue_mutex;                  // Prevents data races to the job queue
  std::condition_variable mutex_condition; // Allows threads to wait on new jobs or termination
  std::vector<std::thread> thread_pool;
  std::queue<std::function<std::variant<int, float>()>> jobs;
  std::queue<Result> outputs;
};

void ThreadPool::ThreadLoop() {
  /*
  The infinite loop function. This is a while (true) loop waiting for the task queue to open up.
        Each thread should be running its own infinite loop, constantly waiting for new tasks to grab and run.
        */
  while (true) {
    std::function<std::variant<int, float>()> job;
    {
      std::unique_lock<std::mutex> lock(queue_mutex);
      mutex_condition.wait(lock, [this] { return !jobs.empty() || should_terminate; });
      if (should_terminate) {
        return;
      }
      job = jobs.front();
      jobs.pop();
    }
    outputs.push(Result{job()}); // Execute job and add output to output queue.
  }
}

void ThreadPool::Start(int num_threads = -1) {
  if (num_threads == -1)                               // assign default number of threads of none is passed as argument.
    num_threads = std::thread::hardware_concurrency(); // Max # of threads the system supports
  thread_pool.resize(num_threads);                     // vector
  for (int i = 0; i < num_threads; i++) {
    // Each execution thread is running the ThreadLoop member function.
    std::cout << i << '\n';
    thread_pool.push_back(std::thread(&ThreadPool::ThreadLoop,
                                      this)); // Implicitly, member function's first argument is a pointer that refers to itself or some
                                              // instance of the same class type.
  }
}

template <typename arg_type>
  requires Numeric<arg_type>
void ThreadPool::QueueJob(const std::function<std::variant<int, float>(arg_type)> &job) {
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
  return poolbusy;
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

class ZetaSession { // Todo: enforce single session object.
public:
  ZetaSession(int thread_cnt) : num_threads{thread_cnt} {
    pool->Start(); // Start thread pool
  }
  ~ZetaSession() { pool->Stop(); }
  void Busy();
  template <typename arg_type>
    requires Numeric<arg_type>
  void SubmitTask(const std::function<void(arg_type)> &task);
  int Size() { return num_threads; }
  static ZetaSession GetSession(); // If there is an active session, return that one.

private:
  int num_threads;
  ThreadPool *pool;
  static ZetaSession curr_session;
};

template <typename arg_type>
  requires Numeric<arg_type>
void ZetaSession::SubmitTask(const std::function<void(arg_type)> &task) {
  pool->QueueJob(task);
}

void ZetaSession::Busy() { pool->Busy(); }

int main() {
  ZetaSession newZeta{10};
  using namespace std;
  cout << newZeta.Size() << '\n';
  return 0;
}
