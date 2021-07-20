#ifndef ZDPLASKIN_PLATFORM_H
#define ZDPLASKIN_PLATFORM_H

#if defined(WIN32) || defined(WIN64)
#define WINDOWS
#elif defined(UNIX) || defined(POSIX)
#define UNIX
#endif

#endif //ZDPLASKIN_PLATFORM_H
