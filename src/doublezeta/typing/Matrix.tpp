#ifndef MATRIX_TPP
#define MATRIX_TPP
#include "Matrix.h"
#include "typing/exceptions.h"
#include <cmath>
#include <iostream>
#include <list>
inline float _vector_dotproduct(std::vector<float> left, std::vector<float> right) {
  /* Computes the dot product of two vectors that are the same size.
   *
   *
   */
  if (left.size() != right.size()) {
    std::cout << "The size of the left vector is: " << left.size() << '\n';
    std::cout << "The size of the right vector is: " << right.size() << '\n';
    throw std::invalid_argument("The two vectors must be same size!");
  }
  int l_ele = left.size();
  float out;
  for (int i = 0; i < l_ele; i++) {
    out += left[i] * right[i];
  }

  return out;
}
inline void vector_to_arr(std::vector<float> vec, float *out) {
  for (int i = 0; i < vec.size(); i++) {
    out[i] = vec[i];
  }
}
template <int_or_float N, DataType T> N find_sum(const Vector<T> &arr) {
  N out;
  for (int i = 0; i < arr.size(); i++) {
    out += arr[i];
  }
  return out;
}
template <int_or_float N, DataType T> N find_sum(const Matrix<T> &arr) {
  // TODO: This needs to change. It is the same as find_cumsum.
  N out;
  for (int i = 0; i < arr.shape[0]; i++) {
    for (int j = 0; j < arr.shape[1]; j++) {
      out += arr.get_value(i, j);
    }
  }
  return out;
}
template <int_or_float N, DataType T> N find_max(const Matrix<T> &arr) {
  N out;
  for (int i = 0; i < arr.shape[0]; i++) {
    for (int j = 0; j < arr.shape[1]; j++) {
      if (out < arr.get_value(i, j)) {
        out = arr.get_value(i, j);
      }
    }
  }
  return out;
}
template <int_or_float N, DataType T> N find_max(const Vector<T> &arr) {
  N out;
  for (int i = 0; i < arr.shape[0]; i++) {
    if (out < arr.get_value(i)) {
      out = arr.get_value(i);
    }
  }
  return out;
}
template <int_or_float N, DataType T> N find_min(const Matrix<T> &arr) {
  N out;
  for (int i = 0; i < arr.shape[0]; i++) {
    for (int j = 0; j < arr.shape[1]; j++) {
      if (out > arr.get_value(i, j)) {
        out = arr.get_value(i, j);
      }
    }
  }
  return out;
}
template <int_or_float N, DataType T> N find_min(const Vector<T> &arr) {
  N out;
  for (int i = 0; i < arr.shape[0]; i++) {
    if (out > arr.get_value(i)) {
      out = arr.get_value(i);
    }
  }
  return out;
}
template <DataType T> int find_argmax(const Vector<T> &arr) {
  int out;
  float largest;
  for (int i = 0; i < arr.shape[0]; i++) {
    if (largest < arr.get_value(i)) {
      largest = arr.get_value(i);
      out = i;
    }
  }
  return out;
}
template <DataType T> std::tuple<int, int> find_argmax(const Matrix<T> &arr) {
  std::tuple<int, int> out;
  float largest = 0;
  for (int i = 0; i < arr.shape[0]; i++) {
    for (int j = 0; j < arr.shape[1]; j++) {
      if (largest > arr.get_value(i, j)) {
        largest = arr.get_value(i, j);
        std::get<0>(out) = i;
        std::get<1>(out) = j;
      }
    }
  }
  return out;
}
template <DataType T> std::tuple<int, int> find_argmin(const Matrix<T> &arr) {
  std::tuple<int, int> out;
  float smallest;
  for (int i = 0; i < arr.shape[0]; i++) {
    for (int j = 0; j < arr.shape[1]; j++) {
      if (smallest > arr.get_value(i, j)) {
        smallest = arr.get_value(i, j);
        std::get<0>(out) = i;
        std::get<1>(out) = j;
      }
    }
  }
  return out;
}
template <DataType T> int find_argmin(const Vector<T> &arr) {
  int out;
  float smallest;
  for (int i = 0; i < arr.shape[0]; i++) {
    if (smallest > arr.get_value(i)) {
      smallest = arr.get_value(i);
      out = i;
    }
  }
  return out;
}
template <int_or_float N, DataType T> N find_cumsum(const Matrix<T> &arr) {
  N out = 0;
  for (int i = 0; i < arr.shape[0]; i++) {
    for (int j = 0; j < arr.shape[1]; j++) {
      out += arr.get_value(i, j);
    }
  }
  return out;
}
template <int_or_float N, DataType T> N find_cumsum(const Vector<T> &arr) {
  N out = 0;
  for (int i = 0; i < arr.shape[0]; i++) {
    out += arr.get_value(i);
  }
  return out;
}
template <int_or_float N, DataType T> N find_mean(const Matrix<T> &arr) {
  N out = 0;
  for (int i = 0; i < arr.shape[0]; i++) {
    for (int j = 0; j < arr.shape[1]; j++) {
      out += arr.get_value(i, j);
    }
  }
  return out / (arr.shape[0] * arr.shape[1]);
}
template <int_or_float N, DataType T> N find_mean(const Vector<T> &arr) {
  N out = 0;
  for (int i = 0; i < arr.size(); i++) {
    out += arr.get_value(i);
  }

  return out / arr.size();
}
template <int_or_float N, DataType T> N find_var(const Matrix<T> &arr) {
  N sums = 0;
  int smpls = (arr.shape[0] * arr.shape[1]);
  for (int i = 0; i < arr.shape[0]; i++) {
    for (int j = 0; j < arr.shape[1]; j++) {
      sums += arr.get_value(i, j);
    }
  }
  Matrix<T> temp = (arr - (sums / smpls));
  // TODO: THere is probably a mistake here; Temp should be transposed?
  return find_sum(temp * temp) / smpls;
}
template <int_or_float N, DataType T> N find_var(const Vector<T> &arr) {
  N sums = 0;
  for (int i = 0; i < arr.size(); i++) {
    sums += arr.get_value(i);
  }
  Vector<T> temp = (arr - (sums / arr.size()));
  return find_sum(temp * temp) / arr.size();
}
template <int_or_float N, DataType T> N find_std(const Matrix<T> &arr) { return std::sqrt(find_var(arr)); }
template <int_or_float N, DataType T> N find_std(const Vector<T> &arr) { return std::sqrt(find_var(arr)); }
template <DataType T> Vector<T> choose(const Vector<T> &left, const Vector<T> &right, const Vector<bool> &mask) {
  if (left.size() != right.size())
    throw std::invalid_argument("Error: The two vectors differ in size");
  if (left.size() != mask.size())
    throw std::invalid_argument("Error: The mask is not the same size as the input vectors");

  Vector<T> out{left.size()};
  for (int i = 0; i < left.size(); i++) {
    if (mask[i]) {
      out[i] = left[i];
    } else {
      out[i] = right[i];
    }
  }
  return out;
}
template <DataType T> Matrix<T> choose(const Matrix<T> &left, const Matrix<T> &right, const Matrix<bool> &mask) {
  if (left.shape[0] != right.shape[0] || left.shape[1] != right.shape[1])
    throw std::invalid_argument("Error: The two matrices differ in size");
  if (left.shape[0] != mask.shape[0] || left.shape[1] != mask.shape[1])
    throw std::invalid_argument("Error: The mask is not the same size as the input matrices");
  Matrix<T> out{left.shape[0], left.shape[1]};
  for (int i = 0; i < out.shape[0]; i++) {
    for (int j = 0; j < out.shape[1]; j++) {
      if (mask.get_value(i, j)) {
        out.set_value(i, j, left.get_value(i, j));
      } else {
        out.set_value(i, j, right.get_value(i, j));
      }
    }
  }
  return out;
}
template <DataType T> Matrix<T> choose(const Matrix<T> &left, const Matrix<T> &right, const Vector<bool> &mask, int axis) {
  if (left.shape[0] != right.shape[0] | left.shape[1] != right.shape[1])
    throw std::invalid_argument("Error: The two matrices differ in size");
  if (left.shape[0] != mask.shape[0] | left.shape[1] != mask.shape[1])
    throw std::invalid_argument("Error: The mask is not the same size as the input matrices");
  if (axis != 0 | axis != 1)
    throw std::invalid_argument("Error: The axis value can either be 0 or 1.");

  Matrix<T> out{left.shape[0], left.shape[1]};
  if (axis == 0) {
    for (int i = 0; i < out.shape[0]; i++) {
      for (int j = 0; j < out.shape[1]; j++) {
        if (mask.get_value(j)) {
          out.set_value(i, j, left.get_value(i, j));
        } else {
          out.set_value(i, j, right.get_value(i, j));
        }
      }
    }
  } else {
    for (int i = 0; i < out.shape[0]; i++) {
      for (int j = 0; j < out.shape[1]; j++) {
        if (mask.get_value(i)) {
          out.set_value(i, j, left.get_value(i, j));
        } else {
          out.set_value(i, j, right.get_value(i, j));
        }
      }
    }
  }

  return out;
}

