#include "Tray.h"

const std::string SubutaiLauncher::Tray::BIN = "SubutaiTray";

SubutaiLauncher::Tray::Tray()
{
    _version = "";
    _logger = &Poco::Logger::get("subutai");
}

SubutaiLauncher::Tray::~Tray()
{

}

bool SubutaiLauncher::Tray::findInstallation()
{
    _logger->debug("Searching for Tray installation");
    auto env = new Environment();
    SubutaiString pathVar(env->getVar("PATH", ""));
    std::vector<std::string> path;
    pathVar.split(':', path);
    FileSystem fs;
    for (auto it = path.begin(); it != path.end(); it++) {
	fs.setPath((*it));
	    if (fs.isFileExists(BIN)) {
		_installed = true;
		_path = (*it);
		_location = _path;
		_path.append(FileSystem::DELIM);
		_path.append(BIN);
        _logger->debug("Tray was found in %s", _location);
		return true;
	    }
	}
    _logger->debug("Tray was not found");
    return false;
}

std::string SubutaiLauncher::Tray::extractVersion()
{

    Poco::Process::Args args;
    args.push_back("-v");

    Poco::Pipe pOut;
    Poco::ProcessHandle ph = Poco::Process::launch(_path, args, 0, &pOut, 0);
    ph.wait();
    Poco::PipeInputStream istr(pOut);

    Poco::StreamCopier::copyToString(istr, _version);
    return _version;
}

bool SubutaiLauncher::Tray::isInstalled()
{
    return _installed;
}

