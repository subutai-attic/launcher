#ifndef __INSTALL_IMPL_H__
#define __INSTALL_IMPL_H__

#include <string.h>
#include <unistd.h>
#include <vector>

#include "InstallImpl_Base.h"
#include "FileSystem.h"
#include "Environment.h"
#include "Vars.h"
#include "SubutaiException.h"
#include "String.h"

namespace SubutaiLauncher
{
    class InstallImpl : public InstallImpl_Base
    {
        public:
            InstallImpl();
            void preInstall();
            void install();
            void postInstall();
    };
}

#endif
