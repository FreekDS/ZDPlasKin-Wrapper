#include "unix/UnixLoader.h"

void UnixLoader::init() {
	// TODO implement
}

void *UnixLoader::getFuncAddr(const std::string &name) {
	// TODO implement
	return nullptr;
}

UnixLoader::UnixLoader(const std::string &library)
	: ILibraryLoader(library) {}

UnixLoader::~UnixLoader() {
	// TODO implement
}
