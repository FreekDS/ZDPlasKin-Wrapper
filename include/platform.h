#ifndef ZDPLASKIN_PLATFORM_H
#define ZDPLASKIN_PLATFORM_H

// TODO: Belangrijk!! 64bit 32bit detection and link with appropriate libraries

#if defined(WIN32) || defined(WIN64)

#define WINDOWS
#define SEP "\\"

#define LIB_PATH "\\libs\\win\\"
#define PREP_NAME "preprocessor.exe"
#define BOLSIG_EXT "_x86_64_g.dll"

#define MAX_PATH 260

#elif defined(UNIX) || defined(POSIX)

#define UNIX
#define SEP "/"

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
