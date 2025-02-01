#include "typing/DTypes.h"
#include "typing/Matrix.h"
#include <Python.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <structmember.h>

#define PY_SSIZE_T_CLEAN

extern "C" {
enum MatrixType { BOOL = 0, INT = 1, FLOAT = 2 };
static PyObject *CallError;
struct CpyMatrix {
  PyObject_HEAD; // Compiler may complain about semicolon
  void *obj;
  int row;
  int col;
  float def_val;
  enum MatrixType type;
  char *str_type;
};
static void cpymatrix_dealloc(struct CpyMatrix *self) { Py_TYPE(self)->tp_free((PyObject *)self); }
static int cpymatrix_init(CpyMatrix *self, PyObject *args, PyObject *kwds) {
  char *kwlist[] = {(char *)"row", (char *)"col", (char *)"def_val", (char *)"type",
                    NULL}; // Array of char pointers; cast to char * for c style string literal to c string conversion
  if (!PyArg_ParseTupleAndKeywords(args, kwds, "iifs", kwlist, &self->row, &self->col, &self->def_val, &self->str_type)) {
    return -1;
  }
  if (strcmp(self->str_type, (char *)"float") == 0) {
    self->obj = (void *)new Matrix<float>(self->row, self->col, self->def_val);
    self->type = FLOAT;
  } else if (strcmp(self->str_type, (char *)"int") == 0) {
    self->obj = (void *)new Matrix<int>(self->row, self->col, self->def_val);
    self->type = INT;
  } else if (strcmp(self->str_type, (char *)"bool") == 0) {
    self->obj = (void *)new Matrix<bool>(self->row, self->col, self->def_val);
    self->type = BOOL;
  } else {
    return -1; // Return error
  }

  return 0;
}
static PyMemberDef cpymatrix_attributes[] = {
    {"row", T_INT, offsetof(CpyMatrix, row), 0, "Number of rows"},
    {"col", T_INT, offsetof(CpyMatrix, col), 0, "Number of columns"},
    {"d_type", T_STRING, offsetof(CpyMatrix, str_type), 0, "Data type"},
    {NULL} // Sentinel
};
static PyObject *cpymatrix_print(struct CpyMatrix *self, PyObject *Py_UNUSED(ignored)) {
  switch (self->type) {
  case BOOL:
    ((Matrix<bool> *)self->obj)->print();
    break;
  case INT:
    ((Matrix<int> *)self->obj)->print();
    break;
  case FLOAT:
    ((Matrix<float> *)self->obj)->print();
    break;
  }
  Py_IncRef(Py_None);
  return Py_None;
}
static PyObject *cpymatrix_set_value(struct CpyMatrix *self, PyObject *args, PyObject *kwds) {
  char *kwlist[] = {(char *)"row", (char *)"col", (char *)"value",
                    NULL}; // Array of char pointers; cast to char * for c style string literal to c string conversion
  int row;
  int col;

  switch (self->type) {
  case BOOL:
    try {
      bool val;
      if (!PyArg_ParseTupleAndKeywords(args, kwds, "iip", kwlist, &row, &col, &val)) {
        return NULL;
      }
      ((Matrix<bool> *)self->obj)->set_value(row, col, val);
    } catch (std::invalid_argument) {
      PyErr_SetString(CallError, "CpyMatrix.set_value call failed");
      return NULL;
    };
    break;
  case INT:
    try {
      int val;
      if (!PyArg_ParseTupleAndKeywords(args, kwds, "iii", kwlist, &row, &col, &val)) {
        return NULL;
      }
      ((Matrix<int> *)self->obj)->set_value(row, col, val);
    } catch (std::invalid_argument) {
      PyErr_SetString(CallError, "CpyMatrix.set_value call failed");
      return NULL;
    }
    break;
  case FLOAT:
    try {
      float val;
      if (!PyArg_ParseTupleAndKeywords(args, kwds, "iif", kwlist, &row, &col, &val)) {
        return NULL;
      }
      ((Matrix<float> *)self->obj)->set_value(row, col, val);
    } catch (std::invalid_argument) {
      PyErr_SetString(CallError, "CpyMatrix.set_value call failed");
      return NULL;
    }
    break;
  }

  Py_IncRef(Py_None);
  return Py_None;
}
static PyObject *cpymatrix_get_value(struct CpyMatrix *self, PyObject *args, PyObject *kwds) {
  char *kwlist[] = {(char *)"row", (char *)"col", NULL};
  int row;
  int col;
  if (!PyArg_ParseTupleAndKeywords(args, kwds, "ii", kwlist, &row, &col)) {
    return NULL;
  }
  // TODO: Switch over to operator()
  switch (self->type) {
  case BOOL:
    bool out;
    try {
      out = ((Matrix<bool> *)self->obj)->get_value(row, col);
      return PyBool_FromLong(out); // casts bool to long
    } catch (std::invalid_argument) {
      PyErr_SetString(CallError, "CpyMatrix.get_value call failed");
      return NULL;
    }
  case INT:
    try {
      int out;
      out = ((Matrix<int> *)self->obj)->get_value(row, col);
      return PyLong_FromLong(out);
    } catch (std::invalid_argument) {
      PyErr_SetString(CallError, "CpyMatrix.get_value call failed");
      return NULL;
    }
  case FLOAT:
    try {
      float out;
      out = ((Matrix<float> *)self->obj)->get_value(row, col);
      return PyFloat_FromDouble(out);
    } catch (std::invalid_argument) {
      PyErr_SetString(CallError, "CpyMatrix.get_value call failed");
      return NULL;
    }
  }
}
static PyObject *cpymatrix_add(struct CpyMatrix *self, PyObject *args, PyObject *kwds) {
  char *kwlist[] = {(char *)"other", NULL};
  struct CpyMatrix *right = NULL;
  if (!PyArg_ParseTupleAndKeywords(args, kwds, "O", kwlist, &right)) {
    Py_INCREF(Py_None);
    return Py_None;
  }
  struct CpyMatrix *out = (struct CpyMatrix *)PyObject_New(struct CpyMatrix, Py_TYPE(self));
  switch (self->type) {
  case BOOL:
    try {
      Matrix<bool> temp = (*(Matrix<bool> *)self->obj) + (*(Matrix<bool> *)right->obj);
      if (out == NULL) {
        return NULL;
      }
      Matrix<bool> *temp2 = new Matrix<bool>(temp); // Copy
      out->obj = temp2;
      out->str_type = (char *)"bool";
      out->type = FLOAT;
      out->row = self->row;
      out->col = self->col;
      return (PyObject *)out;
    } catch (std::invalid_argument &e) {
      std::cout << "ERROR: " << e.what() << std::endl;
      PyErr_SetString(CallError, "CpyMatrix.add call failed");
      return NULL;
    }
  case INT:
    try {
      Matrix<int> temp = (*(Matrix<int> *)self->obj) + (*(Matrix<int> *)right->obj);
      if (out == NULL) {
        return NULL;
      }
      Matrix<int> *temp2 = new Matrix<int>(temp); // Copy
      out->obj = temp2;
      out->str_type = (char *)"int";
      out->type = FLOAT;
      out->row = self->row;
      out->col = self->col;
      return (PyObject *)out;

    } catch (std::invalid_argument &e) {
      std::cout << "ERROR: " << e.what() << std::endl;
      PyErr_SetString(CallError, "CpyMatrix.add call failed");
      return NULL;
    }
  case FLOAT:
    try {
      Matrix<float> temp = (*(Matrix<float> *)self->obj) + (*(Matrix<float> *)right->obj);
      if (out == NULL) {
        return NULL;
      }
      Matrix<float> *temp2 = new Matrix<float>(temp); // Copy
      out->obj = temp2;
      out->str_type = (char *)"float";
      out->type = FLOAT;
      out->row = self->row;
      out->col = self->col;
      return (PyObject *)out;
    } catch (std::invalid_argument &e) {
      std::cout << "ERROR: " << e.what() << std::endl;
      PyErr_SetString(CallError, "CpyMatrix.add call failed");
      return NULL;
    }
  }
  Py_INCREF(Py_None);
  return Py_None;
}
static PyObject *cpymatrix_sub(struct CpyMatrix *self, PyObject *args, PyObject *kwds) {
  char *kwlist[] = {(char *)"other", NULL};
  CpyMatrix *right;
  if (!PyArg_ParseTupleAndKeywords(args, kwds, "O", kwlist, &right)) {
    Py_INCREF(Py_None);
    return Py_None;
  }
  struct CpyMatrix *out = (struct CpyMatrix *)PyObject_New(struct CpyMatrix, Py_TYPE(self));
  switch (self->type) {
  case BOOL:
    try {
      Matrix<bool> temp = (*(Matrix<bool> *)self->obj) - (*(Matrix<bool> *)right->obj);
      if (out == NULL) {
        return NULL;
      }
      Matrix<bool> *temp2 = new Matrix<bool>(temp); // Copy
      out->obj = temp2;
      out->str_type = (char *)"bool";
      out->type = FLOAT;
      out->row = self->row;
      out->col = self->col;
      return (PyObject *)out;
    } catch (std::invalid_argument &e) {
      std::cout << "ERROR: " << e.what() << std::endl;
      PyErr_SetString(CallError, "CpyMatrix.add call failed");
      return NULL;
    }
  case INT:
    try {
      Matrix<int> temp = (*(Matrix<int> *)self->obj) - (*(Matrix<int> *)right->obj);
      if (out == NULL) {
        return NULL;
      }
      Matrix<int> *temp2 = new Matrix<int>(temp); // Copy
      out->obj = temp2;
      out->str_type = (char *)"int";
      out->type = FLOAT;
      out->row = self->row;
      out->col = self->col;
      return (PyObject *)out;

    } catch (std::invalid_argument &e) {
      std::cout << "ERROR: " << e.what() << std::endl;
      PyErr_SetString(CallError, "CpyMatrix.add call failed");
      return NULL;
    }
  case FLOAT:
    try {
      Matrix<float> temp = (*(Matrix<float> *)self->obj) - (*(Matrix<float> *)right->obj);
      if (out == NULL) {
        return NULL;
      }
      Matrix<float> *temp2 = new Matrix<float>(temp); // Copy
      out->obj = temp2;
      out->str_type = (char *)"float";
      out->type = FLOAT;
      out->row = self->row;
      out->col = self->col;
      return (PyObject *)out;
    } catch (std::invalid_argument &e) {
      std::cout << "ERROR: " << e.what() << std::endl;
      PyErr_SetString(CallError, "CpyMatrix.add call failed");
      return NULL;
    }
  }
  Py_INCREF(Py_None);
  return Py_None;
}
static PyObject *cpymatrix_transpose(struct CpyMatrix *self, PyObject *Py_UNUSED(ignored)) {
  struct CpyMatrix *out = (struct CpyMatrix *)PyObject_New(struct CpyMatrix, Py_TYPE(self));
  switch (self->type) {
  case BOOL: {
    Matrix<bool> temp = ((Matrix<bool> *)self->obj)->transpose();
    Matrix<bool> *temp2 = new Matrix<bool>(temp); // Copy and move to heap
    out->obj = temp2;
    out->str_type = (char *)"bool";
    out->type = FLOAT;
    out->row = self->row;
    out->col = self->col;
    return (PyObject *)out;
  }
  case INT: {
    Matrix<int> temp = ((Matrix<int> *)self->obj)->transpose();
    Matrix<int> *temp2 = new Matrix<int>(temp); // Copy
    out->obj = temp2;
    out->str_type = (char *)"int";
    out->type = FLOAT;
    out->row = self->row;
    out->col = self->col;
    return (PyObject *)out;
  }
  case FLOAT: {
    Matrix<float> temp = ((Matrix<float> *)self->obj)->transpose();
    Matrix<float> *temp2 = new Matrix<float>(temp); // Copy
    out->obj = temp2;
    out->str_type = (char *)"float";
    out->type = FLOAT;
    out->row = self->row;
    out->col = self->col;
    return (PyObject *)out;
  }
  }
  Py_IncRef(Py_None);
  return Py_None;
}
static PyObject *cpymatrix_mul(struct CpyMatrix *self, PyObject *args, PyObject *kwds) {
  char *kwlist[] = {(char *)"other", NULL};
  CpyMatrix *right;
  if (!PyArg_ParseTupleAndKeywords(args, kwds, "O", kwlist, &right)) {
    Py_INCREF(Py_None);
    return Py_None;
  }
  struct CpyMatrix *out = (struct CpyMatrix *)PyObject_New(struct CpyMatrix, Py_TYPE(self));
  if (out == NULL) {
    return NULL;
  }

  switch (self->type) {
  case BOOL:
    try {
      Matrix<bool> temp = (*(Matrix<bool> *)self->obj) * (*(Matrix<bool> *)right->obj);
      Matrix<bool> *temp2 = new Matrix<bool>(temp); // Copy
      out->obj = temp2;
      out->str_type = (char *)"bool";
      out->type = FLOAT;
      out->row = self->row;
      out->col = self->col;
      return (PyObject *)out;
    } catch (std::invalid_argument &e) {
      std::cout << "ERROR: " << e.what() << std::endl;
      PyErr_SetString(CallError, "CpyMatrix.add call failed");
      return NULL;
    }
  case INT:
    try {
      Matrix<int> temp = (*(Matrix<int> *)self->obj) * (*(Matrix<int> *)right->obj);
      Matrix<int> *temp2 = new Matrix<int>(temp); // Copy
      out->obj = temp2;
      out->str_type = (char *)"int";
      out->type = FLOAT;
      out->row = self->row;
      out->col = self->col;
      return (PyObject *)out;

    } catch (std::invalid_argument &e) {
      std::cout << "ERROR: " << e.what() << std::endl;
      PyErr_SetString(CallError, "CpyMatrix.add call failed");
      return NULL;
    }
  case FLOAT:
    try {
      Matrix<float> temp = (*(Matrix<float> *)self->obj) * (*(Matrix<float> *)right->obj);
      Matrix<float> *temp2 = new Matrix<float>(temp); // Copy
      out->obj = temp2;
      out->str_type = (char *)"float";
      out->type = FLOAT;
      out->row = self->row;
      out->col = self->col;
      return (PyObject *)out;
    } catch (std::invalid_argument &e) {
      std::cout << "ERROR: " << e.what() << std::endl;
      PyErr_SetString(CallError, "CpyMatrix.add call failed");
      return NULL;
    }
  }
  Py_INCREF(Py_None);
  return Py_None;
}
static PyObject *cpymatrix_ndim(struct CpyMatrix *self, PyObject *Py_UNUSED(ignored)) {
  switch (self->type) {
  case BOOL:
    return PyLong_FromLong(((Matrix<bool> *)self->obj)->ndim());
  case INT:
    return PyLong_FromLong(((Matrix<int> *)self->obj)->ndim());
  case FLOAT:
    return PyLong_FromLong(((Matrix<float> *)self->obj)->ndim());
  }
  Py_INCREF(Py_None);
  return Py_None;
}
static PyObject *cpymatrix_nbytes(struct CpyMatrix *self, PyObject *Py_UNUSED(ignored)) {
  switch (self->type) {
  case BOOL:
    return PyLong_FromLong(((Matrix<bool> *)self->obj)->nbytes());
  case INT:
    return PyLong_FromLong(((Matrix<int> *)self->obj)->nbytes());
  case FLOAT:
    return PyLong_FromLong(((Matrix<float> *)self->obj)->nbytes());
  }
  Py_INCREF(Py_None);
  return Py_None;
}
static PyObject *cpymatrix_size(struct CpyMatrix *self, PyObject *Py_UNUSED(ignored)) {
  switch (self->type) {
  case BOOL:
    return PyLong_FromLong(((Matrix<bool> *)self->obj)->size());
  case INT:
    return PyLong_FromLong(((Matrix<int> *)self->obj)->size());
  case FLOAT:
    return PyLong_FromLong(((Matrix<float> *)self->obj)->size());
  }
  Py_INCREF(Py_None);
  return Py_None;
}
static PyObject *cpymatrix_itemsize(struct CpyMatrix *self, PyObject *Py_UNUSED(ignored)) {
  switch (self->type) {
  case BOOL:
    return PyLong_FromLong(((Matrix<bool> *)self->obj)->itemsize());
  case INT:
    return PyLong_FromLong(((Matrix<int> *)self->obj)->itemsize());
  case FLOAT:
    return PyLong_FromLong(((Matrix<float> *)self->obj)->itemsize());
  }
  Py_INCREF(Py_None);
  return Py_None;
}
static PyObject *cpymatrix_flatten(struct CpyMatrix *self, PyObject *Py_UNUSED(ignored)) {
  switch (self->type) {
  case BOOL:
    ((Matrix<bool> *)self->obj)->flatten();
  case INT:
    ((Matrix<int> *)self->obj)->flatten();
  case FLOAT:
    ((Matrix<float> *)self->obj)->flatten();
  }
  Py_IncRef(Py_None);
  return Py_None;
}
static PyMethodDef cpymatrix_methods[] = {
    {"print", (PyCFunction)cpymatrix_print, METH_NOARGS, "Print the Matrix"},
    {"set_value", (PyCFunction)cpymatrix_set_value, METH_VARARGS | METH_KEYWORDS, "Set value at an index"},
    {"get_value", (PyCFunction)cpymatrix_get_value, METH_VARARGS | METH_KEYWORDS, "Get value at an index"},
    {"add", (PyCFunction)cpymatrix_add, METH_VARARGS | METH_KEYWORDS, "Add two matrices"},
    {"sub", (PyCFunction)cpymatrix_sub, METH_VARARGS | METH_KEYWORDS, "Subtract two matrices"},
    {"mult", (PyCFunction)cpymatrix_mul, METH_VARARGS | METH_KEYWORDS, "Multiply two matrices"},
    {"transpose", (PyCFunction)cpymatrix_transpose, METH_VARARGS | METH_KEYWORDS, "Transpose the matrix"},
    {"ndim", (PyCFunction)cpymatrix_ndim, METH_NOARGS, "Get matrix dimensions"},
    {"nbytes", (PyCFunction)cpymatrix_nbytes, METH_NOARGS, "Return size of matrix"},
    {"itemsize", (PyCFunction)cpymatrix_itemsize, METH_NOARGS, "Return size of each element"},
    {"size", (PyCFunction)cpymatrix_size, METH_NOARGS, "Return number of elements in matrix"},
    {"flatten", (PyCFunction)cpymatrix_flatten, METH_NOARGS, "Flatten the array"},
    {NULL} // Sentinel
};
static PyTypeObject CpyMatrixType = {
    // Maybe an issue.
    PyVarObject_HEAD_INIT(NULL, 0).tp_name = "matrix.Matrix",
    .tp_basicsize = sizeof(CpyMatrix),
    .tp_itemsize = 0,
    .tp_dealloc = (destructor)cpymatrix_dealloc,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_doc = PyDoc_STR("Matrix objects"),
    .tp_methods = cpymatrix_methods,
    .tp_members = cpymatrix_attributes,
    .tp_init = (initproc)cpymatrix_init,
    .tp_new = PyType_GenericNew,
};

const char *get_zetacore_docs(void) {
  const char docs = 'w';
  const char *spam_doc = &docs;
  return spam_doc;
}

static struct PyModuleDef zetacore_module = {
    PyModuleDef_HEAD_INIT, "zetacore", /* __name__ of module */
    get_zetacore_docs(),               /* module documentation, may be NULL */
    -1,                                /* size of per-interpreter state of the module, or -1 if the module keeps state in global variables. */
};

PyMODINIT_FUNC PyInit_zetacore(void) {
  PyObject *m;
  if (PyType_Ready(&CpyMatrixType) < 0)
    return NULL;

  m = PyModule_Create(&zetacore_module);
  if (m == NULL)
    return NULL;

  Py_INCREF(&CpyMatrixType);
  if (PyModule_AddObject(m, "CpyMatrix", (PyObject *)&CpyMatrixType) < 0) {
    Py_DECREF(&CpyMatrixType);
    Py_DECREF(m);
    return NULL;
  }
  CallError = PyErr_NewException("call.error", NULL, NULL);
  if (PyModule_AddObject(m, "error", CallError) < 0) {
    Py_CLEAR(CallError);
    Py_DECREF(m);
    return NULL;
  }

  return m;
}
}
