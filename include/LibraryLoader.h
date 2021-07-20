#ifndef ZDPLASKIN_LIBRARYLOADER_H
#define ZDPLASKIN_LIBRARYLOADER_H

#include <string>

class ILibraryLoader {
 protected:
	const char *_libPath;
	bool _initialized;
	virtual void *getFuncAddr(const std::string &name) = 0;

	void requireInitialization() const {
		if(!_initialized) {
			throw std::exception();
		}
	}

 public:
	explicit ILibraryLoader(const std::string &library)
		: _libPath(library.c_str()), _initialized(false) {}

	template<typename T>
	T getFunction(const std::string &name) {
		requireInitialization();
		T functionAddress = (T)getFuncAddr(name);
		if (functionAddress)
			return functionAddress;
		throw std::exception();
	}

	virtual void init() = 0;
	virtual ~ILibraryLoader() = default;
};

#endif //ZDPLASKIN_LIBRARYLOADER_H
