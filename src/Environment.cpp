#include "Environment.h"

SubutaiLauncher::Environment::Environment() 
{
    _logger = &Poco::Logger::get("subutai");
    _logger->trace("Starting new Environment instance");
}

SubutaiLauncher::Environment::~Environment() 
{
    _logger->trace("Environment::~Environment");
}

unsigned SubutaiLauncher::Environment::processorNum() 
{
    _logger->trace("Environment: Get CPU number");
    return Poco::Environment::processorCount();
}

unsigned SubutaiLauncher::Environment::is64() 
{
    _logger->trace("Environment: Determining architecture");
#if LAUNCHER_LINUX
#if ( __WORDSIZE == 64 )
#define BUILD_64 1
#endif 

#ifdef BUILD_64
    return 1;
#else 
    return 0;
#endif
#elif LAUNCHER_WINDOWS
    SYSTEM_INFO si;
    GetSystemInfo(&si);
    return 0; //Change 4Win! 
#elif LAUNCHER_MACOS
    return 1;
#endif
    return 0;
}

unsigned long SubutaiLauncher::Environment::ramSize() 
{
    _logger->trace("Environment: Retrieving RAM size");
#if LAUNCHER_LINUX
#if defined _SC_PHYS_PAGES
#if defined _SC_PAGESIZE
    int pages = sysconf(_SC_PHYS_PAGES);
    if (pages <= 0)  pages = 1;

    int pageSize = sysconf(_SC_PAGESIZE);
    if (pageSize <= 0) {
        pageSize = 1;
    } else {
        pageSize = pageSize / 1024;
    }

    //return static_cast<int>(count);
    return pages * pageSize / 1024;
#endif
#endif
#elif LAUNCHER_WINDOWS
    SYSTEM_INFO si;
    GetSystemInfo(&si);
    return si.dwNumberOfProcessors;
#elif LAUNCHER_MACOS
    int mib [] = { CTL_HW, HW_MEMSIZE };
    int64_t value = 0;
    size_t length = sizeof(value);

    if(-1 == sysctl(mib, 2, &value, &length, NULL, 0))
        Poco::Logger::get("subutai").error("Failed to determine RAM size");

    return value;
#endif
    return 1;
}

unsigned SubutaiLauncher::Environment::versionVBox() 
{

#if LAUNCHER_LINUX
#if ( __WORDSIZE == 64 )
#define BUILD_64 1
#endif 

#ifdef BUILD_64
    return 1;
#else 
    return 0;
#endif
#elif LAUNCHER_WINDOWS
    SYSTEM_INFO si;
    GetSystemInfo(&si);
    return 0; //Change 4Win! 
#elif LAUNCHER_MACOS

#endif
    return 0;
}

std::string SubutaiLauncher::Environment::vtxEnabled() 
{
    _logger->trace("Environment: Checking VT-x support");
#if LAUNCHER_LINUX
    SubutaiProcess sp;
    std::string out;
    std::string err;
    std::vector<std::string> args;
    args.push_back("-c");
    args.push_back("lscpu|grep Virtualization:");
    sp.launch("bash", args, "/usr/bin");
    if (sp.wait() == 0) 
    {
        out = sp.getOutputBuffer();
        err = sp.getErrorBuffer();
    }

    SubutaiString sstr(out);
    std::string::size_type n = 0;
    std::string s = "Virtualization:";
    out = sstr.remove("Virtualization:", "");

    SubutaiString sstr1(out);
    out = sstr1.remove(" ", "");
    return out;
#elif LAUNCHER_WINDOWS
    #error Not implemented on this platform
    return "win"; //Change 4Win! 
#elif LAUNCHER_MACOS
    return "VMX";
#endif
}

std::string SubutaiLauncher::Environment::versionOS() 
{
    _logger->trace("Environment: Getting operating system information");
    std::string os;
#if LAUNCHER_LINUX
    os = distroOS("-d");
    os.append(" ");
    os.append(distroOS("-c"));
    SubutaiString sstr(os);
    os = sstr.remove("\n", " ");
    return os;
#elif LAUNCHER_WINDOWS || LAUNCHER_MACOS
    os = Poco::Environment::osDisplayName() + " " + Poco::Environment::osVersion();
    return os;
#endif
    return "Can not define OS";
}

std::string SubutaiLauncher::Environment::distroOS(std::string ar) 
{
    _logger->trace("Environment: Getting OS distribution name");
    SubutaiProcess sp;
    std::string out;
    std::string err;
    std::vector<std::string> args;

#if LAUNCHER_LINUX
    args.push_back(ar);
    sp.launch("lsb_release", args, "/usr/bin");
    if (sp.wait() == 0) 
    {
        out = sp.getOutputBuffer();
        err = sp.getErrorBuffer();
    }
    int found = out.find("Error");
    if (found != std::string::npos) {
        sleep(5);
        out = "";
        err = "";
        sp.launch("lsb_release", args, "/usr/bin");
        if (sp.wait() == 0) 
        {
            out = sp.getOutputBuffer();
            err = sp.getErrorBuffer();
        }
    }
    SubutaiString sstr(out);
    std::vector<std::string> splitted = sstr.ssplit("\t");
    out = splitted.back();
    return out;
#elif LAUNCHER_WINDOWS
    return std::string dis = Poco::Environment::osDisplayName();
#elif LAUNCHER_MACOS
    return "MacOS";
#endif
    return "";
}



