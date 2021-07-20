#include "windows/WindowsLoader.h"

void WindowsLoader::init() {
	// Todo allow for reinitialization (free first in that case)
	_module = LoadLibrary(TEXT(_libPath));
	if (_module==nullptr) {
		_initialized = false;
		throw std::exception();
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
	return (void *)GetProcAddress(_module, (name + '_').c_str());
}