template <DataType T> ZetaSession Vector<T>::_worker = ZetaSession(-1);
template <DataType T> Vector<T>::Vector() {
  // Default Constructor
}
template <DataType T> Vector<T>::Vector(int elems) : _num_ele{elems} {
  // CAUTION: This constructor leads to undefined behavior since values in array are not set.
  // TODO: Maybe it will be bnest to hide this constructor from user.
  if (elems == 0)
    throw std::invalid_argument("The number of elements cannot be 0!");

  shape[0] = 1;
  shape[1] = elems;
  _container = new T[elems];
}
template <DataType T> Vector<T>::Vector(int elems, T def_val) {
  if (elems == 0)
    throw std::invalid_argument("The number of elements cannot be 0!");

  shape[0] = 1;
  shape[1] = elems;
  _num_ele = elems;
  _container = new T[elems];
  for (int i = 0; i < elems; i++) {
    _container[i] = def_val;
  }
}
template <DataType T> Vector<T>::~Vector() { delete _container; }
template <DataType T> Vector<T>::Vector(const Vector<T> &other) {
  shape[0] = other.shape[0];
  shape[1] = other.shape[1];
  _num_ele = other._num_ele;
  for (int i = 0; i < _num_ele; i++) {
    _container[i] = other._container[i];
  }
}
template <DataType T> Vector<T>::Vector(Vector<T> &&other) {
  shape[0] = other.shape[0];
  shape[1] = other.shape[1];
  _num_ele = other._num_ele;
  delete _container;
  _container = other._container;
  other._container = NULL;
}
template <DataType T> const T &Vector<T>::operator[](int idx) {
  if (idx < 0 | idx >= _num_ele)
    throw std::invalid_argument("Invalid index");
  return _container[idx];
}
template <DataType T> const T &Vector<T>::operator[](int idx) const {
  if (idx < 0 | idx >= _num_ele)
    throw std::invalid_argument("Invalid index");
  return _container[idx];
}
template <DataType T> void Vector<T>::set_value(int idx, T val) { _container[idx] = val; }
template <DataType T> T &Vector<T>::get_value(int idx) {
  if (idx < 0 | idx >= _num_ele)
    throw std::invalid_argument("Invalid index");
  return _container[idx];
}
template <DataType T> T &Vector<T>::get_value(int idx) const {
  if (idx < 0 | idx >= _num_ele)
    throw std::invalid_argument("Invalid index");
  return _container[idx];
}
template <DataType T> int Vector<T>::ndim() {
  /*  Number of array dimensions. For a Matrix, this is 2.
   * */
  return 1;
}
template <DataType T> Vector<T> &Vector<T>::get_value(const Vector<bool> &mask) {
  // TODO: Add edge case protection
  int nums = find_sum<int, bool>(mask); // False = 0, True = 1;  This can be sped up via cacheing the number of trues in a boolean vector
  Vector out{nums};
  for (int i = 0; i < _num_ele; i++) {
    if (mask[i] == true)
      out._container[i] = _container[i];
  }
  return std::move(out);
}
template <DataType T> Vector<T> &Vector<T>::operator=(const Vector<T> &other) {
  shape[0] = other.shape[0];
  shape[1] = other.shape[1];
  _num_ele = other._num_ele;
  for (int i = 0; i < _num_ele; i++) {
    _container[i] = other._container[i];
  }
  return *this;
}
template <DataType T> Vector<T> &Vector<T>::operator=(Vector<T> &&other) {
  shape[0] = other.shape[0];
  shape[1] = other.shape[1];
  _num_ele = other.num_ele;

  delete _container;
  _container = other._container;
  other._container = NULL;
  return *this;
}
template <DataType T> Vector<T> Vector<T>::operator+=(const Vector<T> &other) {
  if ((*this).shape[0] != other.shape[0] || (*this).shape[1] != other.shape[1]) {
    throw std::invalid_argument("Shape mismatch between left and right Matrices");
  }
  if ((*this).shape[0] == 0 && (*this).shape[1] == 0) {
    return *this;
  }

  T *my_half = _container + (_num_ele / 2); // sizeof(_container) returns size of increment of pointer.
  T *other_half = other._container + (_num_ele / 2);

  Status<T> *task = new Status<T>((*this)._worker.SubmitTask(&arr_op_sum, _container, other._container, _num_ele / 2, _num_ele / 2));
  Status<T> *task2 = new Status<T>((*this)._worker.SubmitTask(&arr_op_sum, my_half, other_half, _num_ele / 2, _num_ele / 2));

  std::future<T *> temp = task->GetFuturePtr();
  std::future<T *> temp2 = task2->GetFuturePtr();
  temp.wait();
  temp2.wait();
  T *res = temp.get();
  T *res2 = temp2.get();
  for (int i = 0; i < _num_ele / 2; i++) {
    _container[i] = res[i];
  }
  for (int i = _num_ele / 2; i < _num_ele; i++) {
    _container[i] = res2[i];
  }
  delete task;
  delete task2;

  return *this;
}
template <DataType T> Vector<T> Vector<T>::operator-=(const Vector<T> &other) {
  if ((*this).shape[0] != other.shape[0] || (*this).shape[1] != other.shape[1]) {
    throw std::invalid_argument("Shape mismatch between left and right Matrices");
  }
  if ((*this).shape[0] == 0 && (*this).shape[1] == 0) {
    return *this;
  }

  T *my_half = _container + (_num_ele / 2); // sizeof(_container) returns size of increment of pointer.
  T *other_half = other._container + (_num_ele / 2);

  Status<T> *task = new Status<T>((*this)._worker.SubmitTask(&arr_op_sub, _container, other._container, _num_ele / 2, _num_ele / 2));
  Status<T> *task2 = new Status<T>((*this)._worker.SubmitTask(&arr_op_sub, my_half, other_half, _num_ele / 2, _num_ele / 2));

  std::future<T *> temp = task->GetFuturePtr();
  std::future<T *> temp2 = task2->GetFuturePtr();
  temp.wait();
  temp2.wait();
  T *res = temp.get();
  T *res2 = temp2.get();
  for (int i = 0; i < _num_ele / 2; i++) {
    _container[i] = res[i];
  }
  for (int i = _num_ele / 2; i < _num_ele; i++) {
    _container[i] = res2[i];
  }
  delete task;
  delete task2;

  return std::move(*this);
}
template <DataType T> void Vector<T>::print() {
  std::cout << '\n' << "___________" << '\n' << "| ";
  for (int i = 0; i < _num_ele; i++) {
    std::cout << (*this)[i] << " | ";
  }
  std::cout << '\n' << "___________" << '\n';
}
template <DataType T> unsigned int Vector<T>::nbytes() { return sizeof(*_container) * _num_ele; }
template <DataType T> int Vector<T>::size() { return _num_ele; }
template <DataType T> int Vector<T>::size() const { return _num_ele; }
template <DataType T> unsigned int Vector<T>::itemsize() { return sizeof(*_container); }
template <DataType T> void Vector<T>::reshape(int size) {
  // Reshapes the current vector into a new vector with number of elements equal to size.
  bool is_ext = true;
  if (size < _num_ele) {
    std::cout << "Warning: Resizing to a smaller sized Vector" << '\n';
    is_ext = false;
  }
  T *temp = new T[size];
  if (is_ext) {
    for (int i = 0; i < _num_ele; i++) {
      temp[i] = _container[i];
    }
    for (int i = _num_ele; i < size; i++) {
      temp[i] = 0;
    }
  } else {
    for (int i = 0; i < size; i++) {
      temp[i] = _container[i];
    }
  }

  delete _container;
  _container = temp;
  temp = NULL;
  _num_ele = size;
}
template <DataType T> void Vector<T>::flatten() {
  // Do nothing because a Vector is already flat.
}
template <DataType T> void Vector<T>::repeat(int repeats, int axis) {
  if (axis != 0) {
    throw std::invalid_argument("Error: Axis must be 0 for Vector objects");
  }
  (*this).reshape(_num_ele + repeats);
  for (int i = _num_ele - repeats - 1; i < _num_ele; i++) {
    _container[i] = _container[_num_ele - repeats - 1];
  }
}
template <DataType T> std::list<std::list<void *>> Vector<T>::get_reffs() {
  std::list<std::list<void *>> out{};
  std::list<void *> temp{};
  for (int i = 0; i < _num_ele; i++) {
    temp.push_back(&_container[i]);
  }
  out.push_back(temp);
  return out;
}
template <DataType T> Vector<T> operator+(const Vector<T> &a, const Vector<T> &b) {
  // Element-wise add two matrices.
  if (a.shape[0] != b.shape[0] || a.shape[1] != b.shape[1]) {
    throw std::invalid_argument("Shape mismatch between left and right Matrices");
  }
  if (a.shape[0] == 0 && a.shape[1] == 0) {
    return Vector<T>{0, 0}; // empty matrix
  }
  Vector<T> out{a.shape[1]};
  for (int i = 0; i < a.shape[1]; i++) {
    out.set_value(i, a[i] + b[i]);
  }
  return std::move(out);
}
template <DataType T> Vector<T> operator-(const Vector<T> &a, const Vector<T> &b) {
  // Element-wise subtract two matrices.
  if (a.shape[0] != b.shape[0] || a.shape[1] != b.shape[1]) {
    throw std::invalid_argument("Shape mismatch between left and right Matrices");
  }
  if (a.shape[0] == 0 && a.shape[1] == 0) {
    return Matrix<T>{0, 0}; // empty matrix
  }
  Vector<T> out{a.shape[1]};
  for (int i = 0; i < a.shape[1]; i++) {
    out[i] = a[i] - b[i];
  }
  return std::move(out);
}

