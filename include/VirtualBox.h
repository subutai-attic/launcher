#ifndef __SL_VIRTUAL_BOX_H__
#define __SL_VIRTUAL_BOX_H__

#include <cstdlib>
#include <iostream>
#include <utility>
#include <locale>
#include <codecvt>
#include <vector>
#include <string>

#include "Vars.h"

#include "Environment.h"
#include "String.h"
#include "FileSystem.h"
#include "Process.h"

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

};

#endif
