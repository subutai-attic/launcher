#include "Environment.h"

namespace SubutaiLauncher
{

    const std::string Environment::EXTRA_PATH="/usr/local/bin:";

    Environment::Environment() : 
        _impl(new EnvironmentImpl)
    {

    }

    Environment::~Environment() 
    {

    }

    unsigned Environment::processorNum() 
    {
        return _impl->processorNum();
    }

    unsigned Environment::is64() 
    {
        return _impl->is64();
    }

    ULORAMSIZE_T Environment::ramSize() 
    {
        return _impl->ramSize();
    }

    unsigned Environment::versionVBox() 
    {
        return _impl->versionVBox();
    }

    bool Environment::vtxEnabled() 
    {
        return _impl->vtxEnabled();
    }

    std::string Environment::versionOS() 
    {
        return _impl->versionOS();
    }

    std::string Environment::versionNumber()
    {
        return _impl->versionNumber();
    }

    std::string Environment::cpuArch() 
    {
        return _impl->cpuArch();
    }

    unsigned int Environment::cpuNum() 
    {
        return _impl->cpuNum();
    }

    std::string Environment::getVar(const std::string& name, const std::string& defaultValue) 
    {
        return _impl->getVar(name, defaultValue);
    }

    std::string Environment::setVar(const std::string& name, const std::string& value)
    {
        return _impl->setVar(name, value);
    }

    std::string Environment::getDefaultGateway()
    {
        return _impl->getDefaultGateway();
    }

    bool Environment::isNSSMInstalled()
    {
        return _impl->isNSSMInstalled();
    }

    bool Environment::registerService(const std::string& name, const std::string& path, std::vector<std::string> args)
    {
        return _impl->registerService(name, path, args);
    }

    bool Environment::unregisterService(const std::string & name)
    {
        return _impl->unregisterService(name);
    }

    bool Environment::startService(const std::string& name)
    {
        return _impl->startService(name);
    }

    bool Environment::stopService(const std::string& name)
    {
        return _impl->stopService(name);
    }

    void Environment::CreateShortcut(const std::string& source, const std::string& name)
    {
        return _impl->CreateShortcut(source, name);
    }

    int32_t Environment::updatePath(const std::string& path)
    {
        return _impl->updatePath(path);
    }

    bool Environment::killProcess(const std::string & name)
    {
        return _impl->killProcess(name);
    }

    std::string Environment::getDesktopDirectory()
    {
        return _impl->getDesktopDirectory();
    }

    bool Environment::isVBoxInstalled()
    {
        return _impl->isVBoxInstalled();
    }

    bool Environment::writeE2ERegistry(const std::string & name)
    {
        return _impl->writeE2ERegistry(name);
    }

    BOOL Environment::terminateWinProcess(DWORD dwProcessId, UINT uExitCode)
    {
        return _impl->terminateWinProcess(dwProcessId, uExitCode);
    }

    const std::string& Environment::getNetworkConfiguration() const
    {
        return _impl->getNetworkConfiguration();
    }

    const std::string& Environment::getNetstat() const
    {
        return _impl->getNetstat();
    }

    const std::string& Environment::getSystemInfo() const
    {
        return _impl->getSystemInfo();
    }

}
