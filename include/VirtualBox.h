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

#include <Poco/StringTokenizer.h>

#include "Environment.h"
#include "SubutaiString.h"
#include "FileSystem.h"
#include "SubutaiProcess.h"
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
            std::string execute(const std::string& command, int *exitStatus);
            std::string sysExecute(const std::string& command, const std::string& cargs);
            std::string sysExecute(const std::string& command, const std::vector<std::string>& cargs);
            std::string cloneVM();
            bool cleanKnownHosts(std::string sport);
            bool runScripts(std::string instVersion, std::string isMH);
            bool setAutobuildIP(SSH &s_ssh);
            bool stopVM();
            bool restoreNet();
            bool startVM();
            bool waitingSSH(SSH &s_ssh, std::string sport);
            bool connectSSH(SSH &s_ssh);
            bool waitPeerIP();
            bool runAutobuild();
            bool importManagement();
            std::string getBranch( std::string  instVersion);

            // New version
            std::string importVirtualMachine(const std::string& fileName, const std::string& targetName) const;
            std::string getBridgedInterface(const std::string& iface);

        protected:
        private:
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
