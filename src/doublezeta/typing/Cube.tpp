#ifndef CUBE_TPP
#define CUBE_TPP
#include "Cube.h"
#include "typing/exceptions.h"
#include <cmath>
#include <iostream>
#include <list>

template <int_or_float N, DataType T> N find_sum(const Cube<T> &arr) {
  // TODO: Needs to be changed
  N out;
  for (int i = 0; i < arr.shape[0]; i++) {
    for (int j = 0; j < arr.shape[1]; j++) {
      for (int k = 0; k < arr.shape[2]; k++) {
        out += arr.get_value(i, j, k);
      }
    }
  }
  return out;
}
template <int_or_float N, DataType T> N find_max(const Cube<T> &arr) {
  N out;
  for (int i = 0; i < arr.shape[0]; i++) {
    for (int j = 0; j < arr.shape[1]; j++) {
      for (int k = 0; k < arr.shape[2]; k++) {
        if (out < arr.get_value(i, j, k)) {
          out = arr.get_value(i, j, k);
        }
      }
    }
  }
  return out;
}
template <int_or_float N, DataType T> N find_min(const Cube<T> &arr) {
  N out;
  for (int i = 0; i < arr.shape[0]; i++) {
    for (int j = 0; j < arr.shape[1]; j++) {
      for (int k = 0; k < arr.shape[2]; k++) {
        if (out > arr.get_value(i, j, k)) {
          out = arr.get_value(i, j, k);
        }
      }
    }
  }
  return out;
}
template <DataType T> std::tuple<int, int, int> find_argmax(const Cube<T> &arr) {
  std::tuple<int, int, int> out;
  float largest = 0;
  for (int i = 0; i < arr.shape[0]; i++) {
    for (int j = 0; j < arr.shape[1]; j++) {
      for (int k = 0; k < arr.shape[2]; k++) {
        if (largest > arr.get_value(i, j, k)) {
          largest = arr.get_value(i, j, k);
          std::get<0>(out) = i;
          std::get<1>(out) = j;
          std::get<2>(out) = k;
        }
      }
    }
  }
  return out;
}
template <DataType T> std::tuple<int, int, int> find_argmin(const Cube<T> &arr) {
  std::tuple<int, int, int> out;
  float smallest;
  for (int i = 0; i < arr.shape[0]; i++) {
    for (int j = 0; j < arr.shape[1]; j++) {
      for (int k = 0; k < arr.shape[2]; k++) {
        if (smallest > arr.get_value(i, j, k)) {
          smallest = arr.get_value(i, j, k);
          std::get<0>(out) = i;
          std::get<1>(out) = j;
          std::get<2>(out) = k;
        }
      }
    }
  }
  return out;
}
template <int_or_float N, DataType T> N find_cumsum(const Cube<T> &arr) {
  N out = 0;
  for (int i = 0; i < arr.shape[0]; i++) {
    for (int j = 0; j < arr.shape[1]; j++) {
      for (int k = 0; k < arr.shape[2]; k++) {
        out += arr.get_value(i, j, k);
      }
    }
  }
  return out;
}
template <int_or_float N, DataType T> N find_mean(const Cube<T> &arr) {
  N out = 0;
  for (int i = 0; i < arr.shape[0]; i++) {
    for (int j = 0; j < arr.shape[1]; j++) {
      for (int k = 0; k < arr.shape[2]; k++) {
        out += arr.get_value(i, j, k);
      }
    }
  }
  return out / (arr.shape[0] * arr.shape[1] * arr.shape[2]);
}
template <int_or_float N, DataType T> N find_var(const Cube<T> &arr) {
  N sums = 0;
  int smpls = (arr.shape[0] * arr.shape[1] * arr.shape[2]);
  for (int i = 0; i < arr.shape[0]; i++) {
    for (int j = 0; j < arr.shape[1]; j++) {
      for (int k = 0; k < arr.shape[2]; k++) {
        sums += arr.get_value(i, j);
      }
    }
  }
  Cube<T> temp = (arr - (sums / smpls));
  return find_sum(temp * temp.transpose()) / smpls; // matrix-wise dot product
}
template <int_or_float N, DataType T> N find_std(const Cube<T> &arr) { return std::sqrt(find_var(arr)); }
template <DataType T> Cube<T> choose(const Cube<T> &left, const Cube<T> &right, const Cube<bool> &mask) {
  if (left.shape[0] != right.shape[0] || left.shape[1] != right.shape[1] || left.shape[2] != right.shape[2])
    throw std::invalid_argument("Error: The two matrices differ in size");
  if (left.shape[0] != mask.shape[0] || left.shape[1] != mask.shape[1] || left.shape[2] != mask.shape[2])
    throw std::invalid_argument("Error: The mask is not the same size as the input matrices");
  Cube<T> out{left.shape[0], left.shape[1], left.shape[2]};
  for (int i = 0; i < out.shape[0]; i++) {
    for (int j = 0; j < out.shape[1]; j++) {
      for (int k = 0; k < out.shape[2]; k++) {
        if (mask.get_value(i, j, k)) {
          out.set_value(i, j, k, left.get_value(i, j));
        } else {
          out.set_value(i, j, k, right.get_value(i, j));
        }
      }
    }
  }
  return out;
}
template <DataType T> ZetaSession Cube<T>::_worker = ZetaSession(-1);
template <DataType T> Cube<T>::Cube() {
  // Default constructor; Initialize nothing.
}
template <DataType T> Cube<T>::Cube(int x, int y, int z) : _rows{x}, _cols{y}, _depth{z} {
  // CAUTION: This constructor leads to undefined behavior since values in array are not set.
  if (x == 0)
    throw std::invalid_argument("The number of rows cannot be 0!");
  if (y == 0)
    throw std::invalid_argument("The number of columns canot be 0!");
  shape[0] = _rows;
  shape[1] = _cols;
  shape[2] = _depth;
  for (int i = 0; i < _depth; i++) {
    std::vector<T *> new_row;
    for (int j = 0; j < _rows; j++) {
      T *new_col = new T[_cols];
      new_row.push_back(std::move(new_col));
    }
    _container.push_back(std::move(new_row));
  }
}
template <DataType T> Cube<T>::Cube(int x, int y, int z, T def_val) : _rows{x}, _cols{y}, _depth{z} {
  // Defaults value implicit from header file.
  if (x == 0)
    throw std::invalid_argument("The number of rows cannot be 0!");
  if (y == 0)
    throw std::invalid_argument("The number of columns canot be 0!");
  shape[0] = _rows;
  shape[1] = _cols;
  shape[2] = _depth;
  for (int i = 0; i < _depth; i++) {
    std::vector<T *> new_frame;
    for (int k = 0; k < _rows; k++) {
      T *temp = new T[_cols];
      for (int j = 0; j < _cols; j++) {
        temp[j] = def_val;
      }
      new_frame.push_back(std::move(temp));
    }
    _container.push_back(std::move(new_frame)); // Initialize vectors of numbers represented by def_val
  }
}
template <DataType T> Cube<T>::Cube(T ***arr, int x, int y, int z) : _rows{x}, _cols{y}, _depth{z} { // Pointer to 3d array
  if (x == 0)
    throw std::invalid_argument("The number of rows cannot be 0!");
  if (y == 0)
    throw std::invalid_argument("The number of columns canot be 0!");

  for (int i = 0; i < z; i++) {
    std::vector<T *> new_frame;
    for (int j = 0; j < x; j++) {
      T *cols = new T[_cols];
      for (int k = 0; k < y; k++) {
        cols[k] = arr[j][k][i];
      }
      new_frame.push_back(std::move(cols));
    }
    _container.push_back(std::move(new_frame));
  }

  shape[0] = _rows;
  shape[1] = _cols;
  shape[2] = _depth;
}
template <DataType T> Cube<T>::Cube(const Cube<T> &other) {
  _rows = other._rows;
  _cols = other._cols;
  _depth = other._depth;
  shape[0] = _rows;
  shape[1] = _cols;
  shape[2] = _depth;
  for (int i = 0; i < _depth; i++) {
    std::vector<T *> new_frame;
    _container.push_back(new_frame);
    for (int j = 0; j < _rows; j++) {
      T *new_row = new T[_cols]{0};
      _container[i].push_back(std::move(new_row));
      for (int k = 0; k < _cols; k++) {
        _container[i][j][k] = other._container[i][j][k];
      }
    }
  }
}
template <DataType T> Cube<T>::Cube(Cube<T> &&other) {
  _container.clear();
  _rows = other._rows;
  _cols = other._cols;
  _depth = other._depth;
  shape[0] = _rows;
  shape[1] = _cols;
  shape[2] = _depth;
  _container = std::move(other._container);

  // deleting contents in other Matrix.
  other._container.clear();
  other._rows = 0;
  other._cols = 0;
  other._depth = 0;
  other.shape[0] = 0;
  other.shape[1] = 0;
  other.shape[2] = 0;
}
template <DataType T> Cube<T>::~Cube() {
  // Implicitly calls destructor for _contrainer.
  // Don't need to call delete on shape array.
}
template <DataType T> T &Cube<T>::operator()(unsigned int x, unsigned int y, unsigned int z) {
  if (x > _rows)
    throw std::invalid_argument("Index out of bounds!");
  if (y > _cols)
    throw std::invalid_argument("Index out of bounds!");
  if (z > _depth)
    throw std::invalid_argument("Index out of bounds!");

  return _container[z][x][y];
}
template <DataType T> void Cube<T>::set_value(int x, int y, int z, T val) { _container[z][x][y] = val; }
template <DataType T> T &Cube<T>::get_value(int x, int y, int z) { return _container[z][x][y]; }
template <DataType T> T &Cube<T>::get_value(int x, int y, int z) const { return _container[z][x][y]; }
template <DataType T> Cube<T> &Cube<T>::get_value(const Cube<bool> &) {
  // TODO
}
template <DataType T> Cube<T> &Cube<T>::operator=(const Cube<T> &other) {
  _container.clear();
  _rows = other._rows;
  _cols = other._cols;
  _depth = other._depth;
  shape[0] = _rows;
  shape[1] = _cols;
  shape[2] = _depth;
  for (int i = 0; i < _rows; i++) {
    for (int j = 0; j < _cols; j++) {
      for (int k = 0; k < _depth; k++) {
        _container[k][i][j] = other._container[k][i][j];
      }
    }
  }

  return *this;
}
template <DataType T> Cube<T> &Cube<T>::operator=(Cube<T> &&other) {
  _container.clear();
  _rows = other._rows;
  _cols = other._cols;
  _depth = other._depth;
  shape[0] = _rows;
  shape[1] = _cols;
  shape[2] = _depth;
  _container = std::move(other._container);

  other._container.clear();
  other._rows = 0;
  other._cols = 0;
  other._depth = 0;
  other.shape[0] = 0;
  other.shape[1] = 0;
  other.shape[2] = 0;
  return *this;
}
template <DataType T> Cube<T> operator+(const Cube<T> &a, const Cube<T> &b) {
  // Element-wise add two matrices.
  if (a.shape[0] != b.shape[0] || a.shape[1] != b.shape[1] || a.shape[2] != b.shape[2]) {
    throw std::invalid_argument("Shape mismatch between left and right Matrices");
  }
  if (a.shape[0] == 0 && a.shape[1] == 0 && a.shape[2] == 0) {
    return Cube<T>{0, 0, 0}; // empty matrix
  }
  Cube<T> out{a.shape[0], a.shape[1], a.shape[2]};
  std::list<Status<T> *> items;
  for (int z = 0; z < a.shape[2]; z++) {
    for (int i = 0; i < a.shape[0]; i++) {
      Status<T> *temp = new Status<T>(out._worker.SubmitTask(&arr_op_sum, a._container[z][i], b._container[z][i], a.shape[1], b.shape[1]));
      items.push_back(temp);
    }
  }
  for (int z = 0; z < a.shape[2]; z++) {
    for (int i = 0; i < a.shape[0]; i++) {
      std::future<T *> temp = items.front()->GetFuturePtr();
      temp.wait();
      auto res = temp.get();
      for (int j = 0; j < a.shape[1]; j++) {
        out.set_value(i, j, z, res[j]);
      }
      delete items.front();
      items.pop_front();
    }
  }
  items.clear();
  return std::move(out);
}
template <DataType T> Cube<T> operator-(const Cube<T> &a, const Cube<T> &b) {
  // Element-wise subtract two matricesa
  if (a.shape[0] != b.shape[0] || a.shape[1] != b.shape[1] || a.shape[2] != b.shape[2]) {
    throw std::invalid_argument("Shape mismatch between left and right Matrices");
  }
  if (a.shape[0] == 0 && a.shape[1] == 0 && a.shape[2] == 0) {
    return Cube<T>{0, 0, 0}; // empty matrix
  }
  Cube<T> out{a.shape[0], a.shape[1], a.shape[2]};
  std::list<Status<float> *> items;
  for (int z = 0; z < a.shape[2]; z++) {
    for (int i = 0; i < a.shape[0]; i++) {
      Status<T> *temp = new Status<float>(out._worker.SubmitTask(&arr_op_sub, a._container[z][i], b._container[z][i], a.shape[1], b.shape[1]));
      items.push_back(temp);
    }
  }
  for (int z = 0; z < a.shape[2]; z++) {
    for (int i = 0; i < a.shape[0]; i++) {
      std::future<T *> temp = items.front()->GetFuturePtr();
      temp.wait();
      T *res = temp.get();
      for (int j = 0; j < a.shape[1]; j++) { // Iterate over column
        out.set_value(i, j, z, res[j]);
      }
			delete items.front();
			items.pop_front();
    }
  }
  items.clear();
  return std::move(out);
}
template <DataType T> Cube<T> operator*(const Cube<T> &a, const Cube<T> &b) {
  /* Dot product. This is not element-wise multiplication.
   * It is possible to return a 1x1 Matrix. Dot product of 2 vectors
   * represented as matrices will return 1x1 matrix that should be a scalar.
   *
   * */
  if (a.shape[1] != b.shape[0] || a.shape[2] != b.shape[2]) {
    throw std::invalid_argument("Shape mismatch between left and right Matrices");
  }
  if (a.shape[0] == 0 && a.shape[1] == 0) {
    return Cube<T>{0, 0, 0}; // empty matrix
  }

  Cube<T> out{a.shape[0], b.shape[1], b.shape[2]};
  int l_rows = a.shape[0];
  int l_cols = a.shape[1];
  int r_rows = b.shape[0];
  int r_cols = b.shape[1];
  const Cube<T> int_mat = b.transpose(); // TODO: This might be an issue; Assumes tranpose only applied to each matrix
  std::list<Status<T> *> items;
  for (int z = 0; z < a.shape[2]; z++) { // Iterate over each frame
    for (int i = 0; i < l_rows; i++) {
      for (int j = 0; j < r_cols; j++) {
        Status<T> *temp = new Status<T>(a._worker.SubmitTask(&arr_op_dot_prod, a._container[z][i], int_mat._container[z][j], l_cols, r_rows));
        items.push_back(temp);
      }
    }
  }
  for (int z = 0; z < a.shape[2]; z++) {
    for (int i = 0; i < l_rows; i++) {
      for (int j = 0; j < r_cols; j++) {
        std::future<T *> temp = items.front()->GetFuturePtr(); // pop_front is constant time.
        temp.wait();
        out.set_value(i, j, z, (*temp.get()));
        delete items.front();
        items.pop_front();
      }
    }
  }
  items.clear();
  return std::move(out);
}
template <DataType T> Cube<T> operator*(const float &a, const Cube<T> &b) {
  // Multiply the matrice by a scalar a.
  if (b.shape[0] == 0 && b.shape[1] == 0) {
    return Cube<T>{0, 0, 0}; // empty matrix
  }

  Cube<T> out{b.shape[0], b.shape[1], b.shape[2]};
  int r_rows = b.shape[0];
  int r_cols = b.shape[1];
  std::list<Status<T> *> items;
  for (int z = 0; z < b.shape[2]; z++) {
    for (int i = 0; i < r_rows; i++) {
      Status<T> *temp = new Status<T>(b._worker.SubmitTask(&arr_op_mul_c, b._container[z][i], b.shape[1], a));
      items.push_back(temp);
    }
  }
  for (int z = 0; z < b.shape[2]; z++) {
    for (int i = 0; i < r_rows; i++) {
      std::future<T *> temp = items.front()->GetFuturePtr();
      temp.wait();
      T *res = temp.get();
      for (int j = 0; j < r_cols; j++) {
        out.set_value(i, j, z, res[j]);
      }
      delete items.front();
      items.pop_front();
    }
  }
  items.clear();
  return std::move(out);
}
template <DataType T> Cube<T> operator*(const int &a, const Cube<T> &b) {
  // Multiply the matrice by a scalar a.
  if (b.shape[0] == 0 && b.shape[1] == 0) {
    return Cube<T>{0, 0, b.shape[2]}; // empty matrix
  }
  float temp = (float)a;

  return std::move(temp * b);
}
template <DataType T> Cube<T> operator*(const Cube<T> &b, const int &a) { return a * b; }
template <DataType T> Cube<T> operator*(const Cube<T> &b, const float &a) { return a * b; }
template <DataType T> bool operator==(const Cube<T> &a, const Cube<T> &b) {
  if (a.shape[0] != b.shape[0] || a.shape[1] != b.shape[1] || a.shape[2] != b.shape[2]) {
    throw std::invalid_argument("Shape mismatch between left and right Matrices");
  }
  if (a.shape[0] == 0 && a.shape[1] == 0) {
    return true;
  }

  for (int k = 0; k < a.shape[2]; k++) {
    for (int i = 0; i < a.shape[0]; i++) {
      for (int j = 0; j < a.shape[1]; j++) {
        if (a.get_value(i, j, k) != b.get_value(i, j, k))
          return false;
      }
    }
  }
  return true;
}
template <DataType T> bool operator!=(const Cube<T> &a, const Cube<T> &b) {
  if (a.shape[0] != b.shape[0] || a.shape[1] != b.shape[1] || a.shape[2] != b.shape[2]) {
    throw std::invalid_argument("Shape mismatch between left and right Matrices");
  }
  if (a.shape[0] == 0 && a.shape[1] == 0) {
    return false;
  }

  for (int k = 0; k < a.shape[2]; k++) {
    for (int i = 0; i < a.shape[0]; i++) {
      for (int j = 0; j < a.shape[1]; j++) {
        if (a.get_value(i, j, k) != b.get_value(i, j, k))
          return true;
      }
    }
  }
  return false;
}
template <DataType T> Cube<T> Cube<T>::operator+=(const Cube<T> &other) {
  if ((*this).shape[0] != other.shape[0] || (*this).shape[1] != other.shape[1] || (*this).shape[2] != other.shape[2]) {
    throw std::invalid_argument("Shape mismatch between left and right Matrices");
  }
  if ((*this).shape[0] == 0 && (*this).shape[1] == 0) {
    return *this;
  }
  std::list<Status<T> *> items;
  for (int z = 0; z < (*this).shape[2]; z++) {
    for (int i = 0; i < (*this).shape[0]; i++) {
      Status<T> *temp =
          new Status<T>((*this)._worker.SubmitTask(&arr_op_sum, (*this)._container[z][i], other._container[z][i], (*this).shape[1], other.shape[1]));
      items.push_back(temp);
    }
  }
  for (int z = 0; z < (*this).shape[2]; z++) {
    for (int i = 0; i < (*this).shape[0]; i++) {
      std::future<T *> temp = items.front()->GetFuturePtr();
      temp.wait();
      T *res = temp.get();
      for (int j = 0; j < (*this).shape[1]; j++) {
        (*this).set_value(i, j, z, res[j]);
      }
      delete items.front();
      items.pop_front();
    }
  }
  items.clear();
  return *this;
}
template <DataType T> Cube<T> Cube<T>::operator-=(const Cube<T> &other) {
  if (shape[0] != other.shape[0] || shape[1] != other.shape[1] || shape[2] != other.shape[2]) {
    throw std::invalid_argument("Shape mismatch between left and right Matrices");
  }
  if (shape[0] == 0 && shape[1] == 0) {
    return *this;
  }
  std::list<Status<T> *> items;
  for (int z = 0; z < shape[2]; z++) {
    for (int i = 0; i < shape[0]; i++) {
      Status<T> *temp = new Status<T>(_worker.SubmitTask(&arr_op_sub, _container[z][i], other._container[z][i], shape[1], other.shape[1]));
      items.push_back(temp);
    }
  }
  for (int z = 0; z < shape[2]; z++) {
    for (int i = 0; i < shape[0]; i++) {
      std::future<T *> temp = items.front()->GetFuturePtr();
      temp.wait();
      T *res = temp.get();
      for (int j = 0; j < shape[1]; j++) {
        this->set_value(i, j, z, res[j]);
      }
			delete items.front();
			items.pop_front();
    }
  }
  items.clear();
  return *this;
}
template <DataType T> Cube<T> Cube<T>::transpose() const {
  Cube<T> out(_cols, _rows, _depth);
  for (int k = 0; k < _depth; k++) {
    for (int i = 0; i < _rows; i++) {
      for (int j = 0; j < _cols; j++) {
        out.set_value(j, i, k, _container[k][i][j]);
      }
    }
  }
  return std::move(out);
}
template <DataType T> Cube<T> Cube<T>::transpose() {
  Cube<T> out(_cols, _rows, _depth);
  for (int k = 0; k < _depth; k++) {
    for (int i = 0; i < _rows; i++) {
      for (int j = 0; j < _cols; j++) {
        out.set_value(j, i, k, _container[k][i][j]);
      }
    }
  }
  return std::move(out);
}
template <DataType T> Cube<T> Cube<T>::transpose(unsigned int dim0, unsigned int dim1) {
  Cube<T> out(_cols, _rows, _depth);
  if ((dim0 == 0 && dim1 == 1) || (dim0 == 1 && dim1 == 0)) {
    for (int k = 0; k < _depth; k++) {
      for (int i = 0; i < _rows; i++) {
        for (int j = 0; j < _cols; j++) {
          out.set_value(j, i, k, _container[k][i][j]);
        }
      }
    }
  } else if ((dim0 == 0 && dim1 == 2) || (dim0 == 2 && dim1 == 0)) {
    for (int k = 0; k < _depth; k++) {
      for (int i = 0; i < _rows; i++) {
        for (int j = 0; j < _cols; j++) {
          out.set_value(k, j, i, _container[k][i][j]);
        }
      }
    }
  } else if ((dim0 == 1 && dim1 == 2) || (dim0 == 2 && dim1 == 1)) {
    for (int k = 0; k < _depth; k++) {
      for (int i = 0; i < _rows; i++) {
        for (int j = 0; j < _cols; j++) {
          out.set_value(i, k, j, _container[k][i][j]);
        }
      }
    }
  } else {
    throw std::invalid_argument("Error: Invalid argument!");
  }
  return std::move(out);
}
template <DataType T> void Cube<T>::print() {
  for (int z = 0; z < _depth; z++) {
    std::cout << '\n' << "___________";
    for (int i = 0; i < _rows; i++) {
      std::cout << '\n' << "___________" << '\n' << "| ";
      for (int j = 0; j < _cols; j++) {
        std::cout << _container[z][i][j] << " | ";
      }
    }
  }
  std::cout << '\n' << "___________" << '\n';
}
template <DataType T> int Cube<T>::ndim() {
  /*  Number of array dimensions. For a Matrix, this is 2.
   * */
  return 3;
}
template <DataType T> std::list<std::list<std::list<void *>>> Cube<T>::get_reffs() {
  /* list of addresses to each element in the Matrix.
   * */
  std::list<std::list<std::list<void *>>> out{};
  for (int k = 0; k < _depth; k++) {
    std::list<std::list<void *>> frame{};
    for (int i = 0; i < _rows; i++) {
      std::list<void *> temp{};
      for (int j = 0; j < _cols; j++) {
        temp.push_back(&(_container[k][i][j]));
      }
      frame.push_back(temp);
    }
    out.push_back(frame);
  }
  return out;
}
template <DataType T> unsigned int Cube<T>::nbytes() { return _depth * _rows * _cols * sizeof(T); }
template <DataType T> int Cube<T>::size() { return _depth * _rows * _cols; }
template <DataType T> int Cube<T>::size() const { return _depth * _rows * _cols; }
template <DataType T> unsigned int Cube<T>::itemsize() { return sizeof(T *); }
template <DataType T> void Cube<T>::reshape(int x, int y, int z) {
  // Reshapes the current Cube into a new one with number of elements equal to size.
  bool is_ext = true;

  if (x * y * z < size()) {
    std::cout << "Warning: Resizing to a smaller sized Cube. Data will be truncated" << '\n';
    is_ext = false;
  }

  T temp[_depth * _rows * _cols];
  int idx = 0;

  for (int k = 0; k < z; k++) {
    for (int i = 0; i < x; i++) {
      for (int j = 0; j < y; j++) {
        temp[idx] = _container[k][i][j];
        idx++;
      }
    }
  }

  _container.clear();
  idx = 0;

  for (int k = 0; k < z; k++) {
    std::vector<T *> mat;
    _container.push_back(mat);
    for (int i = 0; i < x; i++) {
      T *row = new T[x];
      _container[k].push_back(row);
      for (int j = 0; j < y; j++) {
        if (idx >= _depth * _rows * _cols) {
          _container[k][i][j] = 0;
        } else {
          _container[k][i][j] = temp[idx];
          idx++;
        }
      }
    }
  }

  _rows = x;
  _cols = y;
  _depth = z;
}
template <DataType T> void Cube<T>::flatten() {
  // Flatten into a "Vector".
  T temp[_depth * _rows * _cols];
  int idx = 0;
  for (int k = 0; k < _depth; k++) {
    for (int i = 0; i < _rows; i++) {
      for (int j = 0; j < _cols; j++) {
        temp[idx] = _container[k][i][j];
        idx++;
      }
    }
  }
  _container.clear();
  _container.push_back(std::vector<T *>{new T[_depth * _rows * _cols]}); // TODO: not sure

  for (int i = 0; i < _cols; i++) {
    _container[0][0][i] = temp[i];
  }

  _cols = _depth * _rows * _cols;
  _depth = 1;
  _rows = 1;
}

#endif
