#ifndef __TRAY_H__
#define __TRAY_H__

#include "Environment.h"
#include "SubutaiString.h"
#include "FileSystem.h"
#include "SubutaiProcess.h"
#include "SubutaiLog.h"

#include <string>

namespace SubutaiLauncher {

    class Tray {
        public:
            static const std::string BIN;
            Tray();
            ~Tray();
            std::string extractVersion();
            bool findInstallation();
            bool isInstalled();
        private:
            std::string _version;
            std::string _path;
            std::string _location;
            bool _installed;
    };

}

#endif
