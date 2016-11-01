#ifndef __INSTALL_H__
#define __INSTALL_H__

#include "Vars.h"
#if LAUNCHER_LINUX
#include "InstallImpl_Linux.h"
#elif LAUNCHER_WINDOWS
#include "InstallImpl_Windows.h"
#elif LAUNCHER_MACOS
#include "InstallImpl_Darwin.h"
#endif

namespace SubutaiLauncher {
    class Install : public InstallImpl
    {
        public:
            Install();
            ~Install();
            void preInstall();
            void install();
            void postInstall();
        private:
            InstallImpl* _impl;
    };
}

#endif
