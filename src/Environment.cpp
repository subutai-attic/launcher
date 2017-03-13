#include "Environment.h"

SubutaiLauncher::Environment::Environment() 
{

}

SubutaiLauncher::Environment::~Environment() 
{

}

unsigned SubutaiLauncher::Environment::processorNum() 
{

    auto l = Log::instance()->logger();
#if LAUNCHER_LINUX
#if defined _SC_NPROCESSORS_ONLN
	auto count = sysconf(_SC_NPROCESSORS_ONLN);
	//int count = sysconf(_SC_NPROCESSORS_CONF);
	if (count <= 0) count = 1;
	
	l->debug() << "Environment::processorNum: " << count << std::endl;

	//return static_cast<int>(count);
	return count;
#endif
#elif LAUNCHER_WINDOWS
	SYSTEM_INFO si;
	GetSystemInfo(&si);
	return si.dwNumberOfProcessors;
#elif LAUNCHER_MACOS

#endif
	return 1;
}

unsigned SubutaiLauncher::Environment::is64() 
{

    auto l = Log::instance()->logger();
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

unsigned long SubutaiLauncher::Environment::ramSize() 
{
    auto l = Log::instance()->logger();
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
	l->debug() << "Environment::ramSize: " << pages * pageSize / 1024 << std::endl;

	//return static_cast<int>(count);
	return pages * pageSize / 1024;
#endif
#endif
#elif LAUNCHER_WINDOWS
	SYSTEM_INFO si;
	GetSystemInfo(&si);
	return si.dwNumberOfProcessors;
#elif LAUNCHER_MACOS
#endif
	return 1;
}

unsigned SubutaiLauncher::Environment::versionVBox() 
{

    auto l = Log::instance()->logger();
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
    SubutaiProcess sp;
    std::string out;
    std::string err;
    std::vector<std::string> args;
    args.push_back("-c");
    args.push_back("lscpu|grep Virtualization:");
#if LAUNCHER_LINUX
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
#elif LAUNCHER_WINDOWS
	return "win"; //Change 4Win! 
#elif LAUNCHER_MACOS

#endif
	return out;
}

std::string SubutaiLauncher::Environment::versionOS() 
{
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
    auto l = Log::instance()->logger();

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
    l->debug() << "Environment::distroOS output " << out  << std::endl;
    return out;
#elif LAUNCHER_WINDOWS
    return std::string dis = Poco::Environment::osDisplayName();
#elif LAUNCHER_MACOS
#endif
    return "";
}



std::string SubutaiLauncher::Environment::cpuArch() 
{
    auto l = Log::instance()->logger();
    std::string ar = Poco::Environment::osArchitecture();
    //l->debug() << "SubutaiLAuncher::Environment: arch=" << ar << std::endl;
    return ar;
}

unsigned SubutaiLauncher::Environment::cpuNum() 
{
    auto l = Log::instance()->logger();
    unsigned pr = Poco::Environment::processorCount();
    //l->debug() << "SubutaiLAuncher::Environment: cpuNum=" << pr << std::endl;
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

    auto l = Log::instance()->logger();
    
#if LAUNCHER_LINUX
    sp.launch("ssh", args, "/usr/bin");
    if (sp.wait() == 0) 
    {
	out = sp.getOutputBuffer();
	err = sp.getErrorBuffer();
    }
    l->debug() << "SubutaiLauncher::Environment::versionSSH " << out  << std::endl;
    SubutaiString sstr(out);
    std::vector<std::string> splitted = sstr.ssplit("\n");
    l->debug() << "SubutaiLauncher::Environment::versionSSH " << out  << std::endl;
    out = splitted.front();
    l->debug() << "SubutaiLauncher::Environment::versionSSH " << out  << std::endl;

#elif LAUNCHER_WINDOWS
	return "win"; //Change 4Win! 
#elif LAUNCHER_MACOS

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