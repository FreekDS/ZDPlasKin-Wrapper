#include <iostream>
#include "platform.h"
#include "ZDPlasKinFunctionDefs.h"

#ifdef WINDOWS

#include <windows/WindowsLoader.h>
using PlatformLoader = WindowsLoader;

#endif

#ifdef UNIX

#include "unix/UnixLoader.h"
using PlatformLoader = UnixLoader;

#endif

using namespace std;


using ADD_FT_old = int (*)(int &, int &);
using ADD_FT = int (*)(int &a, int &b, int &result);
using WRITE_FT = void (*)();
using PRINT_FT = void (*)(const char *, size_t size);

/**
 * 1. dynamisch compilen/laden fortran module
 * 		1.1 Uitvoeren van preprocessing executable (OS dependent)
 * 		1.2 Compilatie van fortran library
 * 		1.3 Dynamisch verbinden van fortran lib at runtime (zie huidige code)
 * 2. nodige functie bindings maken (fortran - cpp)
 * 3. python module maken die bovenstaande gebruikt (cpp - python)
 *
 *
 *  4. Cross platformizeren
 */

int main() {
	ILibraryLoader *loader;
	std::string path = R"(C:\Users\gebruiker\CLionProjects\ZDPlaskin\cmake-build-debug\fortran\libfortran_lib.dll)";
	try {
		loader = new PlatformLoader(path);
		loader->init();

		auto add_f = loader->getFunction<PRINT_FT>("print");
		int a = 1, b = 3, result;
		const std::string str = "World hello?";
		add_f(str.c_str(), str.size());
		cout << result;
		delete loader;
		loader = nullptr;
	}
	catch (const std::exception &e) {
		delete loader;
		loader = nullptr;
		return -1;
	}

	return 0;
}
