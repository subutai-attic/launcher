#ifndef __CONFIGURATION_MANAGER_H__
#define __CONFIGURATION_MANAGER_H__

#include <vector>
#include <string>

#include "Vars.h"
#include "Session.h"
#include "Settings.h"
#include "Downloader.h"

namespace SubutaiLauncher {

    struct InstallConfig {
        std::string title;
        std::string description;
    };

    class ConfigurationManager {
        public:
            static const std::string CONFIG_FILE;
            ConfigurationManager();
            ~ConfigurationManager();
            void addConfig(std::string name);
            void addDesc(std::string name, std::string description);
            std::vector<std::string> getConfigs();
        private:
            void load();
            void run();
            std::vector<std::string> _configs;
    };
};

#endif
