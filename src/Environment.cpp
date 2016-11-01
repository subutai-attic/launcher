#include "Environment.h"

SubutaiLauncher::Environment::Environment() 
{

}

SubutaiLauncher::Environment::~Environment() 
{

}

unsigned SubutaiLauncher::Environment::processorNum() 
{
#if LAUNCHER_LINUX
#if defined (_SC_NPROCESSORS_ONLN)
	auto count = sysconf(_SC_NPROCESSORS_ONLN);
	if (count <= 0) count = 1;
	return static_cast<int>(count);
#endif
#elif LAUNCHER_WINDOWS
	SYSTEM_INFO si;
	GetSystemInfo(&si);
	return si.dwNumberOfProcessors;
#elif LAUNCHER_MACOS

#endif
	return 1;
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
#error Not implemented on this platform
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