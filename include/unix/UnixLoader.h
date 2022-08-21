#ifndef ZDPLASKIN_UNIXLOADER_H
#define ZDPLASKIN_UNIXLOADER_H

#include "LibraryLoader.h"

class UnixLoader : public ILibraryLoader {
public:
    explicit UnixLoader(const std::string &library);

    UnixLoader() = default;

    void init() override;

    ~UnixLoader() override;

protected:
    void *getFuncAddr(const std::string &name) override;

};

#endif //ZDPLASKIN_UNIXLOADER_H
