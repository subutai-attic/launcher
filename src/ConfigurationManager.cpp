#include "ConfigurationManager.h"

namespace SubutaiLauncher {

#if LAUNCHER_LINUX
    const std::string ConfigurationManager::CONFIG_FILE = "install_config_linux.py";
#elif LAUNCHER_WINDOWS
    const std::string ConfigurationManager::CONFIG_FILE = "install_config_windows.py";
#elif LAUNCHER_MACOS
    const std::string ConfigurationManager::CONFIG_FILE = "install_config_macos.py";
#else
#error Unknown Platform
#endif

    ConfigurationManager::ConfigurationManager() {

    }

    ConfigurationManager::~ConfigurationManager() {

    }

    void ConfigurationManager::load() {
        auto download = Session::instance()->getDownloader();
        download->setFilename(CONFIG_FILE);
        auto t = download->download();
        t.join();
    }

    void ConfigurationManager::run() {
        
    }

    void ConfigurationManager::addConfig(std::string name) {

    }

    void ConfigurationManager::addDesc(std::string name, std::string description) {

    }

    std::vector<std::string> ConfigurationManager::getConfigs() {
        return _configs;
    }
};
