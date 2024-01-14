#ifndef MATRIX_H
#define MATRIX_H
#include "threadpool/ZetaSession.h"
#include "typing/DTypes.h"
#include "typing/exceptions.h"
#include <vector>

/* TODO: It looks like the Matrix user defined type does not work when passing into a function normally.
 * It works when passing by reference.
 * Multithread this Matrix object will make gradient descent multithreaded.
 *
 * */
class Matrix {
  std::vector<float *> _container;
  int _rows;
  int _cols;
  static ZetaSession _worker;
  friend Matrix operator-(const Matrix &, const Matrix &);
  friend Matrix operator+(const Matrix &, const Matrix &);
  friend Matrix operator*(const float &a, const Matrix &b);
	friend Matrix operator*(const int &a, const Matrix &b);
  friend Matrix operator*(const Matrix &a, const Matrix &b);

public:
  int shape[2];
  Matrix();
  Matrix(int rows, int cols);
  Matrix(int rows, int cols, float def_val);
  Matrix(float **, int, int);
  ~Matrix();
  Matrix(const Matrix &);
  Matrix(Matrix &&);
  // TODO: LIMITATION WITH CURRENT APPROACH
  // Note that if the first index is within bounds but the second index is out of bounds, then undefined behavior occurs.
  // i.e. test1[2][10] will have undefined behavior. There is no way to force array indexing limit.
  float const *const &operator[](int);
  // TODO: const float *operator[](int) const; member function is not tested I think.
  float const *const &operator[](int) const;
  void set_value(int r, int c, float val);
  void set_value(int r, float *row);
  float &get_value(int r, int c);
  Matrix &operator=(const Matrix &);
  Matrix &operator=(Matrix &&);
  Matrix operator+=(const Matrix &);
  Matrix operator-=(const Matrix &);
  Matrix transpose();
  Matrix transpose() const;
  const std::vector<float> getColumn(int j) const;
  void print();
};

Matrix operator+(const Matrix &, const Matrix &);
Matrix operator-(const Matrix &, const Matrix &);
Matrix operator*(const Matrix &, const Matrix &);
Matrix operator*(const int &, const Matrix &);
Matrix operator*(const float &, const Matrix &);
Matrix operator*(const Matrix &, const int &);
Matrix operator*(const Matrix &, const float &);
bool operator==(const Matrix &, const Matrix &);
bool operator!=(const Matrix &, const Matrix &);

float **init_mat(float val, int rows, int cols);

#endif
