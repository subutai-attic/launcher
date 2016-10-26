#ifndef __INSTALL_IMPL_H__
#define __INSTALL_IMPL_H__

#include <string.h>

#include "InstallImpl_Base.h"

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
};

#endif
