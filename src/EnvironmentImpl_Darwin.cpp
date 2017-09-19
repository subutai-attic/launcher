#include "EnvironmentImpl_Darwin.h"
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
        return 0;
    }

    ULORAMSIZE_T EnvironmentImpl::ramSize() 
    {
        _logger->debug("Environment: Retrieving RAM size");
        return 1;
    }

    unsigned EnvironmentImpl::versionVBox() 
    {
        return 0;
    }

    bool EnvironmentImpl::vtxEnabled() 
    {
        std::string pBuffer = getSystemInfo();
        Poco::StringTokenizer lines(pBuffer, " ", Poco::StringTokenizer::TOK_TRIM | Poco::StringTokenizer::TOK_IGNORE_EMPTY);
        for (auto it = lines.begin(); it != lines.end(); it++)
        {
            if ((*it) == "VMX") return true;
        }
        return false;
    }

    std::string EnvironmentImpl::versionOS() 
    {
        _logger->trace("Environment: Getting operating system information");
        std::string os;
        os = Poco::Environment::osDisplayName();
        return os;
    }

    std::string EnvironmentImpl::versionNumber()
    {
        Poco::Process::Args pArgs;
        pArgs.push_back("-productVersion");
        Poco::Pipe pOut;
        Poco::ProcessHandle pH = Poco::Process::launch("/usr/bin/sw_vers", pArgs, 0, &pOut, 0);
        pH.wait();
        std::string pBuffer;
        Poco::PipeInputStream istr(pOut);
        Poco::StreamCopier::copyToString(istr, pBuffer);
        return pBuffer;
    }

    std::string EnvironmentImpl::cpuArch() 
    {
        _logger->trace("Environment: Getting OS Architecture");
        std::string ar = Poco::Environment::osArchitecture();
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
        std::string gatewayName("default");
        int elnum = 6;
        std::string pNetstat = getNetstat();

        Poco::StringTokenizer lines(pNetstat, "\n", Poco::StringTokenizer::TOK_TRIM | Poco::StringTokenizer::TOK_IGNORE_EMPTY);
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
        Poco::Pipe pOut;
        Poco::ProcessHandle ph = Poco::Process::launch("/sbin/ifconfig", pArgs, 0, &pOut, 0);
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
        Poco::ProcessHandle ph = Poco::Process::launch("/usr/sbin/netstat", pArgs, 0, &pOut, 0);
        ph.wait();
        std::string pBuffer;
        Poco::PipeInputStream istr(pOut);
        Poco::StreamCopier::copyToString(istr, pBuffer);
        return pBuffer;
    }

    const std::string& EnvironmentImpl::getSystemInfo() const
    {
        Poco::Process::Args pArgs;
        pArgs.push_back("-a");
        Poco::Pipe pOut;
        Poco::ProcessHandle ph = Poco::Process::launch("/usr/sbin/sysctl", pArgs, 0, &pOut, 0);
        ph.wait();
        std::string pBuffer;
        Poco::PipeInputStream istr(pOut);
        Poco::StreamCopier::copyToString(istr, pBuffer);
        return pBuffer;
    }

}
