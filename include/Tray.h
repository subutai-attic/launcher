#ifndef __TRAY_H__
#define __TRAY_H__

#include "Vars.h"
#include "Environment.h"
#include "SubutaiString.h"
#include "FileSystem.h"

#include "Poco/Logger.h"
#include "Poco/Process.h"
#include "Poco/Pipe.h"
#include "Poco/PipeStream.h"
#include "Poco/StreamCopier.h"

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
            Poco::Logger* _logger;
            std::string _version;
            std::string _path;
            std::string _location;
            bool _installed;
    };

}

#endif
