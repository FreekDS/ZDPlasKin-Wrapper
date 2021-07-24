#ifndef ZDPLASKIN_SRC_ZDPLASKINCOMPILER_H_
#define ZDPLASKIN_SRC_ZDPLASKINCOMPILER_H_

#include <string>
#include "platform.h"

class ZDPlasKinCompiler {
 public:
	static void preprocess(const std::string &input_file);
	static void compile(const std::string &opts = "");
};

#endif //ZDPLASKIN_SRC_ZDPLASKINCOMPILER_H_
