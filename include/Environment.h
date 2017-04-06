#ifndef __ENVIRONMENT_H__
#define __ENVIRONMENT_H__

#include "Vars.h"

#include <string>
#include <cstdlib>
#if LAUNCHER_LINUX
#include <unistd.h>
#endif
#include <limits.h>


#include "SubutaiException.h"
#include "SubutaiString.h"
#include "Poco/Environment.h"
#include "Poco/StringTokenizer.h"
#include "Poco/Process.h"
#include "Poco/Pipe.h"
#include "Poco/PipeStream.h"
#include "Poco/StreamCopier.h"
#include "Poco/Logger.h"

#if LAUNCHER_WINDOWS
//#include <windows.h>
#elif LAUNCHER_MACOS
#include <sys/sysctl.h>
#endif

namespace SubutaiLauncher {

    class Environment {
        public:
            Environment();
            ~Environment();
            std::string versionOS();
            std::string cpuArch();
            unsigned cpuNum();
            unsigned processorNum();
            unsigned is64();
            unsigned long ramSize();
            unsigned versionVBox();
            bool vtxEnabled();
            std::string getVar(const std::string& name, const std::string& defaultValue);
            std::string setVar(const std::string& name, const std::string& value);
            std::string getDefaultGateway();
        private:
            Poco::Logger* _logger;
    };

}

#endif
