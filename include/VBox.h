#ifndef __VBOX_H__
#define __VBOX_H__

#include "Vars.h"

#ifdef LAUNCHER_LINUX
#include <VirtualBox_XPCOM.h>
#include <nsXPCOM.h>
#include <nsIMemory.h>
#include <nsIServiceManager.h>
#include <nsIEventQueue.h>
#include <nsEventQueueUtils.h>
#endif

namespace SubutaiLauncher {

    class VBox : public IVirtualBox {
        public:
            VBox();
            ~VBox();
    };

};

#endif
