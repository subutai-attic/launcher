#ifndef __SL_VIRTUAL_BOX_H__
#define __SL_VIRTUAL_BOX_H__

#include "Vars.h"

#include <cstdlib>
#include <iostream>
#include <utility>
#include <locale>
#include <codecvt>
#include <vector>
#include <string>
#include <iomanip>
#include <ctime>

#include "Poco/StringTokenizer.h"
#include "Poco/Environment.h"
#include "Poco/Logger.h"
#include "Poco/Process.h"
#include "Poco/PipeStream.h"
#include "Poco/StreamCopier.h"
#include "Poco/String.h"

#include "Environment.h"
#include "SubutaiString.h"
#include "FileSystem.h"
#include "SSH.h"
#include "Session.h"

namespace SubutaiLauncher {

    struct SubutaiVM {
        std::string name;
        std::string id;
    };

    class VirtualBox {
        public:
            static const std::string BIN;
            static std::string cloneName;
            static std::string subutaiBranch;
            VirtualBox();
            ~VirtualBox();
            bool findInstallation();
            bool isInstalled();
            bool isRunning();
            bool isUpdateRequired();
            std::string extractVersion();
            void getVms();
            std::vector<SubutaiVM> parseVms(const std::string& buffer);
            std::vector<SubutaiVM> getPeers();
            void addVm(SubutaiVM vm);
            std::string execute(const std::string& command);
            std::string execute(const std::string& command, int &exitStatus);

            // New version
            std::string importVirtualMachine(const std::string& fileName, const std::string& targetName) const;
            std::string getBridgedInterface(const std::string& iface);
            std::string getMachineInfo(const std::string& name);
            bool isMachineExists(const std::string& name);
            bool isMachineRunning(const std::string& name);

        private:
            Poco::Logger* _logger;
            std::string _version;
            std::string _path;
            std::string _location;
            bool _installed;
            bool _running;
            bool _updateRequired;
            std::vector<SubutaiVM> _vms;
    };

}

#endif
