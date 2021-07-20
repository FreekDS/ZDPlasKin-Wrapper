#ifndef ZDPLASKIN_LIBRARYLOADER_H
#define ZDPLASKIN_LIBRARYLOADER_H

#include <string>

class ILibraryLoader {
protected:
	const char* _libPath;

	virtual void* getFuncAddr(const std::string& name) = 0;

public:
	explicit ILibraryLoader(const std::string& library)
			:_libPath(library.c_str()) { }

	template<typename T>
	T getFunction(const std::string& name)
	{
		T functionAddress = (T)getFuncAddr(name);
		if (functionAddress)
			return functionAddress;
		throw std::exception();
	}

	virtual void init() = 0;
	virtual ~ILibraryLoader() = default;
};

#endif //ZDPLASKIN_LIBRARYLOADER_H
