#include "typing/DTypes.h"
#include "typing/exceptions.h"
#include <vector>
#ifndef MATRIX_H
#define MATRIX_H

class Matrix {
  std::vector<std::vector<float>> _container;
  int _rows;
  int _cols;

public:
  int shape[2];
  Matrix(int rows = 0, int cols = 0);
  Matrix(float **, int, int);
  Matrix(const Matrix &);
  Matrix(Matrix &&);
  ~Matrix();
  std::vector<float> &operator[](int);
  const std::vector<float> &operator[](int) const;
  Matrix &operator=(const Matrix &);
  Matrix &operator=(Matrix &&);
  Matrix operator+=(const Matrix);
  Matrix operator-=(const Matrix);
  Matrix transpose();
  std::vector<float> _get_column(const Matrix matx, int j) const;
};

Matrix operator+(const Matrix &, const Matrix &);
Matrix operator-(const Matrix &, const Matrix &);
Matrix operator*(const Matrix &, const Matrix &);
Matrix operator*(const int &, const Matrix &);
Matrix operator*(const float &, const Matrix &);
bool operator==(const Matrix &, const Matrix &);
bool operator!=(const Matrix &, const Matrix &);

#endif
