#include "ConfigurationManager.h"
#include "SL.h"


#if LAUNCHER_LINUX
const std::string SubutaiLauncher::ConfigurationManager::CONFIG_FILE = "launcher-install-config-linux";
#elif LAUNCHER_WINDOWS
const std::string SubutaiLauncher::ConfigurationManager::CONFIG_FILE = "launcher-install-config-windows";
#elif LAUNCHER_MACOS
const std::string SubutaiLauncher::ConfigurationManager::CONFIG_FILE = "launcher-install-config-darwin";
#else
#error Unknown Platform
#endif

SubutaiLauncher::ConfigurationManager::ConfigurationManager(Downloader* d) : _downloader(d)
{
}

SubutaiLauncher::ConfigurationManager::~ConfigurationManager()
{

}

void SubutaiLauncher::ConfigurationManager::load()
{
    std::string f(CONFIG_FILE);
    f.append(".py");
    _downloader->setFilename(f);
    auto t = _downloader->download();
    t.join();
    _file = _downloader->getFullPath();
}

void SubutaiLauncher::ConfigurationManager::run()
{
    _configs.clear();
    auto l = Log::instance()->logger();
    l->debug() << "Reading configuration from " << _downloader->getOutputDirectory() << "/" << CONFIG_FILE << std::endl;
    auto f = _file.substr(0, _file.size() - 3);
    SL* script = new SL(_downloader->getOutputDirectory());
    try {
	script->open(CONFIG_FILE);
    }
    catch (SubutaiException& e) {
	if (e.code() == 1) {
    	    l->error() << CONFIG_FILE << " was not found in " << _downloader->getOutputDirectory() << std::endl;
	    return;
	} else {
    	    l->error() << CONFIG_FILE << " error no " << e.code() << " what: " << e.what() << std::endl;
    	    return;
	}
    }
    try {
        script->execute();
    }
    catch (SLException& e) {
        l->error() << "SLException on script execute " << CONFIG_FILE << ": " << e.displayText();
    }
    catch (std::exception e) {
        l->error() << "Exception on script execute "  << CONFIG_FILE << ": "<<  e.what();
    }
}

void SubutaiLauncher::ConfigurationManager::addConfig(std::string name)
{
    Log::instance()->logger()->debug() << "Adding new configuration" << std::endl;
    InstallConfig c;
    c.title = name;
    _configs.push_back(c);
}

void SubutaiLauncher::ConfigurationManager::addDesc(std::string name, std::string description)
{
    Log::instance()->logger()->debug() << "Setting configuration description" << std::endl;
	for (auto it = _configs.begin(); it != _configs.end(); it++) {
		if ((*it).title == name) {
			(*it).description = description;
		}
	}
}

void SubutaiLauncher::ConfigurationManager::addFile(std::string name, std::string file)
{
	for (auto it = _configs.begin(); it != _configs.end(); it++) {
		if ((*it).title == name) {
			(*it).file = file;
		}
	}
}

std::vector<SubutaiLauncher::InstallConfig> SubutaiLauncher::ConfigurationManager::getConfigs()
{
	return _configs;
}
