#define WINVER 0x0601 // Allow use of features specific to Windows 7 or later.
#define _WIN32_WINNT 0x0601
#include "Environment.h"
#if LAUNCHER_WINDOWS
#include <Shlobj.h>
#include <VersionHelpers.h>
#include <Windows.h>
#pragma comment (lib, "Ntdll.lib")
#else
#include <cpuid.h>
#endif

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

    switch (si.wProcessorArchitecture) {
      /**/
      case PROCESSOR_ARCHITECTURE_AMD64:
        _logger->trace("x64");
        break;
      case PROCESSOR_ARCHITECTURE_ARM:
        _logger->trace("ARM");
        break;
      case PROCESSOR_ARCHITECTURE_IA64:
        _logger->trace("IA64 (intel itanium based) ");
        break;
      case PROCESSOR_ARCHITECTURE_INTEL:
        _logger->trace("x86");
        break;
      case PROCESSOR_ARCHITECTURE_UNKNOWN:
      default:
        _logger->trace("Unknown arch");
    }

    return si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64; //kuku

    return 1;
#endif
    return 0;
}

#if LAUNCHER_WINDOWS
unsigned long long SubutaiLauncher::Environment::ramSize() 
#else
unsigned long SubutaiLauncher::Environment::ramSize() 
#endif
{
    _logger->debug("Environment: Retrieving RAM size");
#if LAUNCHER_LINUX
    struct sysinfo info;
    _logger->trace("Running sysinfo");
    int rc = sysinfo(&info);
    if (rc == 0)
    {
        _logger->debug("Total mem size: %lu", info.totalram);
        return info.totalram;
    }
    /* 
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
    */
#elif LAUNCHER_WINDOWS
    MEMORYSTATUSEX ms;
	ms.dwLength = sizeof(MEMORYSTATUSEX);
    GlobalMemoryStatusEx (&ms);
    _logger->debug("Total mem size: %Lu", ms.ullTotalPhys);
    return ms.ullTotalPhys;
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
#ifndef LAUNCHER_WINDOWS
  enum { eax = 0, ebx, ecx, edx};
  enum { cpuid_sig = 0, cpuid_pifb = 1 } ;
  uint32_t regs[4] = {0};

  if (__get_cpuid(cpuid_pifb, &regs[eax], &regs[ebx], &regs[ecx], &regs[edx])) {
    return regs[ecx] & 0x10;
  } else {
    return false;
  }
#else
  return IsProcessorFeaturePresent(PF_VIRT_FIRMWARE_ENABLED);
#endif
}

std::string SubutaiLauncher::Environment::versionOS() 
{
#ifndef LAUNCHER_WINDOWS
    _logger->trace("Environment: Getting operating system information");
    std::string os;
    //os = Poco::Environment::osDisplayName() + " " + Poco::Environment::osVersion();
	os = Poco::Environment::osDisplayName();
    return os;
#else
  char* version = "Can't detect version, sorry";

   do {
     if (IsWindowsXPOrGreater()) {
       version = "XP Or Greater";
     } else {
       break;
     }

     if (IsWindowsXPSP1OrGreater()) {
       version = "XPSP1 Or Greater";
     } else {
       break;
     }

     if (IsWindowsXPSP2OrGreater()) {
       version = "XPSP2 Or Greater";
     } else {
       break;
     }

     if (IsWindowsXPSP3OrGreater()) {
       version = "XPSP3 Or Greater";
     } else {
       break;
     }

     if (IsWindowsVistaOrGreater()) {
       version = "Vista Or Greater";
     } else {
       break;
     }

     if (IsWindowsVistaSP1OrGreater()) {
       version = "VistaSP1 Or Greater";
     } else {
       break;
     }

     if (IsWindowsVistaSP2OrGreater()) {
       version = "VistaSP2 Or Greater";
     } else {
       break;
     }

     if (IsWindows7OrGreater()) {
       version = "Windows7 Or Greater";
     } else {
       break;
     }

     if (IsWindows7SP1OrGreater()) {
       version = "Windows7SP1 Or Greater";
     } else {
       break;
     }

     if (IsWindows8OrGreater()) {
       version = "Windows8 Or Greater";
     } else {
       break;
     }

     if (IsWindows8Point1OrGreater()) {
       version = "Windows8.1 Or Greater";
     } else {
       break;
     }

     /*if (IsWindows10OrGreater())
     {
        version = "Windows10 Or Greater";
     } else {
       break;
     }*/

   } while (0);
  return std::string(version);
#endif
}

