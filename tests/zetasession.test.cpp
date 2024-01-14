#include "threadpool/ZetaSession.h"
#include <chrono>
#include <gtest/gtest.h>
#include <iostream>
#include <thread>

TEST(Status, initialization) {
  JobNoParam<int> *task1 = new JobNoParam<int>{};
  Status<int> test1{};
  Status<int> test2{task1};
  JobNoParam<float> *task2 = new JobNoParam<float>{};
  Status<float> test3{task2};

  JobNoParam<NumericVariant> *task3 = new JobNoParam<NumericVariant>{};
  Status<NumericVariant> test4{task3};
}

template <Numeric T> T simp_func() { return T{10}; }

template <Numeric T> T one_func(T a) { return a * 2; }
template <> NumericVariant one_func(NumericVariant a) { return std::get<int>(a) * 2; }

TEST(Status, get_future) {
  JobNoParam<int> *task1 = new JobNoParam<int>{&simp_func};
  Status<int> test2{task1};
  auto res = test2.GetFuture();

  JobNoParam<float> *task2 = new JobNoParam<float>{&simp_func};
  Status<float> test3{task2};
  auto res2 = test3.GetFuture();

  JobNoParam<NumericVariant> *task3 = new JobNoParam<NumericVariant>{&simp_func};
  Status<NumericVariant> test4{task3};
  auto res3 = test4.GetFuture();

  JobOneParam<int> *task4 = new JobOneParam<int>{&one_func, 10};
  Status<int> test5{task4};
  auto res4 = test5.GetFuture();

  JobOneParam<float> *task5 = new JobOneParam<float>{&one_func, 20.0};
  Status<float> test6{task5};
  auto res5 = test6.GetFuture();

  JobOneParam<NumericVariant> *task6 = new JobOneParam<NumericVariant>{&one_func, NumericVariant{20}};
  Status<NumericVariant> test7{task6};
  auto res6 = test7.GetFuture();
}

template <int_or_float T> T *simp_func_a(const T *a, const int s) {
  T *out = new T[2]{1, 2};
  return out;
}

TEST(Status, get_future_ptr) {
  int *arr1 = new int[5]{1, 2, 3, 4, 5};
  float *arr2 = new float[5]{1, 2, 3, 4, 5};
  int size = 5;
  JobOneArray<int> *task4 = new JobOneArray<int>{&simp_func_a, arr1, size};
  Status<int> test5{task4};
  auto res4 = test5.GetFuture();

  JobOneArray<float> *task5 = new JobOneArray<float>{&simp_func_a, arr2, size};
  Status<float> test6{task5};
  auto res5 = test6.GetFuture();
}

int sleep_task() {
  std::this_thread::sleep_for(std::chrono::milliseconds(500)); // Half a second.
  return 1;
}

TEST(ThreadPool, core) {
  // Requires visual inspection. Test should complete in .5 seconds.
  ThreadPool mypool;
  mypool.Start(4);
  JobNoParam<int> myjob{&sleep_task};
  mypool.QueueJob(myjob);
  while (true) {
    if (mypool.Busy())
      continue;
    else
      break;
  }
  mypool.Stop();
}

TEST(ThreadPool, core_async) {
  // Requires visual inspection. Test should complete in .5 seconds.
  ThreadPool mypool;
  mypool.Start(4);
  JobNoParam<int> myjob{&sleep_task};
  JobNoParam<int> myjob2{&sleep_task};
  JobNoParam<int> myjob3{&sleep_task};
  JobNoParam<int> myjob4{&sleep_task};
  mypool.QueueJob(myjob);
  mypool.QueueJob(myjob2);
  mypool.QueueJob(myjob3);
  mypool.QueueJob(myjob4);
  while (true) {
    if (mypool.Busy())
      continue;
    else
      break;
  }
  mypool.Stop();
}

TEST(ZetaSession, core) {
  // Requires visual inspection. Test should complete in .5 seconds.
  ZetaSession myzeta(4);
  Status<int> res = myzeta.SubmitTask(&sleep_task);
  Status<int> res2 = myzeta.SubmitTask(&sleep_task);
  Status<int> res3 = myzeta.SubmitTask(&sleep_task);
  Status<int> res4 = myzeta.SubmitTask(&sleep_task);
  while (true) {
    if (myzeta.Busy()) {
      continue;
    } else
      break;
  }
  EXPECT_EQ(res.GetResults(), 1);
  EXPECT_EQ(res2.GetResults(), 1);
  EXPECT_EQ(res3.GetResults(), 1);
  EXPECT_EQ(res4.GetResults(), 1);
}
