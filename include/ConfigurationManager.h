#ifndef __CONFIGURATION_MANAGER_H__
#define __CONFIGURATION_MANAGER_H__

#include <vector>
#include <string>

#include "Vars.h"
#include "Settings.h"
#include "Downloader.h"
#include "SL.h"

namespace SubutaiLauncher 
{

    struct InstallConfig 
    {
        std::string title;
        std::string description;
        std::string file;
    };

    class ConfigurationManager 
    {
        public:
            static const std::string CONFIG_FILE;
            ConfigurationManager(Downloader* d);
            ~ConfigurationManager();
            void addConfig(std::string name);
            void addDesc(std::string name, std::string description);
            void addFile(std::string name, std::string file);
            std::vector<InstallConfig> getConfigs();
            void load();
            void run();
        private:
            std::vector<InstallConfig> _configs;
            Downloader* _downloader;
            std::string _file;
    };
}

#endif