std::string SubutaiLauncher::Environment::cpuArch() 
{
#ifndef LAUNCHER_WINDOWS
    _logger->trace("Environment: Getting OS Architecture");
    std::string ar = Poco::Environment::osArchitecture();
    return ar;
#else
  SYSTEM_INFO si;
  GetNativeSystemInfo(&si);
  switch (si.wProcessorArchitecture) {
    /**/
    case PROCESSOR_ARCHITECTURE_AMD64:
      return "x64";
    case PROCESSOR_ARCHITECTURE_ARM:
      return "ARM";
    case PROCESSOR_ARCHITECTURE_IA64:
      return "IA64";
    case PROCESSOR_ARCHITECTURE_INTEL:
      return "x86";
    case PROCESSOR_ARCHITECTURE_UNKNOWN:
    default:
      return "Unknown arch";
  }
#endif
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

			std::string pGateway(pAdapter->GatewayList.IpAddress.String);
			std::string pIp(pAdapter->IpAddressList.IpAddress.String);

			if (!pIp.empty() && pIp != "0.0.0.0" && !pGateway.empty() && pGateway != "0.0.0.0")
			{
				_logger->debug("IP: %s Gateway: %s", pIp, pGateway);
				if (pAdapterInfo) ENV_FREE(pAdapterInfo);
				return pDescription;
			}

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
		startService(name);
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
		_logger->information("Service was uninstalled");
		return true;
	}

	return false;
}

bool SubutaiLauncher::Environment::startService(const std::string& name)
{
	_logger->information("Starting %s Windows Service", name);
	if (!isNSSMInstalled())
	{
		_logger->error("NSSM tool is not installed");
		return false;
	}
	_logger->debug("NSSM tool found");

	std::string pPath = Session::instance()->getSettings()->getInstallationPath() + "\\bin\\nssm.exe";
	Poco::Process::Args pArgs;
	pArgs.push_back("start");
	pArgs.push_back("\"" + name + "\"");
	
	_logger->trace("Executing NSSM Start: Name: %s ", name);
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
		_logger->error("Service start failed. Exit code: %d", exitCode);
	}
	else
	{
		_logger->information("Service was started");
		return true;
	}

	return false;
}

bool SubutaiLauncher::Environment::stopService(const std::string& name)
{
	_logger->information("Stopping %s Windows Service", name);
	if (!isNSSMInstalled())
	{
		_logger->error("NSSM tool is not installed");
		return false;
	}
	_logger->debug("NSSM tool found");

	std::string pPath = Session::instance()->getSettings()->getInstallationPath() + "\\bin\\nssm.exe";
	Poco::Process::Args pArgs;
	pArgs.push_back("stop");
	pArgs.push_back("\"" + name + "\"");

	_logger->trace("Executing NSSM Stop: Name: %s ", name);
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
		_logger->error("Service stop failed. Exit code: %d", exitCode);
	}
	else
	{
		_logger->information("Service was stopped");
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

	Poco::File pExistLink(pName);
	if (pExistLink.exists())
	{
		_logger->debug("Link file already exists localy. Removing it");
		pExistLink.remove();
	}

	// Determine desktop directory
	// Moving newly created link to desktop FOLDERID_Desktop
	std::string pDesktopPath = getDesktopDirectory();
	
	Poco::File pDesktopLink(pDesktopPath + "\\" + pName);
	if (pDesktopLink.exists())
	{
		_logger->debug("Link file exists. Removing it");
		pDesktopLink.remove();
	}

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
				
				_logger->debug("Moving link file (%s) to %s", std::string(pLinkFile), std::string(pDesktopPath));
				Poco::File f(pLinkFile);
				f.moveTo(std::string(pDesktopPath));
			}
			pShellLink->Release();
		}

	}
	CoUninitialize();
	_logger->trace("Environment::CreateShortcut ~");
#endif
}

