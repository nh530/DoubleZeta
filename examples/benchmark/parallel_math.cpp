#include "algba_cmd/Algebra.h"
#include "double_word/dw.h"
#include "optimizer/GradientDescent.h"
#include "regression/Linear.h"
#include "threadpool/ZetaSession.h"
#include <chrono>
#include <iostream>

float long_add() {
  float out = 0;
  float temp = 4809.1111;
  for (int i = 0; i < 10000000; i++) {
    out = out + temp;
  }
  for (int i = 0; i < 10000000; i++) {
    out = out + temp;
  }
  for (int i = 0; i < 10000000; i++) {
    out = out + temp;
  }
  for (int i = 0; i < 10000000; i++) {
    out = out + temp;
  }
  for (int i = 0; i < 10000000; i++) {
    out = out + temp;
  }
  for (int i = 0; i < 10000000; i++) {
    out = out + temp;
  }

  return out;
}

float fast_add() {
  float out = 0.0;
  float val = 1.3;
  return out + val;
}

std::chrono::duration<double, std::milli> base_trial() {
  auto ti1 = std::chrono::high_resolution_clock::now();
  int val;
  for (int i = 0; i < 6; i++) {
    val = long_add();
  }
  auto ti2 = std::chrono::high_resolution_clock::now();

  return std::chrono::duration<double, std::milli>{ti2 - ti1};
}

std::chrono::duration<double, std::milli> benchmark() {
  ZetaSession newZeta{10};
  auto t1 = std::chrono::high_resolution_clock::now();
  std::future<NumericVariant> out = newZeta.SubmitTask(&long_add);
  std::future<NumericVariant> out2 = newZeta.SubmitTask(&long_add);
  std::future<NumericVariant> out3 = newZeta.SubmitTask(&long_add);
  std::future<NumericVariant> out4 = newZeta.SubmitTask(&long_add);
  std::future<NumericVariant> out5 = newZeta.SubmitTask(&long_add);
  std::future<NumericVariant> out6 = newZeta.SubmitTask(&long_add);
  std::future<NumericVariant> out7 = newZeta.SubmitTask(&long_add);
  std::future<NumericVariant> out8 = newZeta.SubmitTask(&long_add);
  std::future<NumericVariant> out9 = newZeta.SubmitTask(&long_add);
  std::future<NumericVariant> out10 = newZeta.SubmitTask(&long_add);
  out2.wait();
  out.wait();
  out3.wait();
  out4.wait();
  out5.wait();
  out6.wait();
  out10.wait();
  out9.wait();
  out7.wait();
  auto t2 = std::chrono::high_resolution_clock::now();
  return std::chrono::duration<double, std::milli>{t2 - t1};
}

int main() {

  auto trial = benchmark();
  auto base = base_trial();
  std::chrono::duration<double, std::milli> times[10];
  std::chrono::duration<double, std::milli> trials[10];
  for (int i = 0; i < 10; i++) {
    times[i] = base_trial();
    trials[i] = benchmark();
  }
  double sum = 0;
  for (int i = 0; i < 10; i++) {
    sum += times[i].count();
  }
  std::cout << sum / 10 << '\n';
  sum = 0;
  for (int i = 0; i < 10; i++) {
    sum += trials[i].count();
  }
  std::cout << sum / 10 << '\n';
}
