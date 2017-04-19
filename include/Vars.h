#ifndef __VARS_H__
#define __VARS_H__

#if defined(linux) || defined(__linux) || defined(__linux__) || defined(__TOS_LINUX__) || defined(EMSCRIPTEN)
#define LAUNCHER_LINUX 1
#define PATH_DELIM "/"
#elif defined(__APPLE__) || defined(__TOS_MACOS__)
#define LAUNCHER_MACOS 1
#define PATH_DELIM "/"
#elif defined(_WIN32) || defined(_WIN64)
#define LAUNCHER_WINDOWS 1
#define PATH_DELIM "\\"
#else
#define LAUNCHER_LINUX 1
#endif

#ifndef BUILD_SCHEME
#if BUILD_SCHEME_DEV
#define BUILD_SCHEME "dev"
#elif BUILD_SCHEME_MASTER
#define BUILD_SCHEME "master"
#else
#define BUILD_SCHEME "production"
#endif
#endif

#if defined(_WIN32) && defined(POCO_DLL)
    #if defined(Subutai_EXPORTS)
        #define Subutai_API __declspec(dllexport)
    #else
        #define Subutai_API __declspec(dllimport)
    #endif
#endif


#if !defined(Subutai_API)
    #if !defined(POCO_NO_GCC_API_ATTRIBUTE) && defined (__GNUC__) && (__GNUC__ >= 4)
        #define Subutai_API __attribute__ ((visibility ("default")))
    #else
        #define Subutai_API
    #endif
#endif

#endif
