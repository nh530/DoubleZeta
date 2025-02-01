#ifndef MATRIX_H
#define MATRIX_H
#include "Array.h"
#include "threadpool/Algebra.h"
#include "threadpool/ZetaSession.h"
#include "typing/DTypes.h"
#include "typing/exceptions.h"
#include <list>
#include <string>
#include <tuple>
#include <vector>

/* TODO: It looks like the Matrix user defined type does not work when passing into a function normally.
 * It works when passing by reference.
 * TODO: Is it possibly an issue for there to be multiple ZetaSession objects? Each class would have its own.
 * */
template <DataType T> std::vector<std::vector<T>> matrix_to_vector();
template <DataType T> std::tuple<std::tuple<T>> matrix_to_tuple();
template <DataType T> std::list<std::list<T>> matrix_to_list();
template <int_or_float N, DataType T> N find_max(const Matrix<T> &);
template <int_or_float N, DataType T> N find_max(const Vector<T> &);
template <int_or_float N, DataType T> N find_min(const Matrix<T> &);
template <int_or_float N, DataType T> N find_min(const Vector<T> &);
template <DataType T> int find_argmax(const Vector<T> &);
template <DataType T> std::tuple<int, int> find_argmax(const Matrix<T> &);
template <DataType T> int find_argmin(const Vector<T> &);
template <DataType T> std::tuple<int, int> find_argmin(const Matrix<T> &);
template <DataType T> T find_trace(const Matrix<T> &);
template <int_or_float N, DataType T> N find_sum(const Matrix<T> &);
template <int_or_float N, DataType T> N find_sum(const Vector<T> &);
template <int_or_float N, DataType T> N find_cumsum(const Matrix<T> &);
template <int_or_float N, DataType T> N find_cumsum(const Vector<T> &);
template <int_or_float N, DataType T> N find_mean(const Matrix<T> &);
template <int_or_float N, DataType T> N find_mean(const Vector<T> &);
template <int_or_float N, DataType T> N find_var(const Matrix<T> &);
template <int_or_float N, DataType T> N find_var(const Vector<T> &);
template <int_or_float N, DataType T> N find_std(const Matrix<T> &);
template <int_or_float N, DataType T> N find_std(const Vector<T> &);
template <DataType T> Vector<T> choose(const Vector<T> &, const Vector<T> &, const Vector<bool> &);
template <DataType T> Matrix<T> choose(const Matrix<T> &, const Matrix<T> &, const Matrix<bool> &);
template <DataType T> Matrix<T> choose(const Matrix<T> &, const Matrix<T> &, const Vector<bool> &, int);
template <DataType T> T sort();
template <DataType T> std::vector<unsigned int> argsort();
template <DataType T> std::vector<unsigned int> nonzero();
bool all();
bool any();
void matrix_dump();

template <DataType T> class Vector : public Array {
  T *_container = new T;
  int _num_ele;
  static ZetaSession _worker;
  friend Vector<T> operator- <>(const Vector<T> &, const Vector<T> &);
  friend Vector<T> operator+ <>(const Vector<T> &, const Vector<T> &);
  friend Vector<T> operator* <>(const Vector<T> &, const Vector<T> &);
  friend Vector<T> operator* <>(const float &, const Vector<T> &);
  friend Vector<T> operator* <>(const int &, const Vector<T> &);
  friend Vector<T> operator* <>(const Vector<T> &, const float &);
  friend Vector<T> operator* <>(const Vector<T> &, const int &);

public:
  int shape[2];
  std::list<std::list<void *>> get_reffs();
  Vector();
  Vector(int elems);
  Vector(int elems, T def_val);
  ~Vector() override;
  Vector(const Vector<T> &);
  Vector(Vector<T> &&);
  const T &operator[](int);
  const T &operator[](int) const;
  void set_value(int, T);
  T &get_value(int) const;
  T &get_value(int);
  Vector<T> &get_value(const Vector<bool> &);
  Vector<T> &operator=(const Vector<T> &);
  Vector<T> &operator=(Vector<T> &&);
  Vector<T> operator+=(const Vector<T> &);
  Vector<T> operator-=(const Vector<T> &);
  void repeat(int, int);
  int ndim() override;
  void reshape(int);
  void print() override;
  unsigned int nbytes() override;
  int size() override;
  int size() const override;
  unsigned int itemsize() override;
  void flatten() override;
};

template <DataType T> class Matrix : Array {
  std::vector<T *> _container;
  int _rows;
  int _cols;
  static ZetaSession _worker;
  friend Matrix<T> operator- <>(const Matrix<T> &, const Matrix<T> &);
  friend Matrix<T> operator+ <>(const Matrix<T> &, const Matrix<T> &);
  friend Matrix<T> operator* <>(const Matrix<T> &, const Matrix<T> &);
  friend Matrix<T> operator* <>(const float &, const Matrix<T> &);
  friend Matrix<T> operator* <>(const int &, const Matrix<T> &);
  friend Matrix<T> operator* <>(const Matrix<T> &, const float &);
  friend Matrix<T> operator* <>(const Matrix<T> &, const int &);

public:
  int shape[2];
  std::list<std::list<void *>> get_reffs();
  Matrix();
  Matrix(int rows, int cols);
  Matrix(int rows, int cols, T def_val);
  Matrix(T **, int, int);
  ~Matrix() override;
  Matrix(const Matrix<T> &);
  Matrix(Matrix<T> &&);
  // TODO: LIMITATION WITH CURRENT APPROACH
  // Note that if the first index is within bounds but the second index is out of bounds, then undefined behavior occurs.
  // i.e. test1[2][10] will have undefined behavior. There is no way to force array indexing limit.
  T const *const &operator[](int);
  // TODO: const float *operator[](int) const; member function is not tested I think.
  T const *const &operator[](int) const;
	// TODO: Need private set_value and public set_value that has guard rails
  void set_value(int r, int c, T val);
  void set_value(int r, T *row);
  T &get_value(int r, int c);
  T &get_value(int r, int c) const;
  T const *const &get_value(int);
  T const *const &get_value(Matrix<bool>); // TODO: Not implemented
  int operator()(int, int);                // TODO: Not implemented
  Matrix<T> &operator=(const Matrix<T> &);
  Matrix<T> &operator=(Matrix<T> &&);
  Matrix<T> &operator+=(const Matrix<T> &);
  Matrix<T> &operator-=(const Matrix<T> &);
  Matrix<T> transpose();
  Matrix<T> transpose() const;
  const std::vector<float> getColumn(int j) const;
  // Matrix<T> getColumnM(int j); // TODO: not implemented
  int ndim() override;
  void print() override;
  unsigned int nbytes() override;
  int size() override;
  int size() const override;
  unsigned int itemsize() override;
  void flatten() override;
};

float **init_mat(float val, int rows, int cols);
template <DataType T> Matrix<T> clip(Matrix<T> &);
template <DataType T> Matrix<T> round(Matrix<T> &);

#include "Matrix.tpp"

#endif
