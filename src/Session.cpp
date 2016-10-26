#include "Session.h"

namespace SubutaiLauncher 
{

    Session* Session::_instance = NULL;

    Session::Session() 
    {
        _settings = new Settings();
        _downloader = new Downloader();
        _downloader->setOutputDirectory(_settings->getTmpPath());
        _confManager = new ConfigurationManager(_downloader);
    }

    Session::~Session() 
    {

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

    Downloader* Session::getDownloader() 
    {
        return _downloader;
    }

    Settings* Session::getSettings() 
    {
        return _settings;
    }

    ConfigurationManager* Session::getConfManager() 
    {
        return _confManager;
    }
};
