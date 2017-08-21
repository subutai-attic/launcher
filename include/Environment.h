#ifndef __ENVIRONMENT_H__
#define __ENVIRONMENT_H__

#include "Vars.h"

#include <string>
#include <cstdlib>
#include <sstream>
#if LAUNCHER_LINUX
#include <unistd.h>
#include <sys/sysinfo.h>
#endif
#include <limits.h>


#include "SubutaiException.h"
#include "SubutaiString.h"
#include "Poco/Environment.h"
#include "Poco/StringTokenizer.h"
#include "Poco/Process.h"
#include "Poco/Pipe.h"
#include "Poco/PipeStream.h"
#include "Poco/StreamCopier.h"
#include "Poco/Logger.h"
#include "Poco/File.h"
#include "Poco/Path.h"
#include "Session.h"

#if LAUNCHER_WINDOWS

#include <windows.h>
#include <VersionHelpers.h>
#include <WinSock2.h>
#include <shlobj.h>
#include <winnls.h>
#include <shobjidl.h>
#include <objbase.h>
#include <objidl.h>
#include <shlguid.h>
#include <tlhelp32.h>
#include <KnownFolders.h>
#include <wchar.h>

//#include <atlbase.h>
#include <IPHlpApi.h>
#pragma comment(lib, "IPHLPAPI.LIB")
#define ENV_MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
#define ENV_FREE(x) HeapFree(GetProcessHeap(), 0, (x))

#pragma comment(lib, "user32.lib")

#include "Poco/Util/WinRegistryKey.h"
#elif LAUNCHER_MACOS
#include <sys/sysctl.h>
#endif

namespace SubutaiLauncher {

	typedef enum error_codes {
		ec_success = 0,
		ec_wrong_key_type,
		ec_cant_open_reg_key,
		ec_cant_query_reg_value,
		ec_cant_change_registry,
		ec_need_to_realloc,
		ec_existing_val
	} error_codes_t;

    class Environment {
        public:
#if LAUNCHER_MACOS
            static const std::string EXTRA_PATH;
#endif
            Environment();
            ~Environment();
            std::string versionOS();
            std::string versionNumber();
            std::string cpuArch();
            unsigned int cpuNum();
            unsigned processorNum();
            unsigned is64();
#if LAUNCHER_WINDOWS
            unsigned long long ramSize();
#else
            unsigned long ramSize();
#endif
            unsigned versionVBox();
            bool vtxEnabled();
            std::string getVar(const std::string& name, const std::string& defaultValue);
            std::string setVar(const std::string& name, const std::string& value);
            std::string getDefaultGateway();
			// Windows only
			bool isNSSMInstalled();
			bool registerService(const std::string& name, const std::string& path, std::vector<std::string> args);
			bool unregisterService(const std::string& name);
			bool startService(const std::string& name);
			bool stopService(const std::string& name);
			void CreateShortcut(const std::string& source, const std::string& name);
			int32_t updatePath(const std::string& path);
			bool killProcess(const std::string& name);
			std::string getDesktopDirectory();
#if LAUNCHER_WINDOWS
			bool isVBoxInstalled();
			bool writeE2ERegistry(const std::string& name);
#endif
        private:
#if LAUNCHER_WINDOWS
			BOOL terminateWinProcess(DWORD dwProcessId, UINT uExitCode);
#endif
            Poco::Logger* _logger;
    };

}

#endif
