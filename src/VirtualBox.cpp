#include "VirtualBox.h"

const std::string SubutaiLauncher::VirtualBox::BIN = "vboxmanage";

SubutaiLauncher::VirtualBox::VirtualBox() : _version("") 
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
			break;
		}
	}
}

SubutaiLauncher::VirtualBox::~VirtualBox() 
{

}

bool SubutaiLauncher::VirtualBox::isInstalled() 
{
	return _installed;
}

bool SubutaiLauncher::VirtualBox::isRunning() 
{
	return true;
}

bool SubutaiLauncher::VirtualBox::isUpdateRequired() 
{
	return false;
}

std::string SubutaiLauncher::VirtualBox::retrieveVersion()
{
	if (_version != "") {
		return _version;
	}

}

void SubutaiLauncher::VirtualBox::getVms()
{
	std::vector<std::string> args;
	args.push_back("list");
	args.push_back("vms");
	SubutaiProcess p;
	p.launch(BIN, args, _location);
	p.wait();
	auto out = p.getOutputBuffer();
}

std::vector<SubutaiLauncher::SubutaiVM> SubutaiLauncher::VirtualBox::parseVms(const std::string& buffer)
{
	char vmname[150];
	char vmid[150];
	int bsize = 256;
	// Parsing output of vboxmanage:
	// "machine_name" {machine_id}
	std::vector<SubutaiVM> vms;
	SubutaiString buf(buffer);
	std::vector<std::string> lines;
	buf.split('\n', lines);
	for (auto it = lines.begin(); it != lines.end(); it++) {
		const char* line = const_cast<char*>((*it).c_str());
#if LAUNCHER_LINUX
		sscanf(line, "\"%s\" {%s}", vmname, vmid);
#elif LAUNCHER_WINDOWS
		sscanf_s(line, "\"%s\" {%s}", vmname, bsize, vmid, bsize);
#elif LAUNCHER_MACOS
#error Not Implemented on this platform
#endif
		SubutaiVM v;
		v.name = std::string(vmname);
		v.id = std::string(vmid);
		vms.push_back(v);
	}
	return vms;
}
