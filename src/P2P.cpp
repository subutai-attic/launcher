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
			return true;
		}
	}
	return false;
}

std::string SubutaiLauncher::P2P::extractVersion()
{
	if (_version != "") {
		return _version;
	}

	std::vector<std::string> args;
	args.push_back("version");

	SubutaiProcess p;
	p.launch(BIN, args, _location);
	if (p.wait() == 0) {
		_version = p.getOutputBuffer();
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

