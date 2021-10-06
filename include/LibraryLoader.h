#ifndef ZDPLASKIN_LIBRARYLOADER_H
#define ZDPLASKIN_LIBRARYLOADER_H

#include <string>
#include "ZDPlasKinExceptions.h"

class ILibraryLoader {
 protected:
	std::string _libPath;
	bool _initialized = false;

	virtual void *getFuncAddr(const std::string &name) = 0;

	void requireInitialization() const {
		if (!_initialized) {
			throw LibraryException("Library loader is not initialized, call 'setFilePath()' first");
		}
	}

 public:
	explicit ILibraryLoader(const std::string &library)
		: _libPath(library), _initialized(false) {}

	ILibraryLoader() noexcept = default;

	template<typename T>
	T getFunction(const std::string &name) {
		requireInitialization();
		T functionAddress = (T)getFuncAddr(name);
		if (functionAddress)
			return functionAddress;
		throw LibraryException("Cannot find symbol '" + name + "' in library");
	}

	virtual void init() = 0;

	virtual void init(const std::string &library) {
		_libPath = library;
	}

	virtual ~ILibraryLoader() = default;
};

#endif //ZDPLASKIN_LIBRARYLOADER_H
