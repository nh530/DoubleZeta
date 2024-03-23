#include "threadpool/Algebra.h"
#include "threadpool/ZetaSession.h"
#include "typing/Matrix.h"
#include <chrono>
#include <iostream>
#include <list>
ZetaSession newZeta{10};
int N_ROWS = 5000;
int N_COLS = N_ROWS;

void arr_init_(float *out, float c) {
  for (int i = 0; i < N_ROWS; i++) {
    out[i] = c;
  }
}

std::chrono::duration<double, std::milli> base_trial() {
  float *vec_a = new float[N_COLS];
  float *vec_b = new float[N_ROWS];
  arr_init_(vec_a, 1);
  arr_init_(vec_b, 2);
  std::vector<float *> items;
  auto ti1 = std::chrono::high_resolution_clock::now();
  for (int i = 0; i < N_ROWS; i++) {
    for (int j = 0; j < N_COLS; j++) {
      float *out = arr_op_dot_prod(vec_a, vec_b, N_COLS, N_ROWS);
      items.push_back(out);
    }
  }
  auto ti2 = std::chrono::high_resolution_clock::now();
  delete[] vec_a;
  delete[] vec_b;
  for (auto const &ele : items) {
    delete ele;
  }
  items.clear();

  return std::chrono::duration<double, std::milli>{ti2 - ti1};
}

std::chrono::duration<double, std::milli> benchmark2() {
  float *vec_a = new float[N_COLS];
  float *vec_b = new float[N_ROWS];
  arr_init_(vec_a, 1);
  arr_init_(vec_b, 2);
  auto t1 = std::chrono::high_resolution_clock::now();
  std::vector<Status<float> *> out_arr{};
  for (int i = 0; i < N_ROWS; i++) {
    for (int j = 0; j < N_COLS; j++) {
      Status<float> *temp = new Status<float>(newZeta.SubmitTask(&arr_op_dot_prod, vec_a, vec_b, N_ROWS, N_COLS));
      out_arr.push_back(temp);
    }
  }

  while (true) {
    if (newZeta.Busy())
      continue;
    else
      break;
  }
  delete[] vec_a;
  delete[] vec_b;
  for (auto const &ele : out_arr) {
    delete ele;
  }
  out_arr.clear();

  auto t2 = std::chrono::high_resolution_clock::now();
  return std::chrono::duration<double, std::milli>{t2 - t1};
}

std::chrono::duration<double, std::milli> benchmark() {
  Matrix<float> a(N_ROWS, N_COLS, 1);
  Matrix<float> b(N_ROWS, N_COLS, 2);
  auto t1 = std::chrono::high_resolution_clock::now();
  auto out = a * b; // dot product
  auto t2 = std::chrono::high_resolution_clock::now();
  return std::chrono::duration<double, std::milli>{t2 - t1};
}

int main() {
  std::cout << "Number of rows: " << N_ROWS << '\n';
  std::cout << "Number of cols: " << N_COLS << '\n';
  std::chrono::duration<double, std::milli> times[10];
  std::chrono::duration<double, std::milli> trials[10];
  for (int i = 0; i < 2; i++) {
    times[i] = base_trial();
    trials[i] = benchmark();
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
  std::cout << "Multi-threaded calculation runtime: " << summ / 2 << '\n';
  std::cout << "Fractional improvement: " << sum / summ << '\n';
}
