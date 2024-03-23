#ifndef CUBE_H
#define CUBE_H
#include "Array.h"
#include "threadpool/Algebra.h"
#include "threadpool/ZetaSession.h"
#include "typing/DTypes.h"
#include "typing/exceptions.h"
#include <list>
#include <string>
#include <tuple>
#include <vector>

template <DataType T> class Cube;
template <int_or_float N, DataType T> N find_sum(const Cube<T> &arr);
template <int_or_float N, DataType T> N find_max(const Cube<T> &arr);
template <int_or_float N, DataType T> N find_min(const Cube<T> &arr);
template <DataType T> std::tuple<int, int, int> find_argmax(const Cube<T> &arr);
template <DataType T> std::tuple<int, int, int> find_argmin(const Cube<T> &arr);
template <int_or_float N, DataType T> N find_cumsum(const Cube<T> &arr);
template <int_or_float N, DataType T> N find_cumsum(const Cube<T> &arr, int axis);
template <int_or_float N, DataType T> N find_cumsum(const Cube<T> &arr, int axis_x, int axis_y);
template <int_or_float N, DataType T> N find_mean(const Cube<T> &arr);
template <int_or_float N, DataType T> N find_var(const Cube<T> &arr);
template <int_or_float N, DataType T> N find_std(const Cube<T> &arr);
template <DataType T> Cube<T> choose(const Cube<T> &left, const Cube<T> &right, const Cube<bool> &mask);
template <DataType T> Cube<T> choose(const Cube<T> &left, const Cube<T> &right, const Matrix<bool> &mask, int axis);

template <DataType T> class Cube : Array {
  // 3-dimensional Cube.
  std::vector<std::vector<T *>> _container;
  int _rows;
  int _cols;
  int _depth;
  static ZetaSession _worker;
  friend Cube<T> operator- <>(const Cube<T> &, const Cube<T> &);
  friend Cube<T> operator+ <>(const Cube<T> &, const Cube<T> &);
  friend Cube<T> operator* <>(const Cube<T> &, const Cube<T> &);
  friend Cube<T> operator* <>(const float &, const Cube<T> &);
  friend Cube<T> operator* <>(const int &, const Cube<T> &);
  friend Cube<T> operator* <>(const Cube<T> &, const float &);
  friend Cube<T> operator* <>(const Cube<T> &, const int &);

public:
  int shape[3];
  std::list<std::list<std::list<void *>>> get_reffs();
  Cube();
  Cube(int x, int y, int z);
  Cube(int x, int y, int z, T def_val);
  Cube(T ***arr, int x, int y, int z);
  ~Cube() override;
  Cube(const Cube<T> &);
  Cube(Cube<T> &&);
  T &operator()(unsigned int x, unsigned int y, unsigned int z);
  void set_value(int, int, int, T);
  T &get_value(int, int, int) const;
  T &get_value(int, int, int);
  Cube<T> &get_value(const Cube<bool> &);
  Cube<T> &operator=(const Cube<T> &);
  Cube<T> &operator=(Cube<T> &&);
  Cube<T> operator+=(const Cube<T> &);
  Cube<T> operator-=(const Cube<T> &);
  Cube<T> transpose();
  Cube<T> transpose() const;
	Cube<T> transpose(unsigned int, unsigned int);
  void reshape(int, int, int);
  int ndim() override;
  void print() override;
  unsigned int nbytes() override;
  int size() override;
  int size() const override;
  unsigned int itemsize() override;
  void flatten() override;
};

#include "Cube.tpp"

#endif