std::string SubutaiLauncher::Environment::cpuArch() 
{
    _logger->trace("Environment: Getting OS Architecture");
    std::string ar = Poco::Environment::osArchitecture();
    return ar;
}

unsigned SubutaiLauncher::Environment::cpuNum() 
{
    unsigned pr = Poco::Environment::processorCount();
    return pr;
}

std::string SubutaiLauncher::Environment::versionSSH() 
{
    SubutaiProcess sp;
    std::string out;
    std::string err;
    std::vector<std::string> args;
    //args.push_back("-c");
    //args.push_back("ssh");
    args.push_back("-v");
    args.push_back("localhost");

#if LAUNCHER_LINUX || LAUNCHER_MACOS
    sp.launch("ssh", args, "/usr/bin");
    if (sp.wait() == 0) 
    {
        out = sp.getOutputBuffer();
        err = sp.getErrorBuffer();
    }
    SubutaiString sstr(out);
    std::vector<std::string> splitted = sstr.ssplit("\n");
    out = splitted.front();
#elif LAUNCHER_WINDOWS
    return "win"; //Change 4Win! 
#endif
    return out;
}


std::string SubutaiLauncher::Environment::getVar(const std::string& name, const std::string& defaultValue) 
{
#if LAUNCHER_LINUX
    const char* value = getenv(name.c_str());
    if (value) {
        return std::string(value);
    }
#elif LAUNCHER_WINDOWS
    DWORD len = GetEnvironmentVariableA(name.c_str(), 0, 0);
    if (len == 0) throw SubutaiException("Environment variable was not found");
    char* buffer = new char[len];
    GetEnvironmentVariableA(name.c_str(), buffer, len);
    std::string res(buffer);
    delete[] buffer;
    return res;
#elif LAUNCHER_MACOS
    return Poco::Environment::get(name, defaultValue);
#endif
    return defaultValue;
}

std::string SubutaiLauncher::Environment::setVar(const std::string& name, const std::string& value)
{
#if LAUNCHER_WINDOWS
    if (SetEnvironmentVariableA(name.c_str(), value.c_str()) == 0)
    {
        throw SubutaiException("Failed to set environment variable");
    }
#else
    setenv(name.c_str(), value.c_str(), 1);
#endif
}

std::string SubutaiLauncher::Environment::getDefaultGateway()
{
#if LAUNCHER_LINUX
    std::vector<std::string> pargs;
    pargs.push_back("-rn");
    pargs.push_back("|");
    SubutaiProcess p;
    p.launch("/bin/netstat", pargs, "");
    p.wait();
    auto netstat = p.getOutputBuffer();

    Poco::StringTokenizer lines(netstat, "\n", Poco::StringTokenizer::TOK_TRIM | Poco::StringTokenizer::TOK_IGNORE_EMPTY);
    bool isDefault = false;
    for (auto line = lines.begin(); line != lines.end(); line++) {
        Poco::StringTokenizer elements((*line), " ", Poco::StringTokenizer::TOK_TRIM | Poco::StringTokenizer::TOK_IGNORE_EMPTY);
        int i = 0;
        for (auto el = elements.begin(); el != elements.end(); el++) {
            i++;
            if ((*el) == "0.0.0.0") {
                isDefault = true;
            }
            if (isDefault && i == 8) {
                return (*el);
            }
        }
    }
    return "unknown";


    //return p.getOutputBuffer();
#elif LAUNCHER_MACOS
    std::vector<std::string> pargs;
    pargs.push_back("-rn");
    pargs.push_back("|");
    SubutaiProcess p;
    p.launch("/bin/netstat", pargs, "");
    p.wait();
    auto netstat = p.getOutputBuffer();

    Poco::StringTokenizer lines(netstat, "\n", Poco::StringTokenizer::TOK_TRIM | Poco::StringTokenizer::TOK_IGNORE_EMPTY);
    bool isDefault = false;
    for (auto line = lines.begin(); line != lines.end(); line++) {
        Poco::StringTokenizer elements((*line), " ", Poco::StringTokenizer::TOK_TRIM | Poco::StringTokenizer::TOK_IGNORE_EMPTY);
        int i = 0;
        for (auto el = elements.begin(); el != elements.end(); el++) {
            i++;
            if ((*el) == "default") {
                isDefault = true;
            }
            if (isDefault && i == 6) {
                return (*el);
            }
        }
    }
    return "unknown";
#else
#error Not implemented on this platform
#endif
}
