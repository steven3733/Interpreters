// Your C file to be called from python
// To compile :
// gcc -dynamiclib -I/usr/include/python2.7/ -lpython2.7 -o Module.dylib Module.c

#include < Python.h >

static PyObject* py_myFunction(PyObject* self, PyObject* args)
{
  char *s = "Hello from C!";
  return Py_BuildValue("s", s);
}

static PyMethodDef myModule_methods[] = {
  {"myFunction", py_myFunction, METH_VARAGS},
  {NULL, NULL}
};

void initmyModule()
{
  (void) Py_InitModule("myModule", myModule_methods);
}