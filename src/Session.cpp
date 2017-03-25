#include "Session.h"


SubutaiLauncher::Session* SubutaiLauncher::Session::_instance = NULL;

SubutaiLauncher::Session::Session() :
    _sshUser(""),
    _sshPass(""),
    _sshHostname(""),
    _sshPort(0)
{
	_settings = new Settings();
	_downloader = new Downloader();
	_downloader->setOutputDirectory(_settings->getTmpPath());
	_confManager = new ConfigurationManager(_downloader);
	_notificationCenter = new NotificationCenter();
}

SubutaiLauncher::Session::~Session()
{

}

SubutaiLauncher::Session* SubutaiLauncher::Session::instance()
{
	if (!_instance) _instance = new Session();
	return _instance;
}

void SubutaiLauncher::Session::destroyInstance()
{
	delete this;
}

SubutaiLauncher::Downloader* SubutaiLauncher::Session::getDownloader()
{
	return _downloader;
}

SubutaiLauncher::Settings* SubutaiLauncher::Session::getSettings()
{
	return _settings;
}

SubutaiLauncher::ConfigurationManager* SubutaiLauncher::Session::getConfManager()
{
	return _confManager;
}

SubutaiLauncher::NotificationCenter* SubutaiLauncher::Session::getNotificationCenter()
{
    return _notificationCenter;
}

void SubutaiLauncher::Session::setSSHCredentials(const std::string& user, const std::string& pass, const std::string& hostname, long port) 
{
    _sshUser = user;
    _sshPass = pass;
    _sshHostname = hostname;
    _sshPort = port;
}

std::string SubutaiLauncher::Session::getSSHUser()
{
    return _sshUser;
}

std::string SubutaiLauncher::Session::getSSHPass()
{
    return _sshPass;
}

std::string SubutaiLauncher::Session::getSSHHostname()
{
    return _sshHostname;
}

long SubutaiLauncher::Session::getSSHPort()
{
    return _sshPort;
}
