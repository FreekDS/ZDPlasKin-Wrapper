#include "windows/WindowsLoader.h"

void WindowsLoader::init() {
	// Todo allow for reinitialization (free first in that case)
	_module = LoadLibrary(TEXT(_libPath));
	if (_module == nullptr) {
		DWORD error = GetLastError();
		_initialized = false;
		throw LibraryException(
			"Cannot load library at location '" + std::string(_libPath) + "' (code " + std::to_string(error) + ")");
	}
	_initialized = true;
}

WindowsLoader::~WindowsLoader() {
	if (_module)
		FreeLibrary(_module);
	_module = nullptr;
}

WindowsLoader::WindowsLoader(const std::string &library)
	: ILibraryLoader(library) {}

void *WindowsLoader::getFuncAddr(const std::string &name) {
	requireInitialization();
	return (void *)GetProcAddress(_module, name.c_str());
}
