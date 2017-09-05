#ifndef __ENVIRONMENT_H__
#define __ENVIRONMENT_H__

#include "Vars.h"
#include "EnvironmentImpl.h"

namespace SubutaiLauncher {

    class Environment {
        public:
            static const std::string EXTRA_PATH;
            Environment();
            ~Environment();
            std::string versionOS();
            std::string versionNumber();
            std::string cpuArch();
            unsigned int cpuNum();
            unsigned processorNum();
            unsigned is64();
            ULORAMSIZE_T ramSize();
            unsigned versionVBox();
            bool vtxEnabled();
            std::string getVar(const std::string& name, const std::string& defaultValue);
            std::string setVar(const std::string& name, const std::string& value);
            std::string getDefaultGateway();
			// Windows only
			bool isNSSMInstalled();
			bool registerService(const std::string& name, const std::string& path, std::vector<std::string> args);
			bool unregisterService(const std::string& name);
			bool startService(const std::string& name);
			bool stopService(const std::string& name);
			void CreateShortcut(const std::string& source, const std::string& name);
			int32_t updatePath(const std::string& path);
			bool killProcess(const std::string& name);
			std::string getDesktopDirectory();
			bool isVBoxInstalled();
			bool writeE2ERegistry(const std::string& name);
			BOOL terminateWinProcess(DWORD dwProcessId, UINT uExitCode);
        private:
            EnvironmentImpl* _impl;
    };

}

#endif
