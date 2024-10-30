#include <Python.h>
#include "vm.h"

static PyObject* clox_interpret(PyObject* self, PyObject* args)
{
  const char* source;
  if (!PyArg_ParseTuple(args, "s", &source))
  {
    return NULL;
  }

  InterpretResult result = interpret(source);
  return Py_None;
}

static PyMethodDef clox_methods[] = {
    {"interpret", clox_interpret, METH_VARARGS, "Interpret a Lox source code string."},
    {NULL, NULL, 0, NULL}
};

static struct PyModuleDef cloxmodule = {
    PyModuleDef_HEAD_INIT,
    "clox",
    "A Python interface to the Lox VM.",
    -1,
    clox_methods
};

PyMODINIT_FUNC PyInit_pylox(void)
{
    initVM();
    return PyModule_Create(&cloxmodule);
}