template <DataType T> Vector<T> operator*(const Vector<T> &a, const Vector<T> &b) {
  /* Dot product. This is not element-wise multiplication.
   * It is possible to return a 1x1 Matrix. Dot product of 2 vectors
   * represented as matrices will return 1x1 matrix that should be a scalar.
   *
   * */
  if (a.shape[1] != b.shape[0]) {
    throw std::invalid_argument("Shape mismatch between left and right Matrices");
  }
  if (a.shape[0] == 0 && a.shape[1] == 0) {
    return Matrix<T>{0, 0}; // empty matrix
  }

  Matrix<T> out{a.shape[1]};
  for (int i = 0; i < a.shape[1]; i++) {
    out[i] = a[i] * b[i];
  }

  return std::move(out);
}

template <DataType T> Vector<T> operator*(const float &a, const Vector<T> &b) {
  // Multiply the matrice by a scalar a.
  if (b.shape[0] == 0 && b.shape[1] == 0) {
    return Matrix<T>{0, 0}; // empty matrix
  }

  Matrix<T> out{b.shape[1]};
  for (int i = 0; i < b.shape[1]; i++) {
    out[i] = a * b[i];
  }
  return std::move(out);
}
template <DataType T> Vector<T> operator*(const int &a, const Vector<T> &b) {
  // Multiply the matrice by a scalar a.
  if (b.shape[0] == 0 && b.shape[1] == 0) {
    return Matrix<T>{0, 0}; // empty matrix
  }

  Matrix<T> out{b.shape[1]};
  for (int i = 0; i < b.shape[1]; i++) {
    out[i] = a * b[i];
  }

  return std::move(out);
}
template <DataType T> Vector<T> operator*(const Vector<T> &b, const int &a) { return a * b; }
template <DataType T> Vector<T> operator*(const Vector<T> &b, const float &a) { return a * b; }
template <DataType T> bool operator==(const Vector<T> &a, const Vector<T> &b) {
  // TODO: Floating point comparison is slow.
  if (a.shape[0] != b.shape[0] && a.shape[1] != b.shape[1]) {
    throw std::invalid_argument("Shape mismatch between left and right Matrices");
  }
  if (a.shape[1] == 0) {
    return true;
  }
  for (int i = 0; i < a.shape[1]; i++) {
    if (a[i] != b[i])
      return false;
  }
  return true;
}
template <DataType T> bool operator!=(const Vector<T> &a, const Vector<T> &b) {
  // TODO: Floating point comparison is slow.
  if (a.shape[0] != b.shape[0] && a.shape[1] != b.shape[1]) {
    throw std::invalid_argument("Shape mismatch between left and right Matrices");
  }
  if (a.shape[0] == 0 && a.shape[1] == 0) {
    return false;
  }
  for (int i = 0; i < a.shape[0]; i++) {
    if (a[i] != b[i])
      return true;
  }
  return false;
}

