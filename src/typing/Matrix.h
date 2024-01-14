#ifndef MATRIX_H
#define MATRIX_H
#include "algba_cmd/Algebra.h"
#include "threadpool/ZetaSession.h"
#include "typing/DTypes.h"
#include "typing/exceptions.h"
#include <list>
#include <string>
#include <tuple>
#include <vector>

/* TODO: It looks like the Matrix user defined type does not work when passing into a function normally.
 * It works when passing by reference.
 * */
template <DataType T> class Matrix; // Forward declaration

template <DataType T> std::vector<std::vector<T>> matrix_to_vector();
template <DataType T> std::tuple<std::tuple<T>> matrix_to_tuple();
template <DataType T> std::list<std::list<T>> matrix_to_list();
template <Numeric T> T find_max();
template <Numeric T> T find_min();
int find_argmax();
int find_argmin();
template <Numeric T> T find_trace();
template <Numeric T> T find_sum();
template <Numeric T> T find_cumsum();
template <Numeric T> T find_mean();
template <Numeric T> T find_var();
template <Numeric T> T find_std();
bool all();
bool any();
void matrix_dump();

class Array {
public:
  // Methods that provide information on the memory consumption of an array.
  virtual uint nbytes() = 0; // Size of total array in bytes
  virtual int size() = 0;
  virtual std::string dtype() = 0;               // TODO: This probably useless.
  virtual std::tuple<void *, int> strides() = 0; // The address to first element in row and offset to last element in row.
  virtual uint itemsize() = 0;                   // Length of one row in bytes
  virtual ~Array() {}
  virtual void reshape() = 0;
  virtual void resize() = 0;
  virtual void flatten() = 0;
  virtual void repeat() = 0;
  virtual void set_value(int r, float &row) = 0;
  virtual void choose() = 0;
  virtual void sort() = 0;
  virtual std::vector<uint> argsort() = 0;
  virtual std::vector<uint> nonzero() = 0;
};

// Necessary for correct functioning of the friend syntax.
template <DataType T> Matrix<T> operator+(const Matrix<T> &, const Matrix<T> &);
template <DataType T> Matrix<T> operator-(const Matrix<T> &, const Matrix<T> &);
template <DataType T> Matrix<T> operator*(const Matrix<T> &, const Matrix<T> &);
template <DataType T> Matrix<T> operator*(const int &, const Matrix<T> &);
template <DataType T> Matrix<T> operator*(const float &, const Matrix<T> &);
template <DataType T> Matrix<T> operator*(const Matrix<T> &, const int &);
template <DataType T> Matrix<T> operator*(const Matrix<T> &, const float &);
template <DataType T> bool operator==(const Matrix<T> &, const Matrix<T> &);
template <DataType T> bool operator!=(const Matrix<T> &, const Matrix<T> &);

template <DataType T> class Matrix {
  std::vector<T *> _container;
  int _rows;
  int _cols;
  static ZetaSession _worker;
  friend Matrix<T> operator-<>(const Matrix<T> &, const Matrix<T> &);
  friend Matrix<T> operator+<>(const Matrix<T> &, const Matrix<T> &);
  friend Matrix<T> operator*<>(const Matrix<T> &, const Matrix<T> &);
  friend Matrix<T> operator*<>(const float &, const Matrix<T> &);
  friend Matrix<T> operator*<>(const int &, const Matrix<T> &);
  friend Matrix<T> operator*<>(const Matrix<T> &, const float &);
  friend Matrix<T> operator*<>(const Matrix<T> &, const int &);

public:
  int shape[2];
  // int ndim;                                    // TODO: Not implemented
  // std::tuple<std::tuple<void *, int>> strides; // TODO: Make sure this overrides inherited method.
  Matrix();
  Matrix(int rows, int cols);
  Matrix(int rows, int cols, T def_val);
  Matrix(T **, int, int);
  ~Matrix();
  Matrix(const Matrix<T> &);
  Matrix(Matrix<T> &&);
  // TODO: LIMITATION WITH CURRENT APPROACH
  // Note that if the first index is within bounds but the second index is out of bounds, then undefined behavior occurs.
  // i.e. test1[2][10] will have undefined behavior. There is no way to force array indexing limit.
  T const *const &operator[](int);
  // TODO: const float *operator[](int) const; member function is not tested I think.
  T const *const &operator[](int) const;
  void set_value(int r, int c, float val);
  void set_value(int r, float *row);
  T &get_value(int r, int c);
  T const *const &get_value(int);
  T const *const &get_value(Matrix<bool>);
  Matrix<T> &operator=(const Matrix<T> &);
  Matrix<T> &operator=(Matrix<T> &&);
  Matrix<T> operator+=(const Matrix<T> &);
  Matrix<T> operator-=(const Matrix<T> &);
  Matrix<T> transpose();
  Matrix<T> transpose() const;
  const std::vector<float> getColumn(int j) const;
  void print();
};


float **init_mat(float val, int rows, int cols);
template <DataType T> Matrix<T> clip(Matrix<T> &);
template <DataType T> Matrix<T> round(Matrix<T> &);

#include "Matrix.tpp"

#endif
