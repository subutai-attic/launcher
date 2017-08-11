#include "Session.h"

SubutaiLauncher::Session* SubutaiLauncher::Session::_instance = NULL;

SubutaiLauncher::Session::Session() :
  _terminate(false),
  _sshUser(""),
  _sshPass(""),
  _sshHostname(""),
  _sshPort(0)
{
  _settings = new Settings();
  _downloader = new Downloader();
  _downloader->setOutputDirectory(_settings->getTmpPath());
  _notificationCenter = new NotificationCenter();
  _hub = new Hub();
}

SubutaiLauncher::Session::~Session()
{
    if (_settings != nullptr) delete _settings;
    if (_downloader != nullptr) delete _downloader;
    if (_notificationCenter != nullptr) delete _notificationCenter;
    if (_hub != nullptr) delete _hub;
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

SubutaiLauncher::NotificationCenter* SubutaiLauncher::Session::getNotificationCenter()
{
  return _notificationCenter;
}

SubutaiLauncher::Hub* SubutaiLauncher::Session::getHub()
{
    return _hub;
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

void SubutaiLauncher::Session::addStatus(const std::string& text)
{
  _statusPool.push_back(text);
}

std::string SubutaiLauncher::Session::getStatus()
{
  if (_statusPool.empty()) return "";

  auto elem = _statusPool.back();
  _statusPool.pop_back();
  return elem;
}

Poco::Logger& SubutaiLauncher::Session::logger()
{
  return Poco::Logger::get("SubutaiLogger");
}

bool SubutaiLauncher::Session::isTerminating()
{
  return _terminate;
}

void SubutaiLauncher::Session::terminate()
{
  _terminate = true;
}

void SubutaiLauncher::Session::start()
{
  _terminate = false;
}
