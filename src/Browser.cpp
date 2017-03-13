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
    auto env = new Environment();
    SubutaiString str(env->getVar("PATH", ""));
    std::vector<std::string> path;
    str.split(':', path);
    FileSystem fs;
    for (auto it = path.begin(); it != path.end(); it++) {
    	fs.setPath((*it));
	if (fs.isFileExists(BIN)) {
	    _installed = true;
	    _path = (*it);
	    _location = _path;
	    _path.append(FileSystem::DELIM);
	    _path.append(BIN);
	    return true;
	}
    }
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
    return "";
}

std::string SubutaiLauncher::Browser::execute(const std::string& command)
{
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
}

std::string SubutaiLauncher::Browser::sysExecute(const std::string& command, const std::string& cargs)
{
    SubutaiString str(cargs);
    std::vector<std::string> args;
    str.split(' ', args);
    SubutaiProcess p;
    if (p.launch(command, args, "/usr/bin") < 0) {
	return "Error: can not start process";
    }
    if (p.wait() < 0) {
	return "Error: can not wait process";;
    }
    std::string out = p.getOutputBuffer();
    Log::instance()->logger()->debug() << "Browser::sysExecute " << command << " args: " << cargs << " output: " << out << std::endl;
    return out;
}

std::string SubutaiLauncher::Browser::sysExecute(const std::string& command, const std::vector<std::string>& args)
{
    SubutaiProcess p;
    if (p.launch(command, args, "/usr/bin") < 0) {
	return "Error: can not start process";
    }
    if (p.wait() < 0) {
	return "Error: can not wait process";
    }
    std::string out = p.getOutputBuffer();
    Log::instance()->logger()->debug() << "Browser::sysExecute " << command << " output: " << out << std::endl;
    return out;
}
