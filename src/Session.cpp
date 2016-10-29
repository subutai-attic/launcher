#include "Session.h"


Session* SubutaiLauncher::Session::_instance = NULL;

SubutaiLauncher::Session::Session()
{
	_settings = new Settings();
	_downloader = new Downloader();
	_downloader->setOutputDirectory(_settings->getTmpPath());
	_confManager = new ConfigurationManager(_downloader);
}

SubutaiLauncher::Session::~Session()
{

}

Session* SubutaiLauncher::Session::instance()
{
	if (!_instance) _instance = new Session();
	return _instance;
}

void SubutaiLauncher::Session::destroyInstance()
{
	delete this;
}

Downloader* SubutaiLauncher::Session::getDownloader()
{
	return _downloader;
}

Settings* SubutaiLauncher::Session::getSettings()
{
	return _settings;
}

ConfigurationManager* SubutaiLauncher::Session::getConfManager()
{
	return _confManager;
}
