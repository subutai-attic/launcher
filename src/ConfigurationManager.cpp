#include "ConfigurationManager.h"


#if LAUNCHER_LINUX
const std::string SubutaiLauncher::ConfigurationManager::CONFIG_FILE = "install_config_linux";
#elif LAUNCHER_WINDOWS
const std::string SubutaiLauncher::ConfigurationManager::CONFIG_FILE = "install_config_windows";
#elif LAUNCHER_MACOS
const std::string SubutaiLauncher::ConfigurationManager::CONFIG_FILE = "install_config_macos";
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
	std::printf("Reading main configuration\n");
	auto f = _file.substr(0, _file.size() - 3);
	SL script;
	try {
		script.open(f);
	}
	catch (SubutaiException& e) {
		if (e.code() == 1) {
			std::printf("%s file was not found\n", CONFIG_FILE.c_str());
			return;
		}
	}
	try {
		script.execute(CONFIG_FILE);
	}
	catch (SLException& e) {
		std::printf("SL Exception: %s\n", e.displayText().c_str());
	}
	catch (std::exception e) {
		std::printf("Exception: %s\n", e.what());
	}
}

void SubutaiLauncher::ConfigurationManager::addConfig(std::string name)
{
	std::printf("Adding new configuration\n");
	InstallConfig c;
	c.title = name;
	_configs.push_back(c);
}

void SubutaiLauncher::ConfigurationManager::addDesc(std::string name, std::string description)
{
	std::printf("Setting configuration description\n");
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

std::vector<InstallConfig> SubutaiLauncher::ConfigurationManager::getConfigs()
{
	return _configs;
}
