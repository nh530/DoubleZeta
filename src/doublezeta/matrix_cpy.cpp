// #include "typing/DTypes.h"
// #include "typing/Matrix.h"
// #include <Python.h>
// #include <string.h>
// #include <structmember.h>
#include <Python.h>
#include <stdarg.h>
#include <stdio.h>
#define PY_SSIZE_T_CLEAN

// struct CpyMatrix {
//   PyObject_HEAD; // Compiler may complain about semicolon
//   // Matrix<float> *float_obj;
//   // Matrix<int> *int_obj;
//   // Matrix<bool> *bool_obj;
//   int row;
//   int col;
//   float def_val;
// };
// static void cpymatrix_dealloc(struct CpyMatrix *self) {}
// static PyObject *cpymatrix_new(PyTypeObject *type, PyObject *args, PyObject *kwds) {
//   CpyMatrix *self;
//   self = (CpyMatrix *)type->tp_alloc(type, 0);
//   if (self != NULL) {
//     self->row = -1;
//     self->col = -1;
//     self->def_val = 0;
//   }
//   return (PyObject *)self;
// }
// static int cpymatrix_init(CpyMatrix *self, PyObject *args, PyObject *kwds) {
//   char *kwlist[] = {(char *)"row", (char *)"col", (char *)"def_val",
//                     NULL}; // Array of char pointers; cast to char * for c style string literal to c string conversion
//   if (!PyArg_ParseTupleAndKeywords(args, kwds, "iif", kwlist)) {
//     return -1;
//   }
//   return 0;
// }
// static PyMemberDef cpymatrix_attributes[] = {
//     {"row", T_OBJECT_EX, offsetof(CpyMatrix, row), 0, "Number of rows"},
//     {"col", T_OBJECT_EX, offsetof(CpyMatrix, col), 0, "Number of columns"},
//     {"def_val", T_OBJECT_EX, offsetof(CpyMatrix, def_val), 0, "Default Value"},
//     {NULL} // Sentinel
// };
// static PyObject *cpymatrix_print(struct CpyMatrix *self, PyObject *Py_UNUSED(ignored)) {}
// static PyMethodDef cpymatrix_methods[] = {
//     {"print", (PyCFunction)cpymatrix_print, METH_NOARGS, "Print the Matrix"}, {NULL} // Sentinel
// };
// static PyTypeObject CpyMatrixType = {
//     // Maybe an issue.
//     PyVarObject_HEAD_INIT(NULL, 0).tp_name = "matrix.Matrix",
//     .tp_basicsize = sizeof(CpyMatrix),
//     .tp_itemsize = 0,
//     .tp_dealloc = (destructor)cpymatrix_dealloc,
//     .tp_flags = Py_TPFLAGS_DEFAULT,
//     .tp_doc = PyDoc_STR("Matrix objects"),
//     .tp_methods = cpymatrix_methods,
//     .tp_members = cpymatrix_attributes,
//     .tp_init = (initproc)cpymatrix_init,
//     .tp_new = PyType_GenericNew,
// };
// static PyModuleDef cpymatrix_module = {
//     PyModuleDef_HEAD_INIT,
//     .m_name = "doublezeta",
//     .m_doc = "Matrix object module",
//     .m_size = -1,
// };
// PyMODINIT_FUNC PyInit_doublezeta(void) {
//   PyObject *m;
//   // if (PyType_Ready(&CpyMatrixType) < 0)
//   //   return NULL;
//
//   m = PyModule_Create(&cpymatrix_module);
//   if (m == NULL)
//     return NULL;
//
//   // Py_INCREF(&CpyMatrixType);
//   // if (PyModule_AddObject(m, "CpyMatrix", (PyObject *)&CpyMatrixType) < 0) {
//   //   Py_DECREF(&CpyMatrixType);
//   //   Py_DECREF(m);
//   //   return NULL;
//   // }
//   return m;
// }

extern "C" {
static PyObject *spam_system(PyObject *self, PyObject *args) {
  const char *command;
  int sts;

  if (!PyArg_ParseTuple(args, "s", &command))
    return NULL;
  sts = system(command);
  return PyLong_FromLong(sts);
}

static PyMethodDef SpamMethods[] = {
    {"system", spam_system, METH_VARARGS, "Execute a shell command."}, {NULL, NULL, 0, NULL} /* Sentinel */
};

const char *get_spam_docs(void) {
  const char docs = 'w';
  const char *spam_doc = &docs;
  return spam_doc;
}

static struct PyModuleDef doublezetamodule = {
    PyModuleDef_HEAD_INIT, "spam", /* name of module */
    get_spam_docs(),                     /* module documentation, may be NULL */
    -1,                                  /* size of per-interpreter state of the module, or -1 if the module keeps state in global variables. */
    SpamMethods};

PyMODINIT_FUNC PyInit_zetacore(void) { return PyModule_Create(&doublezetamodule); }
}
