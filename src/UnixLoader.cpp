#include "unix/UnixLoader.h"

void UnixLoader::init() {
	// TODO implement
	_initialized = true;
}

void *UnixLoader::getFuncAddr(const std::string &name) {
	requireInitialization();
	// TODO implement
	return nullptr;
}

UnixLoader::UnixLoader(const std::string &library)
	: ILibraryLoader(library) {}

UnixLoader::~UnixLoader() {
	// TODO implement
}
