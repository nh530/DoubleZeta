#include "Matrix.h"

float _vector_dotproduct(std::vector<float> left, std::vector<float> right) {
  /* Computes the dot product of two vectors that are the same size.
   *
   *
   */
  if (left.size() != right.size())
    throw std::invalid_argument("The two vectors must be same size!");

  int l_ele = left.size();
  float out;
  for (int i = 0; i < l_ele; i++) {
    out += left[i] * right[i];
  }

  return out;
}
Matrix::Matrix(int rows, int cols) : _rows{rows}, _cols{cols} {
	// Defaults value implicit from header file.
  shape[0] = _rows;
  shape[1] = _cols;
  for (int i; i < _rows; i++)
    _container.push_back(std::vector<float>(_cols)); // Initialize vectors of 0's
}
Matrix::Matrix(float **array, int rows, int cols) { // Pass by reference to 2-d array
  for (int i; i < rows; i++) {
    _container.push_back(std::vector<float>{});
    for (int j; j < cols; j++) {
      _container[i].push_back(array[i][j]);
    }
  }
  _rows = rows;
  _cols = cols;
  shape[0] = _rows;
  shape[1] = _cols;
}
Matrix::Matrix(const Matrix &other) {
  _rows = other._rows;
  _cols = other._cols;
  shape[0] = _rows;
  shape[1] = _cols;
  for (int i; i < _rows; i++) {
    // There should not be any need to copy every element over.
    _container.push_back(other[i]);
  }
}
Matrix::Matrix(Matrix &&other) {
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
Matrix::~Matrix() {
  // Implicitly calls destructor for _contrainer.
  // Don't need to call delete on shape array.
}
std::vector<float> &Matrix::operator[](int i) { return _container[i]; }
const std::vector<float> &Matrix::operator[](int i) const {
  // vector returned to caller cannot be modified by caller.
  return _container[i];
}
Matrix &Matrix::operator=(const Matrix &other) {
  _container.clear();
  _rows = other._rows;
  _cols = other._cols;
  shape[0] = _rows;
  shape[1] = _cols;
  for (int i; i < _rows; i++) {
    _container.push_back(other[i]);
  }
  return *this;
}
Matrix &Matrix::operator=(Matrix &&other) {
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
std::vector<float> Matrix::_get_column(const Matrix matx, int j) const {
  /*
   * j is the jth column of matrix.
   * Given jth column and matrix, return all elements in jth column.
   */
  std::vector<float> out;
  for (int i = 0; i < matx.shape[0]; i++) {
    out.push_back(matx[i][j]);
  }
  return out;
}
Matrix operator+(const Matrix &a, const Matrix &b) {
  if (a.shape[0] != b.shape[0] && a.shape[1] != b.shape[1]) {
    throw std::invalid_argument("Shape mismatch between left and right Matrices");
  }
  if (a.shape[0] == 0 && a.shape[1] == 0) {
    return Matrix{0, 0}; // empty matrix
  }
  Matrix out{a.shape[0], a.shape[1]};
  for (int i; i < a.shape[0]; i++) {
    for (int j; j < a.shape[1]; j++) {
      out[i][j] = a[i][j] + b[i][j];
    }
  }
  return out;
}
Matrix operator-(const Matrix &a, const Matrix &b) {
  if (a.shape[0] != b.shape[0] && a.shape[1] != b.shape[1]) {
    throw std::invalid_argument("Shape mismatch between left and right Matrices");
  }
  if (a.shape[0] == 0 && a.shape[1] == 0) {
    return Matrix{0, 0}; // empty matrix
  }
  Matrix out{a.shape[0], a.shape[1]};
  for (int i; i < a.shape[0]; i++) {
    for (int j; j < a.shape[1]; j++) {
      out[i][j] = a[i][j] - b[i][j];
    }
  }
  return out;
}

Matrix operator*(const Matrix &a, const Matrix &b) {
  /* It is possible to return a 1x1 Matrix. Dot product of 2 vectors represented as matrices will return 1x1 matrix that should be a scalar.
   *
   * */
  if (a.shape[0] != b.shape[1] && a.shape[1] != b.shape[0]) {
    throw std::invalid_argument("Shape mismatch between left and right Matrices");
  }
  if (a.shape[0] == 0 && a.shape[1] == 0) {
    return Matrix{0, 0}; // empty matrix
  }

  Matrix out{a.shape[0], b.shape[1]};
  int l_rows = a.shape[0];
  int l_cols = a.shape[1];
  int r_rows = b.shape[0];
  int r_cols = b.shape[1];

  for (int i = 0; i < l_rows; i++) {
    for (int j = 0; j < r_cols; j++) {
      out[i][j] = _vector_dotproduct(a[i], a._get_column(b, j));
    }
  }

  return out;
}

Matrix operator*(const int &a, const Matrix &b) {
  if (b.shape[0] == 0 && b.shape[1] == 0) {
    return Matrix{0, 0}; // empty matrix
  }

  Matrix out{b.shape[0], b.shape[1]};
  int r_rows = b.shape[0];
  int r_cols = b.shape[1];

  for (int i = 0; i < r_rows; i++) {
    for (int j = 0; j < r_cols; j++) {
      out[i][j] = a * b[i][j];
    }
  }

  return out;
}
Matrix operator*(const float &a, const Matrix &b) {
  if (b.shape[0] == 0 && b.shape[1] == 0) {
    return Matrix{0, 0}; // empty matrix
  }

  Matrix out{b.shape[0], b.shape[1]};
  int r_rows = b.shape[0];
  int r_cols = b.shape[1];

  for (int i = 0; i < r_rows; i++) {
    for (int j = 0; j < r_cols; j++) {
      out[i][j] = a * b[i][j];
    }
  }

  return out;
}
bool operator==(const Matrix &a, const Matrix &b) {
  if (a.shape[0] != b.shape[0] && a.shape[1] != b.shape[1]) {
    throw std::invalid_argument("Shape mismatch between left and right Matrices");
  }
  if (a.shape[0] == 0 && a.shape[1] == 0) {
    return true;
  }
  for (int i; i < a.shape[0]; i++) {
    for (int j; j < a.shape[1]; j++) {
      if (a[i][j] != b[i][j])
        return false;
    }
  }
  return true;
}
bool operator!=(const Matrix &a, const Matrix &b) {
  if (a.shape[0] != b.shape[0] && a.shape[1] != b.shape[1]) {
    throw std::invalid_argument("Shape mismatch between left and right Matrices");
  }
  if (a.shape[0] == 0 && a.shape[1] == 0) {
    return false;
  }
  for (int i; i < a.shape[0]; i++) {
    for (int j; j < a.shape[1]; j++) {
      if (a[i][j] != b[i][j])
        return true;
    }
  }
  return false;
}

Matrix Matrix::operator+=(const Matrix other) {
  Matrix out{other._rows, other._cols};
  return *this + out;
}
Matrix Matrix::operator-=(const Matrix other) {
  Matrix out{other._rows, other._cols};
  return *this - out;
}
Matrix Matrix::transpose() {
  Matrix out{this->_rows, this->_cols};
  for (int i; i < this->_rows; i++) {
    for (int j; j < this->_cols; j++) {
      out[j][i] = this->_container[i][j];
    }
  }
  return out;
}
