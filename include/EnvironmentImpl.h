#ifndef __ENVIRONMENT_IMPL_H__
#define __ENVIRONMENT_IMPL_H__

#include "Vars.h"

#if LAUNCHER_WINDOWS
#include "EnvironmentImpl_Windows.h"
#elif LAUNCHER_LINUX
#include "EnvironmentImpl_Linux.h"
#elif LAUNCHER_MACOS
#include "EnvironmentImpl_Darwin.h"
#endif

#endif
