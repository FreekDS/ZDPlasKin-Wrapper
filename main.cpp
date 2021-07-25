#include <iostream>
#include <filesystem>

#include "platform.h"
#include "helpers/string_helpers.h"
#include "ZDPlasKinWrapper.h"
#include "ZDPlasKinParams.h"
#include "ZDPlasKinCompiler.h"

#ifdef WINDOWS
#include <windows/WindowsLoader.h>
using PlatformLoader = WindowsLoader;

#endif

#ifdef UNIX

#include "unix/UnixLoader.h"
using PlatformLoader = UnixLoader;

#endif

/**
 * 3. python module maken die bovenstaande gebruikt (cpp - python)
 * 4. Cross platformizeren
 */

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

		// Test method to see if linked correctly
		zdplaskin.init("bolsigdb.dat", true);
	}
	catch (const ZDPlaskinException &e) {
		std::cerr << "Exception occurred" << std::endl;
		std::cerr << e.what() << std::endl;
		return 1;
	}
	return 0;
}
