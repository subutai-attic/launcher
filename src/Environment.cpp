#include "Environment.h"

const std::string SubutaiLauncher::Environment::EXTRA_PATH="/usr/local/bin:";

SubutaiLauncher::Environment::Environment() : 
    _impl(new EnvironmentImpl)
{

}

SubutaiLauncher::Environment::~Environment() 
{

}

unsigned SubutaiLauncher::Environment::processorNum() 
{
    return _impl->processorNum();
}

unsigned SubutaiLauncher::Environment::is64() 
{
    return _impl->is64();
}

ULORAMSIZE_T SubutaiLauncher::Environment::ramSize() 
{
    return _impl->ramSize();
}

unsigned SubutaiLauncher::Environment::versionVBox() 
{
    return _impl->versionVBox();
}

bool SubutaiLauncher::Environment::vtxEnabled() 
{
    return _impl->vtxEnabled();
}

std::string SubutaiLauncher::Environment::versionOS() 
{
    return _impl->versionOS();
}

std::string SubutaiLauncher::Environment::versionNumber()
{
    return _impl->versionNumber();
}

std::string SubutaiLauncher::Environment::cpuArch() 
{
    return _impl->cpuArch();
}

unsigned int SubutaiLauncher::Environment::cpuNum() 
{
    return _impl->cpuNum();
}

std::string SubutaiLauncher::Environment::getVar(const std::string& name, const std::string& defaultValue) 
{
    return _impl->getVar(name, defaultValue);
}

std::string SubutaiLauncher::Environment::setVar(const std::string& name, const std::string& value)
{
    return _impl->setVar(name, value);
}

std::string SubutaiLauncher::Environment::getDefaultGateway()
{
    return _impl->getDefaultGateway();
}

bool SubutaiLauncher::Environment::isNSSMInstalled()
{
    return _impl->isNSSMInstalled();
}

bool SubutaiLauncher::Environment::registerService(const std::string& name, const std::string& path, std::vector<std::string> args)
{
    return _impl->registerService(name, path, args);
}

bool SubutaiLauncher::Environment::unregisterService(const std::string & name)
{
    return _impl->unregisterService(name);
}

bool SubutaiLauncher::Environment::startService(const std::string& name)
{
    return _impl->startService(name);
}

bool SubutaiLauncher::Environment::stopService(const std::string& name)
{
    return _impl->stopService(name);
}

void SubutaiLauncher::Environment::CreateShortcut(const std::string& source, const std::string& name)
{
    return _impl->CreateShortcut(source, name);
}

int32_t SubutaiLauncher::Environment::updatePath(const std::string& path)
{
    return _impl->updatePath(path);
}

bool SubutaiLauncher::Environment::killProcess(const std::string & name)
{
    return _impl->killProcess(name);
}

std::string SubutaiLauncher::Environment::getDesktopDirectory()
{
    return _impl->getDesktopDirectory();
}

bool SubutaiLauncher::Environment::isVBoxInstalled()
{
    return _impl->isVBoxInstalled();
}

bool SubutaiLauncher::Environment::writeE2ERegistry(const std::string & name)
{
    return _impl->writeE2ERegistry(name);
}

BOOL SubutaiLauncher::Environment::terminateWinProcess(DWORD dwProcessId, UINT uExitCode)
{
    return _impl->terminateWinProcess(dwProcessId, uExitCode);
}
