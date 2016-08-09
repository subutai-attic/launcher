#ifndef __VIRTUAL_BOX_H__
#define __VIRTUAL_BOX_H__

#include <cstdlib>

#include "Environment.h"

namespace SubutaiLauncher {

    class VirtualBox {
        public:
            VirtualBox();
            ~VirtualBox();
            bool isInstalled();
            bool isRunning();
            bool isUpdateRequired();
            std::string retrieveVersion();

        private:
            std::string _version;
            bool _installed;
            bool _running;
            bool _updateRequired;

    };

};

#endif
