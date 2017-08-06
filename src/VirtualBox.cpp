#include "VirtualBox.h"

#if LAUNCHER_LINUX
const std::string SubutaiLauncher::VirtualBox::BIN = "vboxmanage";
#elif LAUNCHER_MACOS
const std::string SubutaiLauncher::VirtualBox::BIN = "VBoxManage";
#else
const std::string SubutaiLauncher::VirtualBox::BIN = "VBoxManage.exe";
#endif
std::string SubutaiLauncher::VirtualBox::cloneName = "subutai";
std::string SubutaiLauncher::VirtualBox::subutaiBranch = "subutai-dev";


SubutaiLauncher::VirtualBox::VirtualBox()
{
    _logger = &Poco::Logger::get("subutai");
    _logger->trace("Starting VirtualBox instance");
    _version = ""; 
}

SubutaiLauncher::VirtualBox::~VirtualBox() 
{
    _logger->trace("VirtualBox::~VirtualBox");
}

std::vector<SubutaiLauncher::SubutaiVM> SubutaiLauncher::VirtualBox::getPeers() 
{
	if (_path == "")
	{
		findInstallation();
	}
	_logger->trace("Retrieving list of peers");
    Poco::Process::Args args;
    args.push_back("list");
    args.push_back("vms");

	_logger->trace("Executing %s", _path);
    Poco::Pipe pOut;
    Poco::ProcessHandle ph = Poco::Process::launch(_path, args, 0, &pOut, 0);
    ph.wait();
    Poco::PipeInputStream istr(pOut);
    std::string buffer;
    Poco::StreamCopier::copyToString(istr, buffer);

    std::vector<SubutaiVM> peers = VirtualBox::parseVms(buffer);
    return peers;
}

