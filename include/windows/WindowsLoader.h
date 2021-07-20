#ifndef ZDPLASKIN_WINDOWSLOADER_H
#define ZDPLASKIN_WINDOWSLOADER_H

#include "LibraryLoader.h"
#include <windows.h>

class WindowsLoader : public ILibraryLoader {
 private:
	HMODULE _module = nullptr;
 public:
 private:
	void *getFuncAddr(const std::string &name) override;
 public:
	explicit WindowsLoader(const std::string &library);

	void init() override;

	~WindowsLoader() override;
};

#endif //ZDPLASKIN_WINDOWSLOADER_H
