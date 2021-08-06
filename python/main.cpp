#define PY_SSIZE_T_CLEAN
#include <Python.h>


PyObject* test(PyObject*, PyObject* o) {
	double x = PyFloat_AsDouble(o);
	x++;
	return PyFloat_FromDouble(x);
}


static PyMethodDef test_methods[] = {
	{"test_method", (PyCFunction)test, METH_O, nullptr},
	{nullptr, nullptr, 0, nullptr}
};

static PyModuleDef test_module = {
	PyModuleDef_HEAD_INIT,
	"test module",
	"Description of test module",
	0,
	test_methods
};

PyMODINIT_FUNC PyInit_test_module() {
	return PyModule_Create(&test_module);
}