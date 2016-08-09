#include "VirtualBox.h"

namespace SubutaiLauncher {

    const std::string VirtualBox::BIN = "vboxmanage";

    VirtualBox::VirtualBox() : _version("") {
        auto env = new Environment();
        String str(env->getVar("PATH", ""));
        std::vector<std::string> path;
        str.split(':', path);
        FileSystem fs;
        std::printf("Searching for %s binary\n", BIN.c_str());
        for (auto it = path.begin(); it != path.end(); it++) {
            std::printf("Looking in %s...\n", it->c_str());
            fs.setPath((*it));
            if (fs.isFileExists(BIN)) {
                std::printf("Found in %s\n", it->c_str());
                _installed = true;
                _path = (*it);
                _path.append(FileSystem::DELIM);
                _path.append(BIN);
                break;
            }
        }
    }

    VirtualBox::~VirtualBox() {

    }

    bool VirtualBox::isInstalled() {
        return _installed;
    }

    bool VirtualBox::isRunning() {
        return true;
    }

    bool VirtualBox::isUpdateRequired() {
        return false;
    }

    std::string VirtualBox::retrieveVersion() {
        if (_version != "") {
            return _version;
        }

    }

};
