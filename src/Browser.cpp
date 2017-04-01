#include "Browser.h"

const std::string SubutaiLauncher::Browser::BIN = "google-chrome";

SubutaiLauncher::Browser::Browser()
{
     _version = ""; 
}

SubutaiLauncher::Browser::~Browser() 
{

}

bool SubutaiLauncher::Browser::findInstallation()
{
    
    return false;
}


bool SubutaiLauncher::Browser::isInstalled() 
{
    return _installed;
}

bool SubutaiLauncher::Browser::isRunning() 
{
    return true;
}

bool SubutaiLauncher::Browser::isUpdateRequired() 
{
    return false;
}

std::string SubutaiLauncher::Browser::extractVersion()
{
	return "";
	/*
    if (_version != "") {
        return _version;
    }

    std::vector<std::string> args;
    args.push_back("--version");

    SubutaiProcess p;
    p.launch(BIN, args, _location);
    if (p.wait() == 0) {
        _version = p.getOutputBuffer();
        return _version;
    }
    return "";*/
}

std::string SubutaiLauncher::Browser::execute(const std::string& command)
{
	return "";
	/*
    SubutaiString str(command);
    std::vector<std::string> args;
    str.split(' ', args);
    SubutaiProcess p;
    p.launch(BIN, args, _location);
    p.wait();
    std::string out = p.getOutputBuffer();
    Log::instance()->logger()->debug() << "Browser::execute " << command << ": "<< out << std::endl;
    int i = 0;
    return out;
	*/
}