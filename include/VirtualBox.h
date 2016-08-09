#ifndef __SL_VIRTUAL_BOX_H__
#define __SL_VIRTUAL_BOX_H__

#include <cstdlib>

#include "Vars.h"

#ifdef LAUNCHER_LINUX
#include <VirtualBox_XPCOM.h>
#include <nsXPCOM.h>
#include <nsIMemory.h>
#include <nsIServiceManager.h>
#include <nsIEventQueue.h>
#include <nsEventQueueUtils.h>
#endif

#include "Environment.h"
#include "String.h"
#include "FileSystem.h"

namespace SubutaiLauncher {

    class VirtualBox {
        public:
            static const std::string BIN;
            VirtualBox();
            ~VirtualBox();
            bool isInstalled();
            bool isRunning();
            bool isUpdateRequired();
            std::string retrieveVersion();

        private:
            std::string _version;
            std::string _path;
            bool _installed;
            bool _running;
            bool _updateRequired;

            IVirtualBox _vbox;

    };

};

#endif
