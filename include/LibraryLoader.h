#ifndef ZDPLASKIN_LIBRARYLOADER_H
#define ZDPLASKIN_LIBRARYLOADER_H

#include <string>
#include "ZDPlasKinExceptions.h"

class ILibraryLoader {
protected:
    const char *_libPath = "";
    bool _initialized = false;

    virtual void *getFuncAddr(const std::string &name) = 0;

    void requireInitialization() const {
        if (!_initialized) {
            throw LibraryException("Library loader is not initialized, call 'init()' first");
        }
    }

public:
    explicit ILibraryLoader(const std::string &library)
            : _libPath(library.c_str()), _initialized(false) {}

    ILibraryLoader() = default;

    template<typename T>
    T getFunction(const std::string &name) {
        requireInitialization();
        T functionAddress = (T) getFuncAddr(name);
        if (functionAddress)
            return functionAddress;
        throw LibraryException("Cannot find symbol '" + name + "' in library");
    }

    virtual void init() = 0;

    virtual ~ILibraryLoader() = default;
};

#endif //ZDPLASKIN_LIBRARYLOADER_H
