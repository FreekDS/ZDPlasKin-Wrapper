#ifndef ZDPLASKIN_PLATFORM_H
#define ZDPLASKIN_PLATFORM_H

#if defined(WIN32) || defined(WIN64)

#define WINDOWS

#define LIB_PATH ".\\libs\\win\\"
#define PREP_PATH LIB_PATH "preprocessor.exe"
#define BOLSIG_EXT "_x86_64_g.dll"

#elif defined(UNIX) || defined(POSIX)

#define UNIX

#if defined(POSIX)

#define BOLSIG_EXT ".dylib"
#define LIB_PATH "./libs/macos/"
#else
#define BOLSIG_EXT ".so"
#define LIB_PATH "./libs/linux/"

#endif

#define PREP_PATH LIB_PATH "preprocessor"

#endif

#endif //ZDPLASKIN_PLATFORM_H
