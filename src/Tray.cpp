#include "Tray.h"

const std::string SubutaiLauncher::Tray::BIN = "SubutaiTray";

SubutaiLauncher::Tray::Tray()
{
	_version = "";
}

SubutaiLauncher::Tray::~Tray()
{

}

bool SubutaiLauncher::Tray::findInstallation()
{
    auto l = Log::instance()->logger();
    l->debug() << "Searching for Tray installation" << std::endl;
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
            l->debug() << "Tray found in " << _location << std::endl;
			return true;
		}
	}
    l->debug() << "Tray was not found" << std::endl;
	return false;
}

std::string SubutaiLauncher::Tray::extractVersion()
{
	if (_version != "") {
		return _version;
	}

	std::vector<std::string> args;
	args.push_back("-v");

	SubutaiProcess p;
	p.launch(BIN, args, _location);
	std::string res;
	std::vector<std::string> vres;
	if (p.wait() == 0) {
	    res = p.getOutputBuffer();
	    SubutaiString st(res);
	    vres = st.ssplit("\n");
	    _version = vres.back();
	    return _version;
	}
	return "";
}

bool SubutaiLauncher::Tray::isInstalled()
{
    return _installed;
}

