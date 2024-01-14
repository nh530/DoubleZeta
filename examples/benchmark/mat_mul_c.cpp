#include "algba_cmd/Algebra.h"
#include "threadpool/ZetaSession.h"
#include "typing/Matrix.h"
#include <chrono>
#include <iostream>
#include <list>
ZetaSession newZeta{10};
int N_ROWS = 5000;
int N_COLS = 10000;

void arr_init_(float *out, float c) {
  for (int i = 0; i < N_ROWS; i++) {
    out[i] = c;
  }
}

std::chrono::duration<double, std::milli> base_trial() {
  float *vec_a = new float[N_COLS];
  arr_init_(vec_a, 4904.111f);
  auto ti1 = std::chrono::high_resolution_clock::now();
  std::vector<float *> items;
  Matrix out{N_ROWS, N_COLS};
  for (int i = 0; i < N_ROWS; i++) {
    float *out = arr_op_mul_c(vec_a, N_COLS, 2.0f);
    items.push_back(out);
  }
  int i = 0;
  for (auto const &ele : items) {
    out.set_value(i, ele);
    delete ele;
    i++;
  }

  delete[] vec_a;
  items.clear();
  auto ti2 = std::chrono::high_resolution_clock::now();

  return std::chrono::duration<double, std::milli>{ti2 - ti1};
}

std::chrono::duration<double, std::milli> benchmark() {
  Matrix b(N_ROWS, N_COLS, 4904.111f);
  auto t1 = std::chrono::high_resolution_clock::now();
  auto out = b * 2.0f;
  auto t2 = std::chrono::high_resolution_clock::now();
  return std::chrono::duration<double, std::milli>{t2 - t1};
}
std::chrono::duration<double, std::milli> benchmark2() {
  Matrix b(N_ROWS, N_COLS, 4904.111f);
  auto t1 = std::chrono::high_resolution_clock::now();
  auto out = 2.0f * b;
  auto t2 = std::chrono::high_resolution_clock::now();
  return std::chrono::duration<double, std::milli>{t2 - t1};
}

int main() {
  std::cout << "Number of rows: " << N_ROWS << '\n';
  std::cout << "Number of cols: " << N_COLS << '\n';
  std::chrono::duration<double, std::milli> times[2];
  std::chrono::duration<double, std::milli> trials[2];
  std::chrono::duration<double, std::milli> trials2[2];
  for (int i = 0; i < 2; i++) {
    times[i] = base_trial();
    trials[i] = benchmark();
    trials2[i] = benchmark2();
  }
  double sum = 0;
  for (int i = 0; i < 2; i++) {
    sum += times[i].count();
  }
  std::cout << "Serial calculation runtime: " << sum / 2 << '\n';
  double summ = 0;
  for (int i = 0; i < 2; i++) {
    summ += trials[i].count();
  }
  std::cout << "Multi-threaded calculation (m * 2.0f) runtime: " << summ / 2 << '\n';
  double summm = 0;
  for (int i = 0; i < 2; i++) {
    summm += trials2[i].count();
  }
  std::cout << "Multi-threaded calculation (2.0f * m) runtime: " << summm / 2 << '\n';

  std::cout << "Fractional improvement (m * 2.0f): " << sum / summ << '\n';
  std::cout << "Fractional improvement (2.0f * m): " << sum / summm << '\n';
}
