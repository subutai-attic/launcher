#include "EnvironmentImpl.h"
#include "Session.h"
#include <cpuid.h>

namespace SubutaiLauncher 
{

    const std::string EnvironmentImpl::EXTRA_PATH="/usr/local/bin:";

    EnvironmentImpl::EnvironmentImpl() 
    {
        _logger = &Poco::Logger::get("subutai");
        _logger->trace("Starting new Environment instance");
    }

    EnvironmentImpl::~EnvironmentImpl() 
    {
        _logger->trace("Environment::~Environment");
    }

    unsigned EnvironmentImpl::processorNum() 
    {
        _logger->trace("Environment: Get CPU number");
        return Poco::Environment::processorCount();
    }

    unsigned EnvironmentImpl::is64() 
    {
        _logger->trace("Environment: Determining architecture");
        return 1;
    }

    ULORAMSIZE_T EnvironmentImpl::ramSize() 
    {
        _logger->debug("Environment: Retrieving RAM size");
        struct sysinfo info;
        _logger->trace("Running sysinfo");
        int rc = sysinfo(&info);
        if (rc == 0)
        {
            _logger->debug("Total mem size: %lu", info.totalram);
            return info.totalram;
        }
        return 0;
    }

    unsigned EnvironmentImpl::versionVBox() 
    {
        return 1;
    }

    bool EnvironmentImpl::vtxEnabled() 
    {
        Poco::Process::Args pArgs;
        pArgs.push_back("/proc/cpuinfo");
        Poco::Pipe pOut;
        Poco::ProcessHandle ph = Poco::Process::launch("/bin/cat", pArgs, 0, &pOut, 0);
        int rc = ph.wait();
        if (rc != 0)
        {
            _logger->error("Failed to cat /proc/cpuinfo. VTX query failed");
            return false;
        }
        std::string pBuffer;
        Poco::PipeInputStream istr(pOut);
        Poco::StreamCopier::copyToString(istr, pBuffer);
        Poco::StringTokenizer lines(pBuffer, " ", Poco::StringTokenizer::TOK_TRIM | Poco::StringTokenizer::TOK_IGNORE_EMPTY);
        for (auto it = lines.begin(); it != lines.end(); it++)
        {
            if ((*it) == "vme" || (*it) == "VME") return true;
        }
        return false;
    }

    std::string EnvironmentImpl::versionOS() 
    {
        _logger->trace("Environment: Getting operating system information");
        std::string os;
        //os = Poco::Environment::osDisplayName() + " " + Poco::Environment::osVersion();
        os = Poco::Environment::osDisplayName();
        return os;
    }

    std::string EnvironmentImpl::versionNumber()
    {
        Poco::Process::Args pArgs;
        pArgs.push_back("/etc/lsb-release");
        Poco::Pipe pOut;
        Poco::ProcessHandle pH = Poco::Process::launch("/bin/cat", pArgs, 0, &pOut, 0);
        pH.wait();
        std::string pBuffer;
        Poco::PipeInputStream istr(pOut);
        Poco::StreamCopier::copyToString(istr, pBuffer);
        Poco::StringTokenizer lines(pBuffer, "\n", Poco::StringTokenizer::TOK_TRIM | Poco::StringTokenizer::TOK_IGNORE_EMPTY);
        for (auto it = lines.begin(); it != lines.end(); it++)
        {
            if ((*it).substr(0, 15) == "DISTRIB_RELEASE")
            {
                _logger->trace("Found DISTRIB_RELEASE: %s", (*it));
                std::string num = (*it).substr(16, 5);
                _logger->information("Extracted version: %s", num);
                return num;
            }
        }
        return "Unknown Version";
    }

    std::string EnvironmentImpl::cpuArch() 
    {
        _logger->trace("Environment: Getting OS Architecture");
        std::string ar("");
        ar = Poco::Environment::osArchitecture();
        return ar;
    }

    unsigned int EnvironmentImpl::cpuNum() 
    {
        return Poco::Environment::processorCount();
    }

    std::string EnvironmentImpl::getVar(const std::string& name, const std::string& defaultValue) 
    {
        return Poco::Environment::get(name, defaultValue);
    }

    std::string EnvironmentImpl::setVar(const std::string& name, const std::string& value)
    {
        Poco::Environment::set(name, value);
        return value;
    }

    std::string EnvironmentImpl::getDefaultGateway()
    {
        std::string binary, gatewayName;
        int elnum;
        binary = "/bin/netstat";
        gatewayName = "0.0.0.0";
        elnum = 8;

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
        for (auto line = lines.begin(); line != lines.end(); line++) 
        {
            Poco::StringTokenizer elements((*line), " ", Poco::StringTokenizer::TOK_TRIM | Poco::StringTokenizer::TOK_IGNORE_EMPTY);
            int i = 0;
            for (auto el = elements.begin(); el != elements.end(); el++) 
            {
                i++;
                if ((*el) == gatewayName) isDefault = true;
                if (isDefault && i == elnum) return (*el);
            }
        }
        return "unknown";
    }

    // Check whether NSSM tool is available
    bool EnvironmentImpl::isNSSMInstalled()
    {
        return false;
    }

    bool EnvironmentImpl::registerService(const std::string& name, const std::string& path, std::vector<std::string> args)
    {
        return false;
    }

    bool EnvironmentImpl::unregisterService(const std::string & name)
    {
        return false;
    }

    bool EnvironmentImpl::startService(const std::string& name)
    {
        return false;
    }

    bool EnvironmentImpl::stopService(const std::string& name)
    {
        return false;
    }

    void EnvironmentImpl::CreateShortcut(const std::string& source, const std::string& name)
    {

    }

    int32_t EnvironmentImpl::updatePath(const std::string& path)
    {
        return 0;
    }

    bool EnvironmentImpl::killProcess(const std::string & name)
    {
        return false;
    }

    std::string EnvironmentImpl::getDesktopDirectory()
    {
        return "";
    }

    bool EnvironmentImpl::isVBoxInstalled()
    {
        return false;
    }

    bool EnvironmentImpl::writeE2ERegistry(const std::string & name)
    {
        return false;
    }

    BOOL EnvironmentImpl::terminateWinProcess(DWORD dwProcessId, UINT uExitCode)
    {
        return false;
    }

    const std::string& EnvironmentImpl::getNetworkConfiguration() const
    {
        Poco::Process::Args pArgs;
        pArgs.push_back("addr");
        Poco::Pipe pOut;
        Poco::ProcessHandle ph = Poco::Process::launch("/usr/bin/ip", pArgs, 0, &pOut, 0);
        ph.wait();
        std::string pBuffer;
        Poco::PipeInputStream istr(pOut);
        Poco::StreamCopier::copyToString(istr, pBuffer);
        return pBuffer;
    }

    const std::string& EnvironmentImpl::getNetstat() const
    {
        Poco::Process::Args pArgs;
        pArgs.push_back("-rn");
        Poco::Pipe pOut;
        Poco::ProcessHandle ph = Poco::Process::launch("/bin/netstat", pArgs, 0, &pOut, 0);
        ph.wait();
        std::string pBuffer;
        Poco::PipeInputStream istr(pOut);
        Poco::StreamCopier::copyToString(istr, pBuffer);
        return pBuffer;
    }

    const std::string& EnvironmentImpl::getSystemInfo() const
    {
        return "";
    }

}
