#ifndef __SETTINGS_H__
#define __SETTINGS_H__

#include "Vars.h"

#include <string>
#include <sstream>
#include <fstream>
#include <streambuf>

#include "Vars.h"
#include "FileSystem.h"
#include "Poco/Environment.h"

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
			std::string getDefaultTmpPath() const;
			std::string getDefaultInstallationPath() const;
            void setInstallationPath(const std::string& path);
            void setTmpPath(const std::string& path);
			void setCoreNum(int num);
			void setMemSize(int size);
			int getCoreNum();
			int getMemSize();
        private:
            std::string _installationPath;
            std::string _tmpPath;
			int _coreNum;
			int _memSize;
    };
}

#endif
