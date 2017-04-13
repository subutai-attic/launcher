#include "Vars.h"
#if LAUNCHER_WINDOWS
#include "../include/P2P.h"
#else
#include "P2P.h"
#endif

const std::string SubutaiLauncher::P2P::BIN = "p2p";

SubutaiLauncher::P2P::P2P()
{
    _version = "";
    _logger = &Poco::Logger::get("subutai");
}

SubutaiLauncher::P2P::~P2P()
{

}

bool SubutaiLauncher::P2P::findInstallation()
{
    _logger->debug("Searching for P2P installation");
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
        _logger->debug("P2P found in %s", _location);
	    return true;
	    }
	}
    _logger->debug("P2P was not found");
	return false;
}

std::string SubutaiLauncher::P2P::extractVersion()
{
    Poco::Process::Args args;
	args.push_back("version");
	args.push_back("-n");

    Poco::Pipe pOutPipe;
    Poco::ProcessHandle ph = Poco::Process::launch(_path, args, 0, &pOutPipe, 0);
    ph.wait();

    Poco::PipeInputStream istr(pOutPipe);

    Poco::StreamCopier::copyToString(istr, _version);
    return _version;
}

void SubutaiLauncher::P2P::start()
{

}

void SubutaiLauncher::P2P::stop()
{

}

bool SubutaiLauncher::P2P::isRunning()
{
	return false;
}

bool SubutaiLauncher::P2P::isInstalled()
{
    return _installed;
}

