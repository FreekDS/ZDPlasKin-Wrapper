#include <iostream>
#include <filesystem>
#include "ZDPlasKinCompiler.h"
#include "ZDPlasKinExceptions.h"

void ZDPlasKinCompiler::preprocess(const std::string &input_file) {
	// TODO: allow for communication with subprocess if required (pressing enter etc.)
	std::string command = std::string(PREP_PATH) + " " + input_file + " zdplaskin_m.F90";
	std::cout << "preprocess command: " << command << std::endl;
	int retVal = system(command.c_str());
	if (retVal == 0) {
		std::cout << "Input file preprocessed" << std::endl;
	} else
		throw CompileException("Could not preprocess the file '" + input_file + "'");
}

void ZDPlasKinCompiler::compile(const std::string &opts) {
	// TODO: extra work for cross-platform support is required here
	// TODO: create package with ar command (ar cr testlib.a *.o)
	std::string dvode = std::string(LIB_PATH) + "dvode_f90_m.F90";
	std::string ZDPlasKin = "zdplaskin_m.F90";
	std::string bolsig = std::string(LIB_PATH) + "bolsig" + std::string(BOLSIG_EXT);
	std::string defaultOpts = "-O3 -march=native -std=legacy -ffree-line-length-0 ";

	// Copy dll to executable (required in windows)
	if (!std::filesystem::exists("bolsig" + std::string(BOLSIG_EXT))) {
		std::filesystem::copy(bolsig, "./bolsig" + std::string(BOLSIG_EXT));
	}

	std::stringstream ss;
	ss << "gfortran -shared " << defaultOpts << opts << " " << dvode << " " << bolsig << " " << ZDPlasKin << " "
	   << " -o zdplaskin.dll";
	std::string command = ss.str();
	std::cout << "compile command: " << command << std::endl;
	int retVal = system(command.c_str());
	if (retVal == 0) {
		std::cout << "compiled!" << std::endl;
	} else
		throw CompileException("Could not compile ZDPlasKin with options '" + opts + "'");
}
