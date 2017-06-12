#include "Environment.h"

//USES_CONVERSION;

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
    //os = Poco::Environment::osDisplayName() + " " + Poco::Environment::osVersion();
	os = Poco::Environment::osDisplayName();
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
    binary = "/bin/netstat";
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
#else
	PIP_ADAPTER_INFO pAdapterInfo;
	PIP_ADAPTER_INFO pAdapter = NULL;
	DWORD pRet = 0;

	ULONG pOutBufLen = sizeof(IP_ADAPTER_INFO);

	pAdapterInfo = (IP_ADAPTER_INFO *)ENV_MALLOC(sizeof(IP_ADAPTER_INFO));
	if (pAdapterInfo == NULL)
	{
		_logger->error("Failed to allocate memory");
		return "unknown";
	}

	_logger->debug("Retrieving adapter info");

	if (GetAdaptersInfo(pAdapterInfo, &pOutBufLen) == ERROR_BUFFER_OVERFLOW)
	{
		if (pAdapterInfo) ENV_FREE(pAdapterInfo);
		pAdapterInfo = (IP_ADAPTER_INFO *)ENV_MALLOC(pOutBufLen);
		if (pAdapterInfo == NULL)
		{
			_logger->error("Failed to allocate memory for GetAdaptersInfo");
			return "unknown";
		}
	}
	
	_logger->trace("Memory allocated");

	if ((pRet = GetAdaptersInfo(pAdapterInfo, &pOutBufLen)) == NO_ERROR)
	{
		pAdapter = pAdapterInfo;
		while (pAdapter)
		{
			// TODO: Make sure we are returning default one here
			
			std::string pName = std::string(pAdapter->AdapterName);
			std::string pDescription = std::string(pAdapter->Description);
			
			_logger->information("Network adapter name: %s", pName);
			_logger->information("Adapter description: %s", std::string(pDescription));

			_logger->debug("Retrievin adapetr infor from windows registry");
			std::string baseKey = "HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\Network\\{4d36e972-e325-11ce-bfc1-08002be10318}";
			Poco::Util::WinRegistryKey key(baseKey, true);
			if (!key.exists())
			{
				_logger->error("Network registry key doesn't exists");
				continue;
			}
			std::string subkey = baseKey + "\\" + pName;
			Poco::Util::WinRegistryKey pSubKey(subkey, true);
			if (!pSubKey.exists())
			{
				_logger->error("Network registry subkey doesn't exists");
				continue;
			}
			Poco::Util::WinRegistryKey pConnectionKey(subkey + "\\Connection", true);
			if (!pConnectionKey.exists())
			{
				_logger->error("Network registry connection doesn't exists");
				continue;
			}

			std::string pNiceName = pConnectionKey.getString("Name");
			_logger->information("Adapter name: %s", pNiceName);

			if (pAdapterInfo) ENV_FREE(pAdapterInfo);
			return pDescription;

			// This is unreachiable due to previous TODO
			pAdapter = pAdapter->Next;
		}
	}
	else
	{
		_logger->error("Failed to retrieve adapter info: %lu", pRet);
	}
	
	if (pAdapterInfo) ENV_FREE(pAdapterInfo);
	return "unknown";
#endif
}

// Check whether NSSM tool is available
bool SubutaiLauncher::Environment::isNSSMInstalled()
{
	_logger->debug("Checking NSSM tool");
	std::string path = Session::instance()->getSettings()->getInstallationPath() + "\\bin\\nssm.exe";
	Poco::File f(path);
	if (f.exists())
	{
		_logger->trace("NSSM tool exists");
		return true;
	}
	_logger->trace("NSSM tool doesn't exists");
	return false;
}

bool SubutaiLauncher::Environment::registerService(const std::string& name, const std::string& path, std::vector<std::string> args)
{
	_logger->information("Registering %s as a Windows Service", path);
	if (!isNSSMInstalled())
	{
		_logger->error("NSSM tool is not installed");
		return false;
	}
	_logger->debug("NSSM tool found");
	
	std::string pPath = Session::instance()->getSettings()->getInstallationPath() + "\\bin\\nssm.exe";
	Poco::Process::Args pArgs;
	pArgs.push_back("install");
	pArgs.push_back("\""+name+"\"");
	Poco::Path pBinPath(path);
	pArgs.push_back("\"" + pBinPath.toString() + "\"");
	for (auto it = args.begin(); it != args.end(); it++)
	{
		pArgs.push_back("\"" + (*it) + "\"");
	}
	_logger->trace("Executing NSSM: Name: %s Path: %s", name, path);
	std::string cmdLine = "nssm.exe ";
	for (auto it = pArgs.begin(); it != pArgs.end(); it++)
	{
		cmdLine.append((*it) + " ");
	}
	_logger->debug("Running command: %s", cmdLine);
	Poco::ProcessHandle ph = Poco::Process::launch(pPath, pArgs, 0, 0, 0);
	int exitCode = ph.wait();
	_logger->trace("NSSM has been executed");
	if (exitCode != 0)
	{
		_logger->error("Service installation failed. Exit code: %d", exitCode);
	}
	else
	{
		_logger->information("Service was installed");
		return true;
	}

	return false;
}

