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

#include "Environment.h"
#include "SubutaiString.h"
#include "FileSystem.h"
#include "SubutaiProcess.h"

namespace SubutaiLauncher {

    struct SubutaiVM {
        std::string name;
        std::string id;
    };

    class VirtualBox {
        public:
            static const std::string BIN;
            VirtualBox();
            ~VirtualBox();
            bool isInstalled();
            bool isRunning();
            bool isUpdateRequired();
            std::string retrieveVersion();
            void getVms();
            std::vector<SubutaiVM> parseVms(const std::string& buffer);
            void addVm(SubutaiVM vm);
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