template <DataType T> ZetaSession Matrix<T>::_worker = ZetaSession(-1);
template <DataType T> Matrix<T>::Matrix() {
  // Default constructor; Initialize nothing.
}
template <DataType T> Matrix<T>::Matrix(int rows, int cols) : _rows{rows}, _cols{cols} {
  // CAUTION: This constructor leads to undefined behavior since values in array are not set.
  // TODO: Maybe it will be best to hide this constructor from User.
  if (rows == 0)
    throw std::invalid_argument("The number of rows cannot be 0!");
  if (cols == 0)
    throw std::invalid_argument("The number of columns canot be 0!");
  shape[0] = _rows;
  shape[1] = _cols;
  for (int i = 0; i < _rows; i++) {
    T *temp = new T[_cols];
    _container.push_back(std::move(temp));
  }
}
template <DataType T> Matrix<T>::Matrix(int rows, int cols, T def_val) : _rows{rows}, _cols{cols} {
  // Defaults value implicit from header file.
  if (rows == 0)
    throw std::invalid_argument("The number of rows cannot be 0!");
  if (cols == 0)
    throw std::invalid_argument("The number of columns canot be 0!");
  shape[0] = _rows;
  shape[1] = _cols;
  for (int i = 0; i < _rows; i++) {
    T *temp = new T[_cols];
    for (int j = 0; j < _cols; j++) {
      temp[j] = def_val;
    }
    _container.push_back(std::move(temp)); // Initialize vectors of numbers represented by def_val
  }
}
template <DataType T> Matrix<T>::Matrix(T **array, int rows, int cols) : _rows{rows}, _cols{cols} { // Pass by reference to 2-d array
  if (rows == 0)
    throw std::invalid_argument("The number of rows cannot be 0!");
  if (cols == 0)
    throw std::invalid_argument("The number of columns canot be 0!");

  for (int i = 0; i < rows; i++) {
    T *temp = new T[cols]{0};
    _container.push_back(temp);
    for (int j = 0; j < cols; j++) {
      _container[i][j] = array[i][j];
    }
  }

  shape[0] = _rows;
  shape[1] = _cols;
}
template <DataType T> Matrix<T>::Matrix(const Matrix<T> &other) {
  _rows = other._rows;
  _cols = other._cols;
  shape[0] = _rows;
  shape[1] = _cols;
  for (int i = 0; i < _rows; i++) {
    T *temp = new T[_cols]{0};
    _container.push_back(temp);
    for (int j = 0; j < _cols; j++) {
      _container[i][j] = other._container[i][j];
    }
  }
}
template <DataType T> Matrix<T>::Matrix(Matrix<T> &&other) {
  _container.clear();
  _rows = other._rows;
  _cols = other._cols;
  shape[0] = _rows;
  shape[1] = _cols;
  _container = std::move(other._container);

  // deleting contents in other Matrix.
  other._container.clear();
  other._rows = 0;
  other._cols = 0;
  other.shape[0] = 0;
  other.shape[1] = 0;
}
template <DataType T> Matrix<T>::~Matrix() {
  // Implicitly calls destructor for _contrainer.
  // Don't need to call delete on shape array.
}
template <DataType T> T const *const &Matrix<T>::operator[](int i) {
  if (i > _rows)
    throw std::invalid_argument("Index out of bounds!");
  else if (i < 0)
    throw std::invalid_argument("Index out of bounds!");
  return _container[i];
}
template <DataType T> T const *const &Matrix<T>::operator[](int i) const {
  // vector returned to caller cannot be modified by caller.
  if (i > _rows)
    throw std::invalid_argument("Index out of bounds!");
  else if (i < 0)
    throw std::invalid_argument("Index out of bounds!");

  return _container[i];
}
template <DataType T> void Matrix<T>::set_value(int r, int c, T val) { _container[r][c] = val; }
template <DataType T> T &Matrix<T>::get_value(int r, int c) { return _container[r][c]; }
template <DataType T> T &Matrix<T>::get_value(int r, int c) const { return _container[r][c]; }
template <DataType T> void Matrix<T>::set_value(int r, T *row) {
  // row should be a float array instantiated on heap.
  delete _container[r];
  _container[r] = row;
}
template <DataType T> Matrix<T> &Matrix<T>::operator=(const Matrix<T> &other) {
  _container.clear();
  _rows = other._rows;
  _cols = other._cols;
  shape[0] = _rows;
  shape[1] = _cols;
  for (int i = 0; i < _rows; i++) {
    for (int j = 0; j < _cols; j++) {
      _container[i][j] = other[i][j];
    }
  }

  return *this;
}
template <DataType T> Matrix<T> &Matrix<T>::operator=(Matrix<T> &&other) {
  _container.clear();
  _rows = other._rows;
  _cols = other._cols;
  shape[0] = _rows;
  shape[1] = _cols;
  _container = std::move(other._container);

  other._container.clear();
  other._rows = 0;
  other._cols = 0;
  other.shape[0] = 0;
  other.shape[1] = 0;
  return *this;
}
template <DataType T> const std::vector<float> Matrix<T>::getColumn(int j) const {
  /*
   * j is the jth column of matrix.
   * Given jth column and matrix, return all elements in jth column.
   */
  if (j > _cols)
    throw std::invalid_argument("Index out of bounds!");
  else if (j < 0)
    throw std::invalid_argument("Index out of bounds!");

  std::vector<float> out;
  for (int i = 0; i < (*this).shape[0]; i++) {
    out.push_back((*this)[i][j]);
  }
  return out;
}
template <DataType T> Matrix<T> operator+(const Matrix<T> &a, const Matrix<T> &b) {
  // Element-wise add two matrices.
  if (a.shape[0] != b.shape[0] || a.shape[1] != b.shape[1]) {
    throw std::invalid_argument("Shape mismatch between left and right Matrices");
  }
  if (a.shape[0] == 0 && a.shape[1] == 0) {
    return Matrix<T>{0, 0}; // empty matrix
  }
  Matrix<T> out{a.shape[0], a.shape[1]};
  std::vector<Status<T> *> items;
  for (int i = 0; i < a.shape[0]; i++) {
    Status<T> *temp = new Status<T>(out._worker.SubmitTask(&arr_op_sum, a[i], b[i], a.shape[1], b.shape[1]));
    items.push_back(temp);
  }
  int i = 0;
  for (auto const &ele : items) {
    std::future<T *> temp = ele->GetFuturePtr();
    temp.wait();
    out.set_value(i, temp.get());
    i++;
    delete ele;
  }
  items.clear();
  return std::move(out);
}
inline Matrix<bool> operator+(const Matrix<bool> &a, const Matrix<bool> &b) {
  // Element-wise add two matrices.
  if (a.shape[0] != b.shape[0] || a.shape[1] != b.shape[1]) {
    throw std::invalid_argument("Shape mismatch between left and right Matrices");
  }
  if (a.shape[0] == 0 && a.shape[1] == 0) {
    return Matrix<bool>{0, 0}; // empty matrix
  }
  Matrix<bool> out{a.shape[0], a.shape[1]};
  for (int i = 0; i < a.shape[0]; i++) {
    for (int j = 0; j < a.shape[1]; j++) {
      out.set_value(i, j, a.get_value(i, j) || b.get_value(i, j));
    }
  }
  return std::move(out);
}
template <DataType T> Matrix<T> operator-(const Matrix<T> &a, const Matrix<T> &b) {
  // Element-wise subtract two matrices.
  if (a.shape[0] != b.shape[0] || a.shape[1] != b.shape[1]) {
    throw std::invalid_argument("Shape mismatch between left and right Matrices");
  }
  if (a.shape[0] == 0 && a.shape[1] == 0) {
    return Matrix<T>{0, 0}; // empty matrix
  }
  Matrix<T> out{a.shape[0], a.shape[1]};
  std::list<Status<T> *> items;
  for (int i = 0; i < a.shape[0]; i++) {
    Status<T> *temp = new Status<T>(out._worker.SubmitTask(&arr_op_sub, a[i], b[i], a.shape[1], b.shape[1]));
    items.push_back(temp);
  }
  int i = 0;
  for (auto const &ele : items) {
    std::future<T *> temp = ele->GetFuturePtr();
    temp.wait();
    out.set_value(i, temp.get());
    i++;
    delete ele;
  }
  items.clear();
  return std::move(out);
}
inline Matrix<bool> operator-(const Matrix<bool> &a, const Matrix<bool> &b) {
  // Element-wise add two matrices.
  if (a.shape[0] != b.shape[0] || a.shape[1] != b.shape[1]) {
    throw std::invalid_argument("Shape mismatch between left and right Matrices");
  }
  if (a.shape[0] == 0 && a.shape[1] == 0) {
    return Matrix<bool>{0, 0}; // empty matrix
  }
  Matrix<bool> out{a.shape[0], a.shape[1]};
  for (int i = 0; i < a.shape[0]; i++) {
    for (int j = 0; j < a.shape[1]; j++) {
      out.set_value(i, j, a.get_value(i, j) && !b.get_value(i, j));
    }
  }
  return std::move(out);
}
template <DataType T> Matrix<T> operator*(const Matrix<T> &a, const Matrix<T> &b) {
  /* Dot product. This is not element-wise multiplication.
   * It is possible to return a 1x1 Matrix. Dot product of 2 vectors
   * represented as matrices will return 1x1 matrix that should be a scalar.
   *
   * */
  if (a.shape[1] != b.shape[0]) {
    throw std::invalid_argument("Shape mismatch between left and right Matrices");
  }
  if (a.shape[0] == 0 && a.shape[1] == 0) {
    return Matrix<T>{0, 0}; // empty matrix
  }

  Matrix<T> out{a.shape[0], b.shape[1]};
  int l_rows = a.shape[0];
  int l_cols = a.shape[1];
  int r_rows = b.shape[0];
  int r_cols = b.shape[1];
  const Matrix<T> int_mat = b.transpose();
  std::list<Status<T> *> items;
  for (int i = 0; i < l_rows; i++) {
    for (int j = 0; j < r_cols; j++) {
      Status<T> *temp = new Status<T>(a._worker.SubmitTask(&arr_op_dot_prod, a[i], int_mat[j], l_cols, r_rows));
      items.push_back(temp);
    }
  }
  for (int i = 0; i < l_rows; i++) {
    for (int j = 0; j < r_cols; j++) {
      std::future<T *> temp = items.front()->GetFuturePtr(); // pop_front is constant time.
      temp.wait();
      out.set_value(i, j, (*temp.get()));
      delete items.front();
      items.pop_front();
    }
  }
  items.clear();
  return std::move(out);
}
inline Matrix<bool> operator*(const Matrix<bool> &a, const Matrix<bool> &b) { throw NotImplemented("Invalid operation"); }
template <DataType T> Matrix<T> operator*(const float &a, const Matrix<T> &b) {
  // Multiply the matrice by a scalar a.
  if (b.shape[0] == 0 && b.shape[1] == 0) {
    return Matrix<T>{0, 0}; // empty matrix
  }

  Matrix<T> out{b.shape[0], b.shape[1]};
  int r_rows = b.shape[0];
  int r_cols = b.shape[1];
  std::vector<Status<T> *> items;
  for (int i = 0; i < r_rows; i++) {
    Status<T> *temp = new Status<T>(b._worker.SubmitTask(&arr_op_mul_c, b[i], b.shape[1], a));
    items.push_back(temp);
  }
  int i = 0;
  for (auto const &ele : items) {
    std::future<T *> temp = ele->GetFuturePtr();
    temp.wait();
    out.set_value(i, temp.get());
    i++;
    delete ele;
  }
  items.clear();
  return std::move(out);
}
inline Matrix<bool> operator*(const float &a, const Matrix<bool> &b) { throw NotImplemented("Invalid operation"); }
template <DataType T> Matrix<T> operator*(const int &a, const Matrix<T> &b) {
  // Multiply the matrice by a scalar a.
  if (b.shape[0] == 0 && b.shape[1] == 0) {
    return Matrix<T>{0, 0}; // empty matrix
  }
  float temp = a;
  return std::move(temp * b);
}
inline Matrix<bool> operator*(const int &a, const Matrix<bool> &b) { throw NotImplemented("Invalid operation"); }
template <DataType T> Matrix<T> operator*(const Matrix<T> &b, const int &a) { return std::move(a * b); }
inline Matrix<bool> operator*(const Matrix<bool> &b, const int &a) { throw NotImplemented("Invalid operation"); }
template <DataType T> Matrix<T> operator*(const Matrix<T> &b, const float &a) { return std::move(a * b); }
inline Matrix<bool> operator*(const Matrix<bool> &b, const float &a) { throw NotImplemented("Invalid operation"); }
template <DataType T> bool operator==(const Matrix<T> &a, const Matrix<T> &b) {
  if (a.shape[0] != b.shape[0] || a.shape[1] != b.shape[1]) {
    throw std::invalid_argument("Shape mismatch between left and right Matrices");
  }
  if (a.shape[0] == 0 && a.shape[1] == 0) {
    return true;
  }
  for (int i = 0; i < a.shape[0]; i++) {
    for (int j = 0; j < a.shape[1]; j++) {
      if (a[i][j] != b[i][j])
        return false;
    }
  }
  return true;
}
template <DataType T> bool operator!=(const Matrix<T> &a, const Matrix<T> &b) {
  if (a.shape[0] != b.shape[0] || a.shape[1] != b.shape[1]) {
    throw std::invalid_argument("Shape mismatch between left and right Matrices");
  }
  if (a.shape[0] == 0 && a.shape[1] == 0) {
    return false;
  }
  for (int i = 0; i < a.shape[0]; i++) {
    for (int j = 0; j < a.shape[1]; j++) {
      if (a[i][j] != b[i][j])
        return true;
    }
  }
  return false;
}
template <DataType T> Matrix<T> &Matrix<T>::operator+=(const Matrix<T> &other) {
  if ((*this).shape[0] != other.shape[0] || (*this).shape[1] != other.shape[1]) {
    throw std::invalid_argument("Shape mismatch between left and right Matrices");
  }
  if ((*this).shape[0] == 0 && (*this).shape[1] == 0) {
    return *this;
  }
  std::vector<Status<T> *> items;
  for (int i = 0; i < (*this).shape[0]; i++) {
    Status<T> *temp = new Status<T>((*this)._worker.SubmitTask(&arr_op_sum, (*this)[i], other[i], (*this).shape[1], other.shape[1]));
    items.push_back(temp);
  }
  int i = 0;
  for (auto const &ele : items) {
    std::future<T *> temp = ele->GetFuturePtr();
    temp.wait();
    (*this).set_value(i, temp.get());
    i++;
    delete ele;
  }
  items.clear();
  return *this;
}
template <> inline Matrix<bool> &Matrix<bool>::operator+=(const Matrix<bool> &other) {
  *this = *this + other; // TODO: Make sure this works
  return *this;
}
template <DataType T> Matrix<T> &Matrix<T>::operator-=(const Matrix<T> &other) {
  if ((*this).shape[0] != other.shape[0] || (*this).shape[1] != other.shape[1]) {
    throw std::invalid_argument("Shape mismatch between left and right Matrices");
  }
  if ((*this).shape[0] == 0 && (*this).shape[1] == 0) {
    return *this;
  }
  std::list<Status<T> *> items;
  for (int i = 0; i < (*this).shape[0]; i++) {
    Status<T> *temp = new Status<T>((*this)._worker.SubmitTask(&arr_op_sub, (*this)[i], other[i], (*this).shape[1], other.shape[1]));
    items.push_back(temp);
  }
  int i = 0;
  for (auto const &ele : items) {
    std::future<T *> temp = ele->GetFuturePtr();
    temp.wait();
    (*this).set_value(i, temp.get());
    i++;
    delete ele;
  }
  items.clear();
  return *this;
}
template <> inline Matrix<bool> &Matrix<bool>::operator-=(const Matrix<bool> &other) {
  *this = *this - other;
  return *this;
}
template <DataType T> Matrix<T> Matrix<T>::transpose() const {
  Matrix<T> out(_cols, _rows);
  for (int i = 0; i < (*this)._rows; i++) {
    for (int j = 0; j < (*this)._cols; j++) {
      out.set_value(j, i, _container[i][j]);
    }
  }
  return std::move(out);
}
template <DataType T> Matrix<T> Matrix<T>::transpose() {
  Matrix<T> out(_cols, _rows);
  for (int i = 0; i < (*this)._rows; i++) {
    for (int j = 0; j < (*this)._cols; j++) {
      out.set_value(j, i, _container[i][j]);
    }
  }
  return std::move(out);
}