int32_t SubutaiLauncher::Environment::updatePath(const std::string& path)
{
#if LAUNCHER_WINDOWS
	_logger->trace("Environment::updatePath");

	std::wstring name(path.begin(), path.end());
	const wchar_t* str_template = name.c_str();

	enum { BUFF_SIZE = 1024 * 4 };
	//int8_t* buffer = (int8_t*)malloc(BUFF_SIZE);
	LPBYTE buffer = (LPBYTE)malloc(BUFF_SIZE);

	DWORD buff_len = BUFF_SIZE;
	wchar_t *str_tmp;

	int32_t i, tmp_res, result;
	LPCWSTR key_path = L"SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment";
	HKEY root_key = HKEY_LOCAL_MACHINE;
	HKEY key;
	DWORD key_type;
	wchar_t* key_value = L"Path";

	result = ec_success;

	memset(buffer, 0, buff_len);
	setlocale(LC_ALL, ""); //todo remove

	do {
		if ((tmp_res = RegOpenKeyExW(root_key, key_path,
			0, KEY_ALL_ACCESS, &key)) != ERROR_SUCCESS) {
			printf("RegOpenKey %d ret %d failed with err : %d\r\n", i,
				tmp_res, GetLastError());
			result = ec_cant_open_reg_key;
			break;
		}

		tmp_res = RegQueryValueExW(key, key_value,
			NULL, &key_type,
			buffer, &buff_len);

		if (tmp_res != ERROR_SUCCESS) {
			printf("Failed to query value. %d -> %d\r\n", tmp_res, GetLastError());
			result = ec_cant_query_reg_value;
			break;
		}

		if (key_type != REG_SZ && key_type != REG_EXPAND_SZ) {
			result = ec_wrong_key_type;
			break;
		}

		if ((str_tmp = wcsstr((wchar_t*)buffer, str_template)) != NULL) {
			printf("found value : %ls\r\n", str_template);
			result = ec_existing_val;
			break;
		}

		if (wcslen(str_template) + buff_len + 2 * sizeof(wchar_t) > BUFF_SIZE) {
			//int8_t *tmp_buff = malloc(wcslen(str_template) + buff_len + 2 * sizeof(wchar_t));
			LPBYTE tmp_buff = (LPBYTE)malloc(wcslen(str_template) + buff_len + 2 * sizeof(wchar_t));
			memset(tmp_buff, 0, wcslen(str_template) + buff_len + 2 * sizeof(wchar_t));
			memcpy(tmp_buff, buffer, buff_len);
			free(buffer);
			buffer = tmp_buff;
			//      result = ec_need_to_realloc;
			//      break; //after realloc remove this break
		}

		buff_len -= sizeof(wchar_t);
		memcpy(&buffer[buff_len], L";", sizeof(wchar_t));
		buff_len += sizeof(wchar_t);
		memcpy(&buffer[buff_len], str_template,
			wcslen(str_template) * sizeof(wchar_t));
		buff_len += wcslen(str_template) * sizeof(wchar_t);
		printf("\r\n%.*ls\r\n", buff_len, buffer);
		tmp_res = RegSetValueExW(key, key_value, 0, key_type,
			buffer, buff_len);

		if (tmp_res != ERROR_SUCCESS) {
			printf("RegSetValueEx failed . res : %d, last error : %d \r\n",
				tmp_res, GetLastError());
			result = ec_cant_change_registry;
			break;
		}
	} while (0);

	RegCloseKey(key);

	free(buffer);
	return result;

	_logger->trace("Environment::updatePath ~");
	/*
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
	*/
#endif
}

bool SubutaiLauncher::Environment::killProcess(const std::string & name)
{
#if LAUNCHER_WINDOWS
	_logger->debug("Killing %s process", name);
	HANDLE hProcessSnap;
	HANDLE hProcess;
	PROCESSENTRY32 pe32;
	DWORD dwPriorityClass;

	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE)
	{
		_logger->error("Failed to take snapshot of all processes in the system");
		return(FALSE);
	}

	pe32.dwSize = sizeof(PROCESSENTRY32);

	if (!Process32First(hProcessSnap, &pe32))
	{
		_logger->error("Failed to retrieve information about the first process");
		CloseHandle(hProcessSnap);  
		return(FALSE);
	}

	_logger->trace("Iterating processes snapshot");
	do
	{
		std::string str(pe32.szExeFile);

		if (str == name) 
		{
			_logger->trace("Target process [%s] found", name);
			terminateWinProcess(pe32.th32ProcessID, 1);
		}
	} while (Process32Next(hProcessSnap, &pe32));


	CloseHandle(hProcessSnap);
	return(TRUE);