bool SubutaiLauncher::VirtualBox::findInstallation()
{
    _logger->trace("Searching for VirtualBox installation in PATH");
    std::string path;
#if LAUNCHER_MACOS
    path.append(Environment::EXTRA_PATH);
#endif
#if LAUNCHER_LINUX || LAUNCHER_MACOS
    path.append(Poco::Environment::get("PATH", ""));
    Poco::StringTokenizer st(path, ":",
            Poco::StringTokenizer::TOK_IGNORE_EMPTY | Poco::StringTokenizer::TOK_TRIM);
#else
	std::vector<std::string> st;
	st.push_back(Poco::Environment::get("programfiles")+"\\Oracle\\VirtualBox");
	st.push_back(Poco::Environment::get("programfiles(x86)") + "\\Oracle\\VirtualBox");
	st.push_back(Poco::Environment::get("ProgramW6432") + "\\Oracle\\VirtualBox");
#endif
    for (auto it = st.begin(); it != st.end(); it++)
    {
		_logger->trace("Searching for %s in %s", BIN, (*it));
        std::string fp = (*it);
        fp.append("/"+BIN);
        Poco::File f(fp);
        if (f.exists()) 
		{
            _logger->trace("VirtualBox installation found at %s", fp);
            _installed = true;
            _path = (*it);
            _location = _path;
            _path.append(FileSystem::DELIM);
            _path.append(BIN);
            return true;
        }
    }
    _logger->trace("VirtualBox installation was not found");
    return false;
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

std::string SubutaiLauncher::VirtualBox::getBinaryLocation()
{
    return _path;
}

std::string SubutaiLauncher::VirtualBox::extractVersion()
{
    if (_version != "") 
	{
        return _version;
    }

    Poco::Process::Args args;
    args.push_back("-v");

    Poco::Pipe output;
    Poco::ProcessHandle ph = Poco::Process::launch(_path, args, 0, &output, 0);
    Poco::PipeInputStream istr(output);

    Poco::StreamCopier::copyToString(istr, _version);
    return _version;
}

void SubutaiLauncher::VirtualBox::getVms()
{
    Poco::Process::Args args;
    args.push_back("list");
    args.push_back("vms");

    Poco::Pipe output;
    Poco::ProcessHandle ph = Poco::Process::launch(_path, args, 0, &output, 0);
    Poco::PipeInputStream istr(output);

    std::string result;
    Poco::StreamCopier::copyToString(istr, result);
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
    for (auto it = lines.begin(); it != lines.end(); it++) 
	{
        if ((*it).empty() || (*it).length() < 10) continue;
        const char* line = const_cast<char*>((*it).c_str());
#if LAUNCHER_LINUX || LAUNCHER_MACOS
        sscanf(line, "\"%[^\"]\" {%s}", vmname, vmid);
#elif LAUNCHER_WINDOWS
        sscanf_s(line, "\"%s\" {%s}", vmname, bsize, vmid, bsize);
#endif
        SubutaiVM v;
        v.name = std::string(vmname);
        v.id = std::string(vmid);
        int i = v.name.find("subutai",0);
        if (i != std::string::npos)
		{
            vms.push_back(v);
        }
    }
    return vms;
}

std::string SubutaiLauncher::VirtualBox::execute(const std::string& command)
{
    if (_path.empty()) 
    {
        findInstallation();
    }
	
    _logger->information("VB: Executing '%s %s'", _path, command);
    Poco::Process::Args args;
    Poco::StringTokenizer st(command, " ", Poco::StringTokenizer::TOK_IGNORE_EMPTY | Poco::StringTokenizer::TOK_TRIM);

    for (auto it = st.begin(); it != st.end(); it++)
    {
        

		// Replacing +++ with spaces here to fix path issues
		std::string pCommand = Poco::replace((*it), "+++", " ");
		_logger->trace("Adding arguments %s [%s]", (*it), pCommand);
        args.push_back(pCommand);
    }

    Poco::Pipe output;
    Poco::Pipe error;
    Poco::ProcessHandle ph = Poco::Process::launch(_path, args, 0, &output, &error);
    int status = ph.wait();
    Poco::PipeInputStream iStdout(output);
    Poco::PipeInputStream iStderr(error);

    std::string pStdout;
    std::string pStderr;
    Poco::StreamCopier::copyToString(iStdout, pStdout);
    Poco::StreamCopier::copyToString(iStderr, pStderr);

    _logger->information("Command executed with status %d", status);

    if (!pStderr.empty()) 
	{
        _logger->error("Error during execution of a command %s: %s", command, pStderr);
    }
    return pStdout;
}

std::string SubutaiLauncher::VirtualBox::execute(const std::string& command, int &exitStatus)
{
    if (_path.empty()) 
    {
        findInstallation();
    }
    Poco::Process::Args args;
    Poco::StringTokenizer st(command, " ", Poco::StringTokenizer::TOK_IGNORE_EMPTY | Poco::StringTokenizer::TOK_TRIM);
    for (auto it = st.begin(); it != st.end(); it++)
    {
		// Replacing +++ with spaces here to fix path issues
		std::string pCommand = Poco::replace((*it), "+++", " ");
		_logger->trace("Adding arguments %s [%s]", (*it), pCommand);
		args.push_back(pCommand);
    }

    Poco::Pipe output;
    Poco::Pipe error;
    Poco::ProcessHandle ph = Poco::Process::launch(_path, args, 0, &output, &error);
    int status = ph.wait();
    Poco::PipeInputStream iStdout(output);
    Poco::PipeInputStream iStderr(error);

    std::string pStdout;
    std::string pStderr;
    Poco::StreamCopier::copyToString(iStdout, pStdout);
    Poco::StreamCopier::copyToString(iStderr, pStderr);

    exitStatus = status;
    _logger->information("Command executed with status %d", status);

    if (!pStderr.empty()) 
	{
        _logger->error("Error during execution of a command %s: %s", command, pStderr);
    }
    return pStdout;
}

std::string SubutaiLauncher::VirtualBox::getBridgedInterface(const std::string& iface) 
{
    //auto out = this->execute("list bridgedifs");
    return iface;
}

std::string SubutaiLauncher::VirtualBox::getMachineInfo(const std::string& name) 
{
    Poco::Process::Args args;
    args.push_back("showvminfo");
    args.push_back(name);

    Poco::Pipe pOut;
    Poco::ProcessHandle ph = Poco::Process::launch(_path, args, 0, &pOut, 0);
    ph.wait();
    Poco::PipeInputStream istr(pOut);
    std::string buffer;
    Poco::StreamCopier::copyToString(istr, buffer);
    return buffer;
}

bool SubutaiLauncher::VirtualBox::isMachineExists(const std::string& name)
{
	_logger->trace("Checking %s VM exists", name);
    auto list = getPeers();
    for (auto it = list.begin(); it != list.end(); it++) 
	{
        if ((*it).name == name) 
		{
			_logger->trace("%s exists", name);
            return true;
        }
    }
	_logger->trace("%s does not exists", name);
    return false;
}

bool SubutaiLauncher::VirtualBox::isMachineRunning(const std::string& name)
{
	_logger->trace("Checking %s VM running", name);
    auto list = getPeers();
    for (auto it = list.begin(); it != list.end(); it++) 
	{
		_logger->trace("Checking machine %s", (*it).name);
        if ((*it).name == name) 
		{
			_logger->trace("Machine %s has been found", name);
            auto info = getMachineInfo(name);
            Poco::StringTokenizer lines(info, "\n", Poco::StringTokenizer::TOK_TRIM | Poco::StringTokenizer::TOK_IGNORE_EMPTY);
            for (auto line = lines.begin(); line != lines.end(); line++) 
			{
                if ((*line).substr(0, 6) == "State:") 
				{
                    auto p = (*line).find("running", 0);
                    if (p != std::string::npos) 
					{
						_logger->trace("Machine %s is running", name);
                        return true;
                    } 
					_logger->trace("Machine %s is stopped", name);
                    return false;
                }
            }
        }
    }
	_logger->error("Machine %s has not been found", name);
    return false;
}

std::string SubutaiLauncher::VirtualBox::getHostOnlyAdapter()
{
	_logger->debug("Looking for Host-Only adapter");
	std::string result = execute("list hostonlyifs");

	Poco::StringTokenizer st(result, "\n", Poco::StringTokenizer::TOK_TRIM | Poco::StringTokenizer::TOK_IGNORE_EMPTY);
	for (auto it = st.begin(); it != st.end(); it++)
	{
		if ((*it).substr(0, 4) == "Name")
		{
			return Poco::trim((*it).substr(5, (*it).length()));
		}
	}

	return "undefined";
}
