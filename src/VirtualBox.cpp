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

    void VirtualBox::loadMachines() {
        /*  
        uint32_t c;
        IMachine** list;
        auto res = _vbox->GetMachines(&c, &list);
        
        for (auto i = 0; i < c; i++) {
            PRUnichar* name;
            list[i]->GetName(&name);
            //std::wstring_convert<std::codecvt<char16_t, char, std::mbstate_t>, char16_t> converter;
            std::wstring_convert<codecvt<char16_t, char, std::mbstate_t>> converter;
            std::string n = converter.to_bytes((wchar_t)name);
            std::cout << "Name: " << n << std::endl;
        }
        */
    }

    void VirtualBox::convertName(PRUnichar* orig) {
        /*
        nsAutoString str;
        str.Assign(orig);

        int l = str.length();
        *jj
            */
    }

};
