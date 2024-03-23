#ifndef ARRAY_H
#define ARRAY_H
#include "typing/DTypes.h"
#include "typing/exceptions.h"
#include <list>
#include <string>
#include <tuple>
#include <vector>

// Forward declaration
template <DataType T> class Matrix;
template <DataType T> class Vector;
template <DataType T> class Cube;
// Necessary for correct functioning of the friend syntax.
template <DataType T> Vector<T> operator+(const Vector<T> &, const Vector<T> &);
template <DataType T> Vector<T> operator-(const Vector<T> &, const Vector<T> &);
template <DataType T> Vector<T> operator*(const Vector<T> &, const Vector<T> &);
template <DataType T> Vector<T> operator*(const int &, const Vector<T> &);
template <DataType T> Vector<T> operator*(const float &, const Vector<T> &);
template <DataType T> Vector<T> operator*(const Vector<T> &, const int &);
template <DataType T> Vector<T> operator*(const Vector<T> &, const float &);
template <DataType T> bool operator==(const Vector<T> &, const Vector<T> &);
template <DataType T> bool operator!=(const Vector<T> &, const Vector<T> &);
template <DataType T> Matrix<T> operator+(const Matrix<T> &, const Matrix<T> &);
template <DataType T> Matrix<T> operator-(const Matrix<T> &, const Matrix<T> &);
template <DataType T> Matrix<T> operator*(const Matrix<T> &, const Matrix<T> &);
template <DataType T> Matrix<T> operator*(const int &, const Matrix<T> &);
template <DataType T> Matrix<T> operator*(const float &, const Matrix<T> &);
template <DataType T> Matrix<T> operator*(const Matrix<T> &, const int &);
template <DataType T> Matrix<T> operator*(const Matrix<T> &, const float &);
template <DataType T> bool operator==(const Matrix<T> &, const Matrix<T> &);
template <DataType T> bool operator!=(const Matrix<T> &, const Matrix<T> &);
template <DataType T> Cube<T> operator+(const Cube<T> &, const Cube<T> &);
template <DataType T> Cube<T> operator-(const Cube<T> &, const Cube<T> &);
template <DataType T> Cube<T> operator*(const Cube<T> &, const Cube<T> &);
template <DataType T> Cube<T> operator*(const int &, const Cube<T> &);
template <DataType T> Cube<T> operator*(const float &, const Cube<T> &);
template <DataType T> Cube<T> operator*(const Cube<T> &, const int &);
template <DataType T> Cube<T> operator*(const Cube<T> &, const float &);
template <DataType T> bool operator==(const Cube<T> &, const Cube<T> &);
template <DataType T> bool operator!=(const Cube<T> &, const Cube<T> &);

class Array {
  // Note this is not ment to be some contractual aggrement on the usage of the concrete classes.
public:
  // Methods that provide information on the memory consumption of an object.
  virtual unsigned int nbytes() = 0; // Size of total array in bytes
  virtual int size() = 0;
  virtual int size() const = 0;
  virtual unsigned int itemsize() = 0; // Length of one row in bytes
  virtual ~Array() {}
  virtual void flatten() = 0;
  virtual int ndim() = 0;
  virtual void print() = 0;
};

#endif
