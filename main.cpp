#include <iostream>
#include <filesystem>

#include "platform.h"
#include "helpers/string_helpers.h"
#include "ZDPlasKinWrapper.h"
#include "ZDPlasKinParams.h"
#include "examples/example1.h"

#ifdef WINDOWS
#include <windows/WindowsLoader.h>
using PlatformLoader = WindowsLoader;

#endif

#ifdef UNIX

#include "unix/UnixLoader.h"
using PlatformLoader = UnixLoader;

#endif

// TODO: handle console in/output on errors with preprocessor/ZDPlaskin/...
// TODO: create python module
// TODO: 'cross-platformize' everything
// - UnixLoader
// - Check if compilation works on UNIX systems
// - Unix preprocessing
// TODO: documentation
// TODO: build/install script
// - gfortran compiler is a requirement
// TODO: recreate ZDPlasKin examples in cpp/python

int main() {
	std::string inFile = "kinet.inp";
//	ZDPlasKinCompiler::preprocess(inFile);
//	ZDPlasKinCompiler::compile();
	std::string srcPath = std::filesystem::canonical(inFile).string();
	utils::removeSubstr(srcPath, inFile);
	std::string path = srcPath + "zdplaskin.dll";
	PlatformLoader loader(path);
	ZDPlasKinParams parameters(srcPath + "zdplaskin_m.F90");
	ZDPlasKinWrapper zdplaskin{&loader, &parameters};

	try {
		loader.init();
		parameters.readParams();

		example1(zdplaskin);
	}
	catch (const ZDPlaskinException &e) {
		std::cerr << "Exception occurred" << std::endl;
		std::cerr << e.what() << std::endl;
		return 1;
	}
	return 0;
}
