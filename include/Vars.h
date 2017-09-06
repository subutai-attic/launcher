#ifndef __VARS_H__
#define __VARS_H__

#define POCO_STATIC 1
#define LIBSSH_STATIC 1

#if defined(linux) || defined(__linux) || defined(__linux__) || defined(__TOS_LINUX__) || defined(EMSCRIPTEN)
#define LAUNCHER_LINUX 1
#define PATH_DELIM "/"
#elif defined(__APPLE__) || defined(__TOS_MACOS__)
#define LAUNCHER_MACOS 1
#define PATH_DELIM "/"
#elif defined(_WIN32) || defined(_WIN64)
#define LAUNCHER_WINDOWS 1
#define PATH_DELIM "\\"
#else
#define LAUNCHER_LINUX 1
#endif

#ifndef BUILD_SCHEME
#if BUILD_SCHEME_DEV
#define BUILD_SCHEME "dev"
#elif BUILD_SCHEME_MASTER
#define BUILD_SCHEME "master"
#else
#define BUILD_SCHEME "production"
#endif
#endif

#if defined(_WIN32) && defined(POCO_DLL)
    #if defined(Subutai_EXPORTS)
        #define Subutai_API __declspec(dllexport)
    #else
        #define Subutai_API __declspec(dllimport)
    #endif
#endif


#if !defined(Subutai_API)
    #if !defined(POCO_NO_GCC_API_ATTRIBUTE) && defined (__GNUC__) && (__GNUC__ >= 4)
        #define Subutai_API __attribute__ ((visibility ("default")))
    #else
        #define Subutai_API
    #endif
#endif

#if LAUNCHER_WINDOWS
#define WINVER 0x0601 // Allow use of features specific to Windows 7 or later.
#define _WIN32_WINNT 0x0601
#endif

// System information
#define SI_LAUNCHER_VERSION         "LauncherVersion"
#define SI_SYSTEM_INFO              "SystemInfo"
#define SI_CPU_INFO                 "CPUInfo"
#define SI_OS_NAME                  "OSName"
#define SI_CPU_ARCH                 "CPUArch"
#define SI_CORE_NUM                 "CoresNum"
#define SI_MEM_SIZE                 "MemSize"
#define SI_P2P_VERSION              "P2PVersion"
#define SI_TRAY_VERSION             "TrayVersion"
#define SI_E2E_VERSION              "E2EVersion"
#define SI_VBOX_VERSION             "VBoxVersion"
#define SI_COCOASUDO_INSTALLED      "cocoasudoInstalled"
#define SI_OSASCRIPT_INSTALLED      "osascriptInstalled"
#define SI_TUNTAP_INSTALLED         "tuntapInstalled"
#define SI_SSH_VERSION              "sshVersion"
#define SI_GKSU_INSTALLED           "gksuInstalled"
#define SI_NSSM_INSTALLED           "nssmInstalled"
#define SI_P2P_SERVICE_EXISTS       "P2PServiceExists"
#define SI_P2P_SERVICE_RUNNING      "P2PServiceRunning"
#define SI_IP                       "IPConfiguration"
#define SI_NETSTAT                  "NetStatConfiguration"
#define SI_DEFAULT_GATEWAY          "DefaultGateway"
#define SI_VMS                      "SubutaiMachines"


// Steps definitions
#define STEP_INIT                   "INIT"
#define STEP_P2P                    "P2P"
#define STEP_E2E                    "E2E"
#define STEP_TRAY                   "TRAY"
#define STEP_PEER                   "PEER"
#define STEP_RH                     "RH"

// Generic actions
#define ACT_INIT                    "INIT"      // Initialization 
#define ACT_DOWNLOAD                "DWL"       // Download
#define ACT_UNPACK                  "UNAR"      // Unarchive
#define ACT_INSTALL                 "INST"      // Installation using current OS installation method (ubuntu - dpkg, macos - installer, windows - .exe)
#define ACT_FILE_INFO               "FINF"      // Request remote file info
#define ACT_TEMPLATE_INFO           "TINF"      // Request remote template info

// VirtualBox related actions
#define VB_START                    "VSTA" 
#define VB_STOP                     "VSTO"
#define VB_MODIFY                   "VMOD"
#define VB_DHCP                     "VDHCP" 

#endif
