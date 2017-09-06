#include "EnvironmentImpl.h"
#include "Session.h"
#include <Shlobj.h>
#include <VersionHelpers.h>
#include <Windows.h>
#pragma comment(lib, "Ntdll.lib")
#pragma comment(lib, "Shell32.lib")


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
    }

    ULORAMSIZE_T EnvironmentImpl::ramSize() 
    {
        _logger->debug("Environment: Retrieving RAM size");
        MEMORYSTATUSEX ms;
        ms.dwLength = sizeof(MEMORYSTATUSEX);
        GlobalMemoryStatusEx (&ms);
        _logger->debug("Total mem size: %Lu", ms.ullTotalPhys);
        return ms.ullTotalPhys;
    }

    unsigned EnvironmentImpl::versionVBox() 
    {
        return 0; //Change 4Win! 
    }

    bool EnvironmentImpl::vtxEnabled() 
    {
        return IsProcessorFeaturePresent(PF_VIRT_FIRMWARE_ENABLED);
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
        return "UNKNOWN";
    }

    std::string EnvironmentImpl::cpuArch() 
    {
        //_logger->trace("Environment: Getting OS Architecture");
        std::string ar = Poco::Environment::osArchitecture();
		//_logger->trace("Environment: Getting OS Architecture");
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
    }

    // Check whether NSSM tool is available
    bool EnvironmentImpl::isNSSMInstalled()
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

    bool EnvironmentImpl::registerService(const std::string& name, const std::string& path, std::vector<std::string> args)
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

    bool EnvironmentImpl::unregisterService(const std::string & name)
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

    bool EnvironmentImpl::startService(const std::string& name)
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

    bool EnvironmentImpl::stopService(const std::string& name)
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

    void EnvironmentImpl::CreateShortcut(const std::string& source, const std::string& name)
    {
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
    }

    int32_t EnvironmentImpl::updatePath(const std::string& path)
    {
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
    }

    bool EnvironmentImpl::killProcess(const std::string & name)
    {
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
    }

    std::string EnvironmentImpl::getDesktopDirectory()
    {
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
        return "";
    }

    bool EnvironmentImpl::isVBoxInstalled()
    {
        enum { BUFF_SIZE = 1024 * 4 };
        LPBYTE buffer = (LPBYTE)malloc(BUFF_SIZE);
        DWORD buff_len = BUFF_SIZE;

        int32_t result = 0;
        LPCWSTR key_path[] = { L"SOFTWARE\\Wow6432Node\\Oracle\\VirtualBox",
            L"SOFTWARE\\Oracle\\VirtualBox",
            NULL };
        LPCWSTR* tmp = key_path;

        HKEY root_key = HKEY_LOCAL_MACHINE;
        HKEY key;

        result = 0;
        memset(buffer, 0, buff_len);
        setlocale(LC_ALL, ""); //todo remove

        for (; *tmp; ++tmp) {
            printf("%ls\n", *tmp);
            if ((result = RegOpenKeyExW(root_key, *tmp, 0, KEY_READ | KEY_WOW64_64KEY, &key)) ==
                    ERROR_SUCCESS) {
                result = 0;
                break;
            }
            else {
                printf("%d %d\n", result, GetLastError());
            }
            RegCloseKey(key);
        }

        printf("%s\n", result ?
                "VBOx not found, babushka" :
                "VBox found, let's start riot");

        free(buffer);
        if (result == 0) return true;
        return false;
    }

    bool EnvironmentImpl::writeE2ERegistry(const std::string & name)
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
    }

    BOOL EnvironmentImpl::terminateWinProcess(DWORD dwProcessId, UINT uExitCode)
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

    const std::string& EnvironmentImpl::getNetworkConfiguration() const
    {
		_logger->trace("Getting network configuration");
        Poco::Process::Args pArgs;
        pArgs.push_back("/all");
        Poco::Pipe pOut;
        Poco::ProcessHandle ph = Poco::Process::launch("ipconfig", pArgs, 0, &pOut, 0);
        ph.wait();
        std::string pBuffer;
        Poco::PipeInputStream istr(pOut);
        Poco::StreamCopier::copyToString(istr, pBuffer);
		_logger->trace("ipconfig: %s", pBuffer);
        return pBuffer;
    }

    const std::string& EnvironmentImpl::getNetstat() const
    {
        return "";
    }

    const std::string& EnvironmentImpl::getSystemInfo() const
    {
        return "";
    }

}
