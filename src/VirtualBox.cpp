#include "VirtualBox.h"

const std::string SubutaiLauncher::VirtualBox::BIN = "vboxmanage";
std::string SubutaiLauncher::VirtualBox::cloneName = "subutai";
std::string SubutaiLauncher::VirtualBox::subutaiBranch = "subutai-dev";


SubutaiLauncher::VirtualBox::VirtualBox()
{
    _logger = &Poco::Logger::get("subutai");
    _version = ""; 
}

SubutaiLauncher::VirtualBox::~VirtualBox() 
{

}

std::vector<SubutaiLauncher::SubutaiVM> SubutaiLauncher::VirtualBox::getPeers() 
{
    std::vector<std::string> args;
    args.push_back("list");
    args.push_back("vms");
    SubutaiProcess p;
    p.launch(BIN, args, _location);
    p.wait();
    auto out = p.getOutputBuffer();
    std::vector<SubutaiVM> peers = VirtualBox::parseVms(out);
    return peers;
}

bool SubutaiLauncher::VirtualBox::findInstallation()
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

std::string SubutaiLauncher::VirtualBox::extractVersion()
{
    if (_version != "") {
        return _version;
    }

    std::vector<std::string> args;
    args.push_back("-v");

    SubutaiProcess p;
    p.launch(BIN, args, _location);
    if (p.wait() == 0) {
        _version = p.getOutputBuffer();
        return _version;
    }
    return "";
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
        if (i != std::string::npos){
            vms.push_back(v);
        }
    }
    return vms;
}

std::string SubutaiLauncher::VirtualBox::execute(const std::string& command)
{
    SubutaiString str(command);
    std::vector<std::string> args;
    str.split(' ', args);
    SubutaiProcess p;
    p.launch(BIN, args, _location);
    p.wait();
    std::string out = p.getOutputBuffer();
    _logger->debug("VirtualBox::execute %s: %s", command, out);
    int i = 0;
    return out;
}

std::string SubutaiLauncher::VirtualBox::execute(const std::string& command, int &exitStatus)
{
    SubutaiString str(command);
    std::vector<std::string> args;
    str.split(' ', args);
    SubutaiProcess p;
    p.launch(BIN, args, _location);
    auto status = p.wait();
    exitStatus = status;
    std::string out = p.getOutputBuffer();
    std::string err = p.getErrorBuffer();
    if (status != 0) {
        _logger->error("VB Command execution gave error: %s", err);
    }
    _logger->debug("VirtualBox::execute %s[%d]: %s", command, status, out);
    int i = 0;
    return out;
}

std::string SubutaiLauncher::VirtualBox::getBridgedInterface(const std::string& iface) 
{
    auto out = this->execute("list bridgedifs");
    return iface;
}

std::string SubutaiLauncher::VirtualBox::getMachineInfo(const std::string& name) 
{
    std::vector<std::string> args;
    args.push_back("showvminfo");
    args.push_back(name);
    SubutaiProcess p;
    p.launch(BIN, args, _location);
    p.wait();
    return p.getOutputBuffer();
}

bool SubutaiLauncher::VirtualBox::isMachineExists(const std::string& name)
{
    auto list = getPeers();
    for (auto it = list.begin(); it != list.end(); it++) {
        if ((*it).name == name) {
            return true;
        }
    }
    return false;
}

bool SubutaiLauncher::VirtualBox::isMachineRunning(const std::string& name)
{
    auto list = getPeers();
    for (auto it = list.begin(); it != list.end(); it++) {
        if ((*it).name == name) {
            auto info = getMachineInfo(name);
            Poco::StringTokenizer lines(info, "\n", Poco::StringTokenizer::TOK_TRIM | Poco::StringTokenizer::TOK_IGNORE_EMPTY);
            for (auto line = lines.begin(); line != lines.end(); line++) {
                if ((*line).substr(0, 6) == "State:") {
                    auto p = (*line).find("running", 0);
                    if (p != std::string::npos) {
                        return true;
                    } 
                    return false;
                }
            }
        }
    }
    return false;
}
