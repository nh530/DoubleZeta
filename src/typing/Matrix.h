#include "typing/DTypes.h"
#include "typing/exceptions.h"
#include <vector>
#ifndef MATRIX_H
#define MATRIX_H
/* TODO: It looks like the Matrix user defined type does not work when passing into a function normally.
 * It works when passing by reference.
 *
 *
 * */
class Matrix {
  std::vector<float *> _container;
  int _rows;
  int _cols;
	float _def_val;

public:
  int shape[2];
  Matrix(int rows = 0, int cols = 0, float def_val = 0);
  Matrix(float **, int, int);
  Matrix(const Matrix &);
  Matrix(Matrix &&);
  ~Matrix();
  float *operator[](int);
  const float *operator[](int) const;
  Matrix &operator=(const Matrix &);
  Matrix &operator=(Matrix &&);
  Matrix operator+=(const Matrix);
  Matrix operator-=(const Matrix);
  Matrix transpose();
  const std::vector<float> getColumn(int j) const;
	// Add a print function.
};

Matrix operator+(const Matrix &, const Matrix &);
Matrix operator-(const Matrix &, const Matrix &);
Matrix operator*(const Matrix &, const Matrix &);
Matrix operator*(const int &, const Matrix &);
Matrix operator*(const float &, const Matrix &);
bool operator==(const Matrix &, const Matrix &);
bool operator!=(const Matrix &, const Matrix &);

#endif
