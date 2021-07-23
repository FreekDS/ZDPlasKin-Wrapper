#include <sstream>
#include <iostream>
#include "ZDPlasKinCompiler.h"
#include "ZDPlasKinExceptions.h"

void ZDPlasKinCompiler::preprocess(const std::string &input_file) {
	std::string command = std::string(PREP_PATH) + " " + input_file + " zdplaskin_m.F90";
	std::cout << "preprocess command: " << command << std::endl;
	int retVal = system(command.c_str());
	if (retVal == 0) {
		std::cout << "Input file preprocessed" << std::endl;
	} else
		throw CompileException("Could not preprocess the file '" + input_file + "'");
}

void ZDPlasKinCompiler::compile(const std::string &opts) {
	std::string dvode = std::string(LIB_PATH) + "dvode_f90_m.F90";
	std::string ZDPlasKin = "./zdplaskin_m.F90";
	std::string bolsig = std::string(LIB_PATH) + "bolsig" + std::string(BOLSIG_EXT);

	std::stringstream ss;
	ss << "gfortran " << opts << " " << dvode << " " << bolsig << " " << ZDPlasKin << " " << " -o zdplaskin";
	std::string command = ss.str();
	std::cout << "compile command: " << command << std::endl;
	int retVal = system(command.c_str());
	if (retVal == 0) {
		std::cout << "compiled!" << std::endl;
	} else
		throw CompileException("Could not compile ZDPlasKin with options '" + opts + "'");
}
