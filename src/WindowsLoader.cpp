#include "windows/WindowsLoader.h"

void WindowsLoader::init() {
	_module = LoadLibrary(TEXT(_libPath));
	if (_module==nullptr) {
		throw std::exception();
	}
}

WindowsLoader::~WindowsLoader() {
	if (_module)
		FreeLibrary(_module);
	_module = nullptr;
}

WindowsLoader::WindowsLoader(const std::string &library)
	: ILibraryLoader(library) {}

void *WindowsLoader::getFuncAddr(const std::string &name) {
	return (void *)GetProcAddress(_module, (name + '_').c_str());
}