template <DataType T> void Matrix<T>::print() {
  for (int i = 0; i < _rows; i++) {
    std::cout << '\n' << "___________" << '\n' << "| ";
    for (int j = 0; j < _cols; j++) {
      std::cout << (*this)[i][j] << " | ";
    }
  }
  std::cout << '\n' << "___________" << '\n';
}
template <DataType T> int Matrix<T>::ndim() {
  /*  Number of array dimensions. For a Matrix, this is 2.
   * */
  return 2;
}
template <DataType T> std::list<std::list<void *>> Matrix<T>::get_reffs() {
  /* list of addresses to each element in the Matrix.
   * */
  std::list<std::list<void *>> out{};
  for (int i = 0; i < _rows; i++) {
    std::list<void *> temp{};
    for (int j = 0; j < _cols; j++) {
      temp.push_back(&(_container[i][j]));
    }
    out.push_back(temp);
  }
  return out;
}
template <DataType T> unsigned int Matrix<T>::nbytes() { return sizeof(std::vector<T>) + (sizeof(T) * _rows * _cols); }
template <DataType T> int Matrix<T>::size() { return _rows * _cols; }
template <DataType T> int Matrix<T>::size() const { return _rows * _cols; }
template <DataType T> unsigned int Matrix<T>::itemsize() { return sizeof(T); }
template <DataType T> void Matrix<T>::flatten() {
  std::vector<T *> temp = std::move(_container);
  _container.clear();
  _container.push_back(new T[_cols * _rows]);
  int idx = 0;
  for (int i = 0; i < _rows; i++) {
    for (int j = 0; j < _cols; j++) {
      _container[0][idx] = temp[i][j];
			idx += 1;
    }
  }

  _cols = _rows * _cols;
  _rows = 1;
}

#endif
