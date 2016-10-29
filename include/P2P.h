#ifndef __P2P_H__
#define __P2P_H__

#include "Environment.h"
#include "String.h"
#include "FileSystem.h"
#include "Process.h"

namespace SubutaiLauncher {

    class P2P {
        public:
            static const std::string BIN;
            P2P();
            ~P2P();
            bool findInstallation();
            std::string extractVersion();
            void start();
            void stop();
            bool isRunning();
        private:
            std::string _version;
            std::string _path;
            std::string _location;
            bool _installed;
    };

}

#endif
