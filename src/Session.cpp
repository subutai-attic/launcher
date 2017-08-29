#include "Session.h"
#include "SSH.h"

namespace SubutaiLauncher
{

    Session* Session::_instance = NULL;

    Session::Session() :
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

    Session::~Session()
    {
        // Cleanup stalled SSH sessions
        for (auto it = _sshSessions.begin(); it != _sshSessions.end(); it++)
        {
            (*it).session->closeChannel();
            (*it).session->disconnect();
            delete (*it).session;
        }
        _sshSessions.clear();
        if (_settings != nullptr) delete _settings;
        if (_downloader != nullptr) delete _downloader;
        if (_notificationCenter != nullptr) delete _notificationCenter;
        if (_hub != nullptr) delete _hub;
    }

    Session* Session::instance()
    {
        if (!_instance) _instance = new Session();
        return _instance;
    }

    void Session::destroyInstance()
    {
        delete this;
    }

    SubutaiLauncher::Downloader* Session::getDownloader()
    {
        return _downloader;
    }

    SubutaiLauncher::Settings* Session::getSettings()
    {
        return _settings;
    }

    SubutaiLauncher::NotificationCenter* Session::getNotificationCenter()
    {
        return _notificationCenter;
    }

    SubutaiLauncher::Hub* Session::getHub()
    {
        return _hub;
    }

    void Session::setSSHCredentials(const std::string& user, const std::string& pass, const std::string& hostname, long port) 
    {
        _sshUser = user;
        _sshPass = pass;
        _sshHostname = hostname;
        _sshPort = port;
    }

    std::string Session::getSSHUser()
    {
        return _sshUser;
    }

    std::string Session::getSSHPass()
    {
        return _sshPass;
    }

    std::string Session::getSSHHostname()
    {
        return _sshHostname;
    }

    long Session::getSSHPort()
    {
        return _sshPort;
    }

    void Session::addStatus(const std::string& text)
    {
        _statusPool.push_back(text);
    }

    void Session::replaceStatus(const std::string& text)
    {
        _statusPool.push_back(text);
    }

    std::string Session::getStatus()
    {
        if (_statusPool.empty()) return "";

        auto elem = _statusPool.back();
        _statusPool.pop_back();
        return elem;
    }

    Poco::Logger& Session::logger()
    {
        return Poco::Logger::get("SubutaiLogger");
    }

    bool Session::isTerminating()
    {
        return _terminate;
    }

    void Session::terminate()
    {
        _terminate = true;
    }

    void Session::start()
    {
        _terminate = false;
    }

    SSH* Session::makeSSHSession(const std::string& name, bool empty)
    {
        SSHSession s;
        s.name = name;
        s.session = new SSH();
        if (!empty) 
        {
            s.session->setHost(getSSHHostname(), getSSHPort());
            s.session->setUsername(getSSHUser(), getSSHPass());
            s.session->connect();
            s.session->authenticate();
            s.session->openChannel();
        }
        _sshSessions.push_back(s);
        return s.session;
    }

    SSH* Session::getSSHSession(const std::string& name)
    {
        for (auto it = _sshSessions.begin(); it != _sshSessions.end(); it++)
        {
            if ((*it).name == name)
            {
                return (*it).session;
            }
        }
        return nullptr;
    }

    void Session::finalizeSSHSession(const std::string& name)
    {
        for (auto it = _sshSessions.begin(); it != _sshSessions.end(); it++)
        {
            if ((*it).name == name)
            {
                (*it).session->closeChannel();
                (*it).session->disconnect();
                delete (*it).session;
                _sshSessions.erase(it);
                return;
            }
        }
    }

}