#endif
}

std::string SubutaiLauncher::Environment::getDesktopDirectory()
{
#if LAUNCHER_WINDOWS
	_logger->debug("Retrieving desktop directory");
	wchar_t* pDesktopPath = 0;
	
	HRESULT hr = SHGetKnownFolderPath(FOLDERID_Desktop, 0, NULL, &pDesktopPath);
	if (SUCCEEDED(hr))
	{
		std::wstring ws(pDesktopPath);
		std::string pResult(ws.begin(), ws.end());
		CoTaskMemFree(static_cast<void*>(pDesktopPath));
		return pResult;
	}
	_logger->error("Failed to retrieve desktop path");
	CoTaskMemFree(static_cast<void*>(pDesktopPath));
#endif
	return "";
}

#if LAUNCHER_WINDOWS
bool SubutaiLauncher::Environment::writeE2ERegistry(const std::string & name)
{
	char* e2e_id = "kpmiofpmlciacjblommkcinncmneeoaa";

	char* key_x86[] = { "Software", "Google", "Chrome", "Extensions", e2e_id, NULL };
	char* key_x64[] = { "Software", "Wow6432Node", "Google",
		"Chrome", "Extensions", e2e_id, NULL };
	char** keys[] = { key_x86, key_x64, NULL };
	int32_t i, j, err;

	HKEY root_key, sub_key;
	DWORD disposition;
	const char* property = "update_url";
	const char* property_val = "https://clients2.google.com/service/update2/crx";

	setlocale(LC_ALL, "");

	for (i = 0; keys[i]; ++i) {
		root_key = HKEY_LOCAL_MACHINE;
		for (j = 0; keys[i][j]; ++j) {
			if ((err = RegCreateKeyExA(root_key, keys[i][j],
				0, NULL,
				REG_OPTION_NON_VOLATILE,
				KEY_ALL_ACCESS, NULL,
				&sub_key, &disposition)) != ERROR_SUCCESS) {
				printf("RegCreateKeyEx failed. res : %d, le : %d\r\n", err,
					GetLastError());
				break;
			}

			if (disposition == REG_OPENED_EXISTING_KEY) {
				printf("existing key : %s\r\n", keys[i][j]);
			}

			RegCloseKey(root_key);
			root_key = sub_key;
		}

		err = RegSetValueExA(root_key, property,
			0, REG_SZ,
			(BYTE*)property_val,
			strlen(property_val)*sizeof(char));

		RegCloseKey(root_key);
		if (err != ERROR_SUCCESS) {
			printf("Can't set value. Err : %d, LE : %d\r\n",
				err, GetLastError());
			continue;
		}
	}
	return true;
	/*
	Poco::Util::WinRegistryKey tkey("HKEY_LOCAL_MACHINE\\Software\\Wow6432Node\\Google\\Chrome\\Extensions\\kpmiofpmlciacjblommkcinncmneeoaa", false, REG_OPTION_NON_VOLATILE);
	if (!tkey.exists())
	{
		_logger->error("Specified windows key doesn't exists");
		//tkey.key();
		return false;
	}

	tkey.setString("update_url", "https://clients2.google.com/service/update2/crx");

	return false;
	*/
}
#endif

#if LAUNCHER_WINDOWS
BOOL SubutaiLauncher::Environment::terminateWinProcess(DWORD dwProcessId, UINT uExitCode)
{
	_logger->information("Terminating process");
	DWORD dwDesiredAccess = PROCESS_TERMINATE;
	BOOL  bInheritHandle = FALSE;
	HANDLE hProcess = OpenProcess(dwDesiredAccess, bInheritHandle, dwProcessId);
	if (hProcess == NULL)
	{
		_logger->error("Couldn't open process");
		return FALSE;
	}

	BOOL result = TerminateProcess(hProcess, uExitCode);

	CloseHandle(hProcess);

	_logger->information("Process termination successful");
	return result;
}
#endif
