#include "EnvironmentImpl.h"

#if LAUNCHER_WINDOWS
//#include "EnvironmentImpl_Windows.cpp"
#elif LAUNCHER_LINUX
#include "EnvironmentImpl_Linux.cpp"
#elif LAUNCHER_MACOS
#include "EnvironmentImpl_Darwin.cpp"
#endif
