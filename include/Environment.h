#ifndef __ENVIRONMENT_H__
#define __ENVIRONMENT_H__

#include "Vars.h"

#include <string>
#include <cstdlib>
#include <unistd.h>
#include <limits.h>
#if LAUNCHER_WINDOWS
#include <windows.h>
#endif

#include "SubutaiException.h"
#include "SubutaiLog.h"
#include "SubutaiProcess.h"
#include "SubutaiString.h"
#include <Poco/Environment.h>

namespace SubutaiLauncher {

    class Environment {
        public:
            Environment();
            ~Environment();
	    std::string versionOS();
	    std::string distroOS(std::string ar);
	    std::string cpuArch();
	    unsigned cpuNum();
            unsigned processorNum();
	    unsigned is64();
	    unsigned long ramSize();
	    unsigned versionVBox();
	    std::string versionSSH();
	    std::string vtxEnabled();
            std::string getVar(const std::string& name, const std::string& defaultValue);
            std::string setVar(const std::string& name, const std::string& value);
    };

}

#endif
