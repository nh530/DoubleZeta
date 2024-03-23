#include "threadpool/Algebra.h"
#include "typing/Matrix.h"
#include <chrono>
#include <iostream>
#include <list>
int N_ROWS = 4000;
int N_COLS = 10000;

void arr_init_(float *out, float c) {
  for (int i = 0; i < N_COLS; i++) {
    out[i] = c;
  }
}

std::chrono::duration<double, std::milli> base_trial() {
  float *vec_a = new float[N_COLS];
  float *vec_b = new float[N_COLS];
  arr_init_(vec_a, 1.1f);
  arr_init_(vec_b, 4904.111f);
  std::vector<float *> items;
  auto ti1 = std::chrono::high_resolution_clock::now();
  Matrix<float> out{N_ROWS, N_COLS};
  for (int i = 0; i < N_ROWS; i++) {
    float *out = arr_op_sub(vec_a, vec_b, N_COLS, N_COLS);
    items.push_back(out);
  }
  int i = 0;
  for (auto const &ele : items) {
    out.set_value(i, ele);
    delete ele;
    i++;
  }
  items.clear();
  delete[] vec_a;
  delete[] vec_b;
  auto ti2 = std::chrono::high_resolution_clock::now();

  return std::chrono::duration<double, std::milli>{ti2 - ti1};
}

std::chrono::duration<double, std::milli> benchmark() {
  Matrix<float> a(N_ROWS, N_COLS, 1.1f);
  Matrix<float> b(N_ROWS, N_COLS, 4904.111f);
  auto t1 = std::chrono::high_resolution_clock::now();
  a -= b;
  auto t2 = std::chrono::high_resolution_clock::now();
  return std::chrono::duration<double, std::milli>{t2 - t1};
}
std::chrono::duration<double, std::milli> benchmark2() {
  Matrix<float> a(N_ROWS, N_COLS, 1.1f);
  Matrix<float> b(N_ROWS, N_COLS, 4904.111f);
  auto t1 = std::chrono::high_resolution_clock::now();
  auto out = a - b;
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
  std::cout << "Multi-threaded calculation (-=) runtime: " << summ / 2 << '\n';
  double summm = 0;
  for (int i = 0; i < 2; i++) {
    summm += trials2[i].count();
  }
  std::cout << "Multi-threaded calculation (-) runtime: " << summm / 2 << '\n';

  std::cout << "Fractional improvement (-=): " << sum / summ << '\n';
	std::cout << "Fractional improvement (-): " << sum / summm << '\n';
}