bool SubutaiLauncher::Environment::unregisterService(const std::string & name)
{
	_logger->information("Unregistering %s Windows Service", name);
	if (!isNSSMInstalled())
	{
		_logger->error("NSSM tool is not installed");
		return false;
	}
	_logger->debug("NSSM tool found");

	std::string pPath = Session::instance()->getSettings()->getInstallationPath() + "\\bin\\nssm.exe";
	Poco::Process::Args pArgs;
	pArgs.push_back("remove");
	pArgs.push_back("\"" + name + "\"");
	pArgs.push_back("confirm");

	std::string cmdLine = "nssm.exe ";
	for (auto it = pArgs.begin(); it != pArgs.end(); it++)
	{
		cmdLine.append((*it) + " ");
	}
	_logger->debug("Running command: %s", cmdLine);
	Poco::ProcessHandle ph = Poco::Process::launch(pPath, pArgs, 0, 0, 0);
	int exitCode = ph.wait();
	_logger->trace("NSSM has been executed");
	if (exitCode != 0)
	{
		_logger->error("Service installation failed. Exit code: %d", exitCode);
	}
	else
	{
		_logger->information("Service was installed");
		return true;
	}

	return false;
}

void SubutaiLauncher::Environment::CreateShortcut(const std::string& source, const std::string& name)
{
#if LAUNCHER_WINDOWS
	_logger->trace("Environment::CreateShortcut");
	Poco::Path pPath(source);
	std::string pName = name;

	_logger->debug("Shortcut for %s located in %s with name %s", pPath.getFileName(), pPath.parent().toString(), pName.append(".lnk"));

	std::string pLinkFile = std::string(pName).append("");

	//LPCSTR pszTargetfile = pPath.getFileName().c_str();
	LPCSTR pszTargetfile = source.c_str();
	LPCSTR pszTargetargs = "";
	LPCSTR pszLinkfile = pLinkFile.c_str();
	LPCSTR pszDescription = name.c_str();
	int iShowmode = SW_SHOW;
	//LPCSTR pszCurdir = pPath.parent().toString().c_str();
	LPCSTR pszCurdir = "";
	LPCSTR pszIconfile = "";
	int iIconindex = 0;
	HRESULT       hRes;                  /* Returned COM result code */
	IShellLink*   pShellLink;            /* IShellLink object pointer */
	IPersistFile* pPersistFile;          /* IPersistFile object pointer */
	WCHAR wszLinkfile[MAX_PATH];		 /* pszLinkfile as Unicode string */
	int           iWideCharsWritten;     /* Number of wide characters
										 written */
	CoInitialize(NULL);
	hRes = E_INVALIDARG;
	if (
		(pszTargetfile != NULL) && (strlen(pszTargetfile) > 0) &&
		(pszTargetargs != NULL) &&
		(pszLinkfile != NULL) && (strlen(pszLinkfile) > 0) &&
		(pszDescription != NULL) &&
		(iShowmode >= 0) &&
		(pszCurdir != NULL) &&
		(pszIconfile != NULL) &&
		(iIconindex >= 0)
		)
	{
		hRes = CoCreateInstance(
			CLSID_ShellLink,     /* pre-defined CLSID of the IShellLink
								 object */
			NULL,                 /* pointer to parent interface if part of
								  aggregate */
			CLSCTX_INPROC_SERVER, /* caller and called code are in same
								  process */
			IID_IShellLink,      /* pre-defined interface of the
								 IShellLink object */
			(LPVOID*)&pShellLink);         /* Returns a pointer to the IShellLink
										   object */
		if (SUCCEEDED(hRes))
		{
			/* Set the fields in the IShellLink object */
			hRes = pShellLink->SetPath(pszTargetfile);
			hRes = pShellLink->SetArguments(pszTargetargs);
			if (strlen(pszDescription) > 0)
			{
				hRes = pShellLink->SetDescription(pszDescription);
			}
			if (iShowmode > 0)
			{
				hRes = pShellLink->SetShowCmd(iShowmode);
			}
			if (strlen(pszCurdir) > 0)
			{
				hRes = pShellLink->SetWorkingDirectory(pszCurdir);
			}
			if (strlen(pszIconfile) > 0 && iIconindex >= 0)
			{
				hRes = pShellLink->SetIconLocation(pszIconfile, iIconindex);
			}

			/* Use the IPersistFile object to save the shell link */
			hRes = pShellLink->QueryInterface(
				IID_IPersistFile,         /* pre-defined interface of the
										  IPersistFile object */
				(LPVOID*)&pPersistFile);            /* returns a pointer to the
													IPersistFile object */
			if (SUCCEEDED(hRes))
			{
				iWideCharsWritten = MultiByteToWideChar(CP_ACP, 0,
					pszLinkfile, -1,
					wszLinkfile, MAX_PATH);
				hRes = pPersistFile->Save(wszLinkfile, TRUE);
				pPersistFile->Release();

				// Moving newly created link to desktop FOLDERID_Desktop
				PTCHAR filePath;
				if (FAILED(SHGetFolderPath(NULL,
					CSIDL_DESKTOPDIRECTORY | CSIDL_FLAG_CREATE,
					NULL,
					SHGFP_TYPE_CURRENT,
					filePath))) // Store the path of the desktop in filePath.
				{
					_logger->error("Failed to find Desktop directory");
				}
				else
				{
					Poco::File f(pLinkFile);
					f.moveTo(std::string(filePath));
				}
			}
			pShellLink->Release();
		}

	}
	CoUninitialize();
	_logger->trace("Environment::CreateShortcut ~");
#endif
}

