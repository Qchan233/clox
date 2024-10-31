#include <Python.h>
#include "vm.h"
#include "compiler.h"
#include "memory.h"

static PyObject* clox_interpret(PyObject* self, PyObject* args)
{
  const char* source;
  if (!PyArg_ParseTuple(args, "s", &source))
  {
    return NULL;
  }

  InterpretResult result = interpret(source);
  Py_RETURN_NONE;
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


typedef struct {
    PyObject_HEAD
    ObjFunction* function;
} LoxFunction;

// 销毁函数
static void
LoxFunction_dealloc(LoxFunction* self) {
    // 如果需要，可以释放function内存
    if (self->function != NULL) {
        freeObject((Obj*) self->function);
        self->function = NULL;
    }
    Py_TYPE(self)->tp_free((PyObject*) self);
}

// 初始化函数
static int
LoxFunction_init(LoxFunction* self, PyObject* args, PyObject* kwds) {
    const char* source_code;
    if (!PyArg_ParseTuple(args, "s", &source_code)) {
        return -1;
    }

    // 使用compile API将字符串转换为ObjFunction*
    self->function = compile(source_code);
    if (self->function == NULL) {
        PyErr_SetString(PyExc_RuntimeError, "Failed to compile source code.");
        return -1;
    }

    return 0;
}

// 定义LoxFunction类型
static PyTypeObject LoxFunctionType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "clox.LoxFunction",
    .tp_basicsize = sizeof(LoxFunction),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_new = PyType_GenericNew,
    .tp_init = (initproc) LoxFunction_init,
    .tp_dealloc = (destructor) LoxFunction_dealloc,
};


PyMODINIT_FUNC PyInit_pylox(void)
{
    initVM();
    int result = PyType_Ready(&LoxFunctionType);
    if (result < 0)
        return NULL;

    PyObject* m = PyModule_Create(&cloxmodule);
    if (m == NULL)
        return NULL;


    Py_INCREF(&LoxFunctionType);
    if (PyModule_AddObject(m, "LoxFunction", (PyObject*) &LoxFunctionType) < 0) {
      printf("Failed to add LoxFunction to module.\n");
      Py_DECREF(&LoxFunctionType);
      Py_DECREF(m);
      return NULL;
    }

    return m;
}