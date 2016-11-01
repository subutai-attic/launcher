#ifndef __SETTINGS_H__
#define __SETTINGS_H__

#include <string>
#include <sstream>
#include <fstream>
#include <streambuf>

#include "Vars.h"
#include "FileSystem.h"

namespace SubutaiLauncher {
    class Settings {
        public:
            static const std::string CONFIG_FILE;
            static const std::string DEFAULT_PATH;
            static const std::string DEFAULT_TMP;
            Settings();
            ~Settings();
            void load();
            void save();
            std::string getInstallationPath() const;
            std::string getTmpPath() const;
            void setInstallationPath(const std::string& path);
            void setTmpPath(const std::string& path);
        private:
            std::string _installationPath;
            std::string _tmpPath;
    };
}

#endif
