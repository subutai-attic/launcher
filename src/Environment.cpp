#include "Environment.h"

#if LAUNCHER_MACOS
const std::string SubutaiLauncher::Environment::EXTRA_PATH="/usr/local/bin:";
#endif

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

bool SubutaiLauncher::Environment::vtxEnabled() 
{
#if LAUNCHER_LINUX
    _logger->trace("Environment: Checking VT-x support");

    Poco::Process::Args args;
    
    Poco::Pipe pOut;

    Poco::ProcessHandle ph = Poco::Process::launch("/usr/bin/lscpu", args, 0, &pOut, 0);
    ph.wait();

    Poco::PipeInputStream istr(pOut);
    std::string buffer;
    Poco::StreamCopier::copyToString(istr, buffer);

    size_t vmx = buffer.find("vmx");
    size_t virt = buffer.find("Virtualization");
    if (vmx != std::string::npos && virt != std::string::npos) 
    {
        return true;
    }
    return false;

#elif LAUNCHER_WINDOWS
    return false;
#elif LAUNCHER_MACOS
    return true;
#endif
}

std::string SubutaiLauncher::Environment::versionOS() 
{
    _logger->trace("Environment: Getting operating system information");
    std::string os;
    os = Poco::Environment::osDisplayName() + " " + Poco::Environment::osVersion();
    return os;
}

std::string SubutaiLauncher::Environment::cpuArch() 
{
    _logger->trace("Environment: Getting OS Architecture");
    std::string ar = Poco::Environment::osArchitecture();
    return ar;
}

unsigned int SubutaiLauncher::Environment::cpuNum() 
{
    return Poco::Environment::processorCount();
}

std::string SubutaiLauncher::Environment::getVar(const std::string& name, const std::string& defaultValue) 
{
    return Poco::Environment::get(name, defaultValue);
}

std::string SubutaiLauncher::Environment::setVar(const std::string& name, const std::string& value)
{
    Poco::Environment::set(name, value);
	return value;
}

std::string SubutaiLauncher::Environment::getDefaultGateway()
{
#if LAUNCHER_LINUX || LAUNCHER_MACOS
    std::string binary, gatewayName;
    int elnum;
#if LAUNCHER_LINUX
    binary = "/usr/bin/netstat";
    gatewayName = "0.0.0.0";
    elnum = 8;
#elif LAUNCHER_MACOS
    binary = "/usr/sbin/netstat";
    gatewayName = "default";
    elnum = 6;
#endif

    Poco::Process::Args args;
    args.push_back("-rn");
    
    Poco::Pipe pOut;
    Poco::ProcessHandle ph = Poco::Process::launch(binary, args, 0, &pOut, 0);
    ph.wait();

    Poco::PipeInputStream istr(pOut);
    std::string netstat;
    Poco::StreamCopier::copyToString(istr, netstat);

    Poco::StringTokenizer lines(netstat, "\n", Poco::StringTokenizer::TOK_TRIM | Poco::StringTokenizer::TOK_IGNORE_EMPTY);
    bool isDefault = false;
    for (auto line = lines.begin(); line != lines.end(); line++) {
        Poco::StringTokenizer elements((*line), " ", Poco::StringTokenizer::TOK_TRIM | Poco::StringTokenizer::TOK_IGNORE_EMPTY);
        int i = 0;
        for (auto el = elements.begin(); el != elements.end(); el++) {
            i++;
            if ((*el) == gatewayName) {
                isDefault = true;
            }
            if (isDefault && i == elnum) {
                return (*el);
            }
        }
    }
    return "unknown";
#else
	return "Not Implemented";
#endif
}
