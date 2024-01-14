#include "Matrix.h"
#include "algba_cmd/Algebra.h"
#include "typing/exceptions.h"
#include <iostream>
#include <list>
float _vector_dotproduct(std::vector<float> left, std::vector<float> right) {
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
void vector_to_arr(std::vector<float> vec, float *out) {
  for (int i = 0; i < vec.size(); i++) {
    out[i] = vec[i];
  }
}
ZetaSession Matrix::_worker = ZetaSession(-1);
Matrix::Matrix() {
  // Default constructor; Initialize nothing.
}
Matrix::Matrix(int rows, int cols) : _rows{rows}, _cols{cols} {
  // Defaults value implicit from header file.
  // # TODO: throw error if pass 0 rows 0 cols.
  // CAUTION: This constructor leads to undefined behavior if values in array are not set.
  // TODO: Maybe it will be best to hide this constructor from User.
  if (rows == 0)
    throw std::invalid_argument("The number of rows cannot be 0!");
  if (cols == 0)
    throw std::invalid_argument("The number of columns canot be 0!");
  shape[0] = _rows;
  shape[1] = _cols;
  for (int i = 0; i < _rows; i++) {
    float *temp = new float[_cols];
    _container.push_back(std::move(temp)); // Initialize vectors of numbers represented by def_val
  }
}

Matrix::Matrix(int rows, int cols, float def_val) : _rows{rows}, _cols{cols} {
  // Defaults value implicit from header file.
  // # TODO: throw error if pass 0 rows 0 cols.
  if (rows == 0)
    throw std::invalid_argument("The number of rows cannot be 0!");
  if (cols == 0)
    throw std::invalid_argument("The number of columns canot be 0!");
  shape[0] = _rows;
  shape[1] = _cols;
  for (int i = 0; i < _rows; i++) {
    float *temp = new float[_cols];
    for (int j = 0; j < _cols; j++) {
      temp[j] = def_val;
    }
    _container.push_back(std::move(temp)); // Initialize vectors of numbers represented by def_val
  }
}
Matrix::Matrix(float **array, int rows, int cols) : _rows{rows}, _cols{cols} { // Pass by reference to 2-d array
  if (rows == 0)
    throw std::invalid_argument("The number of rows cannot be 0!");
  if (cols == 0)
    throw std::invalid_argument("The number of columns canot be 0!");

  for (int i = 0; i < rows; i++) {
    float *temp = new float[cols]{0};
    _container.push_back(temp);
    for (int j = 0; j < cols; j++) {
      _container[i][j] = array[i][j];
    }
  }

  shape[0] = _rows;
  shape[1] = _cols;
}
Matrix::Matrix(const Matrix &other) {
  _rows = other._rows;
  _cols = other._cols;
  shape[0] = _rows;
  shape[1] = _cols;
  for (int i = 0; i < _rows; i++) {
    float *temp = new float[_cols]{0};
    _container.push_back(temp);
    for (int j = 0; j < _cols; j++) {
      _container[i][j] = other._container[i][j];
    }
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
float const *const &Matrix::operator[](int i) {
  if (i > _rows)
    throw std::invalid_argument("Index out of bounds!");
  else if (i < 0)
    throw std::invalid_argument("Index out of bounds!");
  return _container[i];
}
float const *const &Matrix::operator[](int i) const {
  // vector returned to caller cannot be modified by caller.
  if (i > _rows)
    throw std::invalid_argument("Index out of bounds!");
  else if (i < 0)
    throw std::invalid_argument("Index out of bounds!");

  return _container[i];
}
void Matrix::set_value(int r, int c, float val) { _container[r][c] = val; }
float &Matrix::get_value(int r, int c) { return _container[r][c]; }
void Matrix::set_value(int r, float *row) {
  // row should be a float array instantiated on heap.
  delete _container[r];
  _container[r] = row;
}
Matrix &Matrix::operator=(const Matrix &other) {
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
const std::vector<float> Matrix::getColumn(int j) const {
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
Matrix operator+(const Matrix &a, const Matrix &b) {
  // Element-wise add two matrices.
  if (a.shape[0] != b.shape[0] || a.shape[1] != b.shape[1]) {
    throw std::invalid_argument("Shape mismatch between left and right Matrices");
  }
  if (a.shape[0] == 0 && a.shape[1] == 0) {
    return Matrix{0, 0}; // empty matrix
  }
  Matrix out{a.shape[0], a.shape[1]};
  std::vector<Status<float> *> items;
  for (int i = 0; i < a.shape[0]; i++) {
    Status<float> *temp = new Status<float>(out._worker.SubmitTask(&arr_op_sum, a[i], b[i], a.shape[1], b.shape[1]));
    items.push_back(temp);
  }
  int i = 0;
  for (auto const &ele : items) {
    std::future<float *> temp = ele->GetFuturePtr();
    temp.wait();
    out.set_value(i, temp.get());
    i++;
    delete ele;
  }
  items.clear();
  return std::move(out);
}
Matrix operator-(const Matrix &a, const Matrix &b) {
  // Element-wise subtract two matrices.
  if (a.shape[0] != b.shape[0] || a.shape[1] != b.shape[1]) {
    throw std::invalid_argument("Shape mismatch between left and right Matrices");
  }
  if (a.shape[0] == 0 && a.shape[1] == 0) {
    return Matrix{0, 0}; // empty matrix
  }
  Matrix out{a.shape[0], a.shape[1]};
  std::list<Status<float> *> items;
  for (int i = 0; i < a.shape[0]; i++) {
    Status<float> *temp = new Status<float>(out._worker.SubmitTask(&arr_op_sub, a[i], b[i], a.shape[1], b.shape[1]));
    items.push_back(temp);
  }
  int i = 0;
  for (auto const &ele : items) {
    std::future<float *> temp = ele->GetFuturePtr();
    temp.wait();
    out.set_value(i, temp.get());
    i++;
    delete ele;
  }
  items.clear();
  return std::move(out);
}

Matrix operator*(const Matrix &a, const Matrix &b) {
  /* Dot product. This is not element-wise multiplication.
   * It is possible to return a 1x1 Matrix. Dot product of 2 vectors
   * represented as matrices will return 1x1 matrix that should be a scalar.
   *
   * */
  if (a.shape[1] != b.shape[0]) {
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
  const Matrix int_mat = b.transpose();
  std::list<Status<float> *> items;
  for (int i = 0; i < l_rows; i++) {
    for (int j = 0; j < r_cols; j++) {
      Status<float> *temp = new Status<float>(a._worker.SubmitTask(&arr_op_dot_prod, a[i], int_mat[j], l_cols, r_rows));
      items.push_back(temp);
    }
  }
  for (int i = 0; i < l_rows; i++) {
    for (int j = 0; j < r_cols; j++) {
      std::future<float *> temp = items.front()->GetFuturePtr(); // pop_front is constant time.
      temp.wait();
      out.set_value(i, j, (*temp.get()));
      delete items.front();
      items.pop_front();
    }
  }
  for (auto const &ele : items) {
    delete ele;
  }
  items.clear();
  return std::move(out);
}

Matrix operator*(const float &a, const Matrix &b) {
  // Multiply the matrice by a scalar a.
  if (b.shape[0] == 0 && b.shape[1] == 0) {
    return Matrix{0, 0}; // empty matrix
  }

  Matrix out{b.shape[0], b.shape[1]};
  int r_rows = b.shape[0];
  int r_cols = b.shape[1];

  std::vector<Status<float> *> items;
  for (int i = 0; i < r_rows; i++) {
    Status<float> *temp = new Status<float>(b._worker.SubmitTask(&arr_op_mul_c, b[i], b.shape[1], a));
    items.push_back(temp);
  }
  int i = 0;
  for (auto const &ele : items) {
    std::future<float *> temp = ele->GetFuturePtr();
    temp.wait();
    out.set_value(i, temp.get());
    i++;
    delete ele;
  }
  items.clear();
  return std::move(out);
}
Matrix operator*(const int &a, const Matrix &b) {
  // Multiply the matrice by a scalar a.
  if (b.shape[0] == 0 && b.shape[1] == 0) {
    return Matrix{0, 0}; // empty matrix
  }

  Matrix out{b.shape[0], b.shape[1]};
  int r_rows = b.shape[0];
  int r_cols = b.shape[1];

  for (int i = 0; i < r_rows; i++) {
    for (int j = 0; j < r_cols; j++) {
      out.set_value(i, j, (a * b[i][j]));
    }
  }

  return std::move(out);
}
Matrix operator*(const Matrix &b, const int &a) { return a * b; }
Matrix operator*(const Matrix &b, const float &a) { return a * b; }
bool operator==(const Matrix &a, const Matrix &b) {
  // TODO: Floating point comparison is slow.
  if (a.shape[0] != b.shape[0] && a.shape[1] != b.shape[1]) {
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
bool operator!=(const Matrix &a, const Matrix &b) {
  // TODO: Floating point comparison is slow.
  if (a.shape[0] != b.shape[0] && a.shape[1] != b.shape[1]) {
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

Matrix Matrix::operator+=(const Matrix &other) {
  if ((*this).shape[0] != other.shape[0] || (*this).shape[1] != other.shape[1]) {
    throw std::invalid_argument("Shape mismatch between left and right Matrices");
  }
  if ((*this).shape[0] == 0 && (*this).shape[1] == 0) {
    return *this;
  }
  std::vector<Status<float> *> items;
  for (int i = 0; i < (*this).shape[0]; i++) {
    Status<float> *temp = new Status<float>((*this)._worker.SubmitTask(&arr_op_sum, (*this)[i], other[i], (*this).shape[1], other.shape[1]));
    items.push_back(temp);
  }
  int i = 0;
  for (auto const &ele : items) {
    std::future<float *> temp = ele->GetFuturePtr();
    temp.wait();
    (*this).set_value(i, temp.get());
    i++;
    delete ele;
  }
  items.clear();
  return std::move(*this);
}
Matrix Matrix::operator-=(const Matrix &other) {
  if ((*this).shape[0] != other.shape[0] || (*this).shape[1] != other.shape[1]) {
    throw std::invalid_argument("Shape mismatch between left and right Matrices");
  }
  if ((*this).shape[0] == 0 && (*this).shape[1] == 0) {
    return *this;
  }
  std::list<Status<float> *> items;
  for (int i = 0; i < (*this).shape[0]; i++) {
    Status<float> *temp = new Status<float>((*this)._worker.SubmitTask(&arr_op_sub, (*this)[i], other[i], (*this).shape[1], other.shape[1]));
    items.push_back(temp);
  }
  int i = 0;
  for (auto const &ele : items) {
    std::future<float *> temp = ele->GetFuturePtr();
    temp.wait();
    (*this).set_value(i, temp.get());
    i++;
    delete ele;
  }
  items.clear();
  return std::move(*this);
}
Matrix Matrix::transpose() const {
  Matrix out(_cols, _rows);
  for (int i = 0; i < (*this)._rows; i++) {
    for (int j = 0; j < (*this)._cols; j++) {
      out.set_value(j, i, _container[i][j]);
    }
  }
  return std::move(out);
}
Matrix Matrix::transpose() {
  Matrix out(_cols, _rows);
  for (int i = 0; i < (*this)._rows; i++) {
    for (int j = 0; j < (*this)._cols; j++) {
      out.set_value(j, i, _container[i][j]);
    }
  }
  return std::move(out);
}

void Matrix::print() {
  for (int i = 0; i < _rows; i++) {
    std::cout << '\n' << "___________" << '\n' << "| ";
    for (int j = 0; j < _cols; j++) {
      std::cout << (*this)[i][j] << " | ";
    }
  }
  std::cout << '\n' << "___________" << '\n';
}
