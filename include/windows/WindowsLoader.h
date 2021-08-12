#ifndef ZDPLASKIN_WINDOWSLOADER_H
#define ZDPLASKIN_WINDOWSLOADER_H

#include "LibraryLoader.h"
#include "ZDPlasKinExceptions.h"
#include <windows.h>

class WindowsLoader : public ILibraryLoader {
 private:
	HMODULE _module = nullptr;
 public:
 private:
	void *getFuncAddr(const std::string &name) override;
 	void free();

 	static std::string checkLibExt(const std::string &str);

 public:
	explicit WindowsLoader(const std::string &library);

	WindowsLoader() noexcept = default;

	void init(const std::string &library) override;

	void init() override;

	~WindowsLoader() override;
};

#endif //ZDPLASKIN_WINDOWSLOADER_H
