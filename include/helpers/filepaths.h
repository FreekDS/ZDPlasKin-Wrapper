#ifndef ZDPLASKIN_FILEPATHS_H
#define ZDPLASKIN_FILEPATHS_H

#include "platform.h"
#include "bindings.h"
#include <filesystem>

#ifdef WINDOWS

#include <Windows.h>

std::string getExePath() {

    char path[MAX_PATH];
    HMODULE hm = nullptr;

    if (GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS |
                          GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
                          (LPCSTR) &handle_func, &hm) == 0)
    {
        DWORD ret = GetLastError();
        fprintf(stderr, "GetModuleHandle failed, error = %d\n", (int)ret);
        // Return or however you want to handle an error.
        return "";
    }
    if (GetModuleFileName(hm, path, sizeof(path)) == 0)
    {
        DWORD ret = GetLastError();
        fprintf(stderr, "GetModuleFileName failed, error = %d\n", (int)ret);
        // Return or however you want to handle an error.
        return "";
    }
    return path;

    char buffer[MAX_PATH];
    GetModuleFileName(nullptr, buffer, sizeof(buffer));
    return std::filesystem::path(buffer).parent_path().string();
}

#else

std::string getExePath() {
    return std::filesystem::canonical("/proc/self/exe").string();
}

#endif


#endif //ZDPLASKIN_FILEPATHS_H
