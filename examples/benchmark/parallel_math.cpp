#include "threadpool/ZetaSession.h"
#include <chrono>
#include <iostream>
#include <list>

ZetaSession newZeta{11};
/* N_SMPLS simulates the number of tasks to run. For example, N_SMPLS = 10 means there are 10 tasks created in the threadpool.
 * LEN_ARRAY simulates the number of values in a fake summation. For example, LEN_ARRAY = 100 means to execute a summation 100 * 6 times.
 *
 * */
int N_SMPLS = 3456;
int LEN_ARRAY = 4608;

float long_add() {
  float out = 0.0f;
  float temp = 4809.1111f;
  for (int i = 0; i < LEN_ARRAY; i++) {
    out = out + temp;
  }
  return out;
}

std::chrono::duration<double, std::milli> base_trial() {
  auto ti1 = std::chrono::high_resolution_clock::now();
  int val = 0;
  for (int i = 0; i < N_SMPLS; i++) {
    val = long_add();
  }
  auto ti2 = std::chrono::high_resolution_clock::now();

  return std::chrono::duration<double, std::milli>{ti2 - ti1};
}

std::chrono::duration<double, std::milli> benchmark3() {
  auto t1 = std::chrono::high_resolution_clock::now();
  float out = 0;
  std::list<Status<float> *> out_arr{};
  for (int i = 0; i < N_SMPLS; i++) {
    Status<float> *temp = new Status<float>(newZeta.SubmitTask(&long_add)); // note pointer temp is a local variable.
    out_arr.push_back(temp);
  }
  while (newZeta.Busy()) {
    continue;
  }
  auto t2 = std::chrono::high_resolution_clock::now();
  return std::chrono::duration<double, std::milli>{t2 - t1};
}

int main() {
  std::cout << "Number of tasks: " << N_SMPLS << '\n';
  std::cout << "Number of Summations: " << LEN_ARRAY << '\n';
  std::chrono::duration<double, std::milli> times[10];
  std::chrono::duration<double, std::milli> trials[10];
  for (int i = 0; i < 10; i++) {
    times[i] = base_trial();
    trials[i] = benchmark3();
  }
  double sum = 0;
  for (int i = 0; i < 10; i++) {
    sum += times[i].count();
  }
  std::cout << "Serial calculation runtime: " << sum / 10 << '\n';
  double summ = 0;
  for (int i = 0; i < 10; i++) {
    summ += trials[i].count();
  }
  std::cout << "Multi-threaded calculation runtime: " << summ / 10 << '\n';
  std::cout << "Fractional improvement: " << sum / summ << '\n';
}
