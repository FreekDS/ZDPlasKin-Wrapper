#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <ZDPlasKinWrapper.h>
#include <ZDPlasKinCompiler.h>
#include <platform.h>
#include <iostream>

#ifdef WINDOWS
#include <windows/WindowsLoader.h>
using PlatformLoader = WindowsLoader;

#endif

#ifdef UNIX

#include "unix/UnixLoader.h"
using PlatformLoader = UnixLoader;

#endif

static PlatformLoader libraryLoader;
static ZDPlasKinParams params;
static ZDPlasKinWrapper zdp(&libraryLoader, &params);

PyObject *zdp_test(PyObject *, PyObject *o) {
	std::cout << "Hello World!" << std::endl;
	return Py_BuildValue("i", 21);
}

PyDoc_STRVAR(zdp_init_doc, "init(bolsigDB=\"\", updateDB=false, /)\n"
						   "--\n\n"
						   "Initializes the Bolsig module and the ZDPlasKin module.\n"
						   "\n"
						   "Parameters\n"
						   "----------\n"
						   "bolsigDB: str\n\t"
						   "Path to the bolsig database file.\n"
						   "updateDB: bool\n\t"
						   "whether or not the new bolsig file should be used");
PyObject *zdp_init(PyObject *self, PyObject *args, PyObject *kwargs) {
	// TODO Initialize everything (use setters of globals)
	// TODO preprocess and recompile

	// TODO parse args
	// TODO throw python errors

	// TODO check
	PyObject* self_path = PyModule_GetFilenameObject(self);


//	const char* testje;
//	PyArg_ParseTuple(self_path, "s", &testje);

	std::wstring ws(Py_GetProgramFullPath());
	std::string tes = std::string(ws.begin(), ws.end());
	//std::cout << PyUnicode_AsUTF8(self_path) << std::endl << tes << "ha" << std::endl;

	std::string inFile = "C:/Users/gebruiker/CLionProjects/ZDPlaskin/cmake-build-debug/kinet.inp)";

	std::cout << inFile << std::endl;

	ZDPlasKinCompiler::preprocess(inFile);
	ZDPlasKinCompiler::compile();

	std::string bolsig_filepath;
	bool updateDB = false;
	try {
		// These should not change

		libraryLoader.init("zdplaskin");
		params.setFilePath("./zdplaskin_m.F90");
		params.readParams();

		zdp.init(bolsig_filepath, updateDB);
	} catch (ZDPlaskinException &ex) {
		std::cout << ex.what() << std::endl;
	}

	Py_RETURN_NONE;
}

static PyMethodDef zdplaskin_methods[] = {
	{"test", (PyCFunction)zdp_test, METH_NOARGS, "A test method, returns 21"},
	{"init", (PyCFunction)zdp_init, METH_VARARGS | METH_KEYWORDS, zdp_init_doc},
	{nullptr, nullptr, 0, nullptr}
};

static PyModuleDef zdplaskin = {
	PyModuleDef_HEAD_INIT,
	"ZDPlasKin-Python",
	"Description of ZDPlasKin module",
	-1,
	zdplaskin_methods
};

PyMODINIT_FUNC PyInit_zdplaskin() {
	return PyModule_Create(&zdplaskin);
}