void SubutaiLauncher::Environment::updatePath()
{
	std::string pPath = Poco::Environment::get("PATH");
	_logger->debug("PATH: %s", pPath);
	Poco::StringTokenizer st(pPath, ";", Poco::StringTokenizer::TOK_TRIM | Poco::StringTokenizer::TOK_IGNORE_EMPTY);
	std::string pBin = Session::instance()->getSettings()->getInstallationPath();
	Poco::Path pBinPath(pBin);
	pBinPath.append("/bin");

	bool bFound = false;

	for (auto it = st.begin(); it != st.end(); it++)
	{
		_logger->trace("Probing %s", (*it));
		if ((*it) == pBinPath.toString())
		{
			bFound = false;
			break;
		}
	}

	if (!bFound)
	{
		_logger->information("Updating PATH environment variable to %s", pBinPath.toString());
		pPath.append(";");
		pPath.append(pBinPath.toString());
		try 
		{
			Poco::Environment::set("PATH", pPath);
		}
		catch (Poco::SystemException& e)
		{
			_logger->error("Failed to set PATH environment variable");
		}
		return;
	}
	_logger->information("PATH variable is up-to-date");
}

bool SubutaiLauncher::Environment::killProcess(const std::string & name)
{
#if LAUNCHER_WINDOWS
	HANDLE hProcessSnap;
	HANDLE hProcess;
	PROCESSENTRY32 pe32;
	DWORD dwPriorityClass;

	// Take a snapshot of all processes in the system.
	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE)
	{
		return(FALSE);
	}

	// Set the size of the structure before using it.
	pe32.dwSize = sizeof(PROCESSENTRY32);

	// Retrieve information about the first process,
	// and exit if unsuccessful
	if (!Process32First(hProcessSnap, &pe32))
	{
		CloseHandle(hProcessSnap);  // clean the snapshot object
		return(FALSE);
	}

	// Now walk the snapshot of processes 
	do
	{
		std::string str(pe32.szExeFile);

		if (str == name) // put the name of your process you want to kill
		{
			terminateWinProcess(pe32.th32ProcessID, 1);
		}
	} while (Process32Next(hProcessSnap, &pe32));

	CloseHandle(hProcessSnap);
	return(TRUE);
#endif
}

#if LAUNCHER_WINDOWS
BOOL SubutaiLauncher::Environment::terminateWinProcess(DWORD dwProcessId, UINT uExitCode)
{
	DWORD dwDesiredAccess = PROCESS_TERMINATE;
	BOOL  bInheritHandle = FALSE;
	HANDLE hProcess = OpenProcess(dwDesiredAccess, bInheritHandle, dwProcessId);
	if (hProcess == NULL)
		return FALSE;

	BOOL result = TerminateProcess(hProcess, uExitCode);

	CloseHandle(hProcess);

	return result;
}
#endif
