#include "SubutaiP2P.h"

const std::string SubutaiLauncher::P2P::BIN = "p2p";

SubutaiLauncher::P2P::P2P()
{
    _version = "";
}

SubutaiLauncher::P2P::~P2P()
{

}

bool SubutaiLauncher::P2P::findInstallation()
{
    auto l = Log::instance()->logger();
    //l->debug() << "Searching for P2P installation" << std::endl;
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
	    //l->debug() << "P2P found in " << _location << std::endl;
	    return true;
	    }
	}
	//l->debug() << "P2P was not found" << std::endl;
	return false;
}

std::string SubutaiLauncher::P2P::extractVersion()
{
//	if (_version != "") {
//		return _version;
//	}

	std::vector<std::string> args;
	args.push_back("version");
	args.push_back("-n");

	SubutaiProcess p;
	p.launch(BIN, args, _location);
	if (p.wait() == 0) {
	    _version = p.getOutputBuffer();
    	    size_t pos = _version.find_first_of("-");
    	    if (pos != std::string::npos) {
            _version = _version.substr(0, pos);
    	    }
	    return _version;
	}
	return "";
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

