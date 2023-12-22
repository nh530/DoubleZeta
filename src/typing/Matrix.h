#include "typing/DTypes.h"
#include "typing/exceptions.h"
#include <vector>
#ifndef MATRIX_H
#define MATRIX_H
/* TODO: It looks like the Matrix user defined type does not work when passing into a function normally.
 * It works when passing by reference.
 * Multithread this Matrix object will make gradient descent multithreaded.
 *
 * */
class Matrix {
  std::vector<float *> _container;
  int _rows;
  int _cols;

public:
  int shape[2];
  Matrix();
  Matrix(int rows, int cols, float def_val = 0);
  Matrix(float **, int, int);
  ~Matrix();
  Matrix(const Matrix &);
  Matrix(Matrix &&);
  // TODO: LIMITATION WITH CURRENT APPROACH
  // Note that if the first index is within bounds but the second index is out of bounds, then undefined behavior occurs.
  // i.e. test1[2][10] will have undefined behavior. There is no way to force array indexing limit.
  float *operator[](int);
  const float *operator[](int) const;
  Matrix &operator=(const Matrix &);
  Matrix &operator=(Matrix &&);
  Matrix operator+=(const Matrix);
  Matrix operator-=(const Matrix);
  Matrix transpose();
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
