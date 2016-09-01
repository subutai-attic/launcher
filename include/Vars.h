#ifndef __VARS_H__
#define __VARS_H__

#if defined(linux) || defined(__linux) || defined(__linux__) || defined(__TOS_LINUX__) || defined(EMSCRIPTEN)
#define LAUNCHER_LINUX 1
#elif defined(__APPLE__) || defined(__TOS_MACOS__)
#define LAUNCHER_MACOS 1
#elif defined(_WIN32) || defined(_WIN64)
#define LAUNCHER_WIN 1
#else
#define LAUNCHER_LINUX 1
#endif

#endif
