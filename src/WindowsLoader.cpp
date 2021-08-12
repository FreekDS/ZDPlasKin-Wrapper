#include "windows/WindowsLoader.h"

void WindowsLoader::init() {
	// Todo allow for reinitialization (free first in that case)
	free();
	_module = LoadLibrary(TEXT(_libPath.c_str()));
	if (_module == nullptr) {
		DWORD error = GetLastError();
		_initialized = false;
		throw LibraryException(
			"Cannot load library at location '" + std::string(_libPath) + "' (code " + std::to_string(error) + ")");
	}
	_initialized = true;
}

WindowsLoader::~WindowsLoader() {
	free();
}

WindowsLoader::WindowsLoader(const std::string &library)
	: ILibraryLoader(checkLibExt(library)) {}

void *WindowsLoader::getFuncAddr(const std::string &name) {
	requireInitialization();
	return (void *)GetProcAddress(_module, name.c_str());
}

void WindowsLoader::init(const std::string &library) {
	ILibraryLoader::init(checkLibExt(library));
	init();
}

void WindowsLoader::free() {
	if (_module)
		FreeLibrary(_module);
	_module = nullptr;
}

std::string WindowsLoader::checkLibExt(const std::string &str) {
	if (!str.ends_with(".dll")) {
		return str + ".dll";
	}
	return str;
}
