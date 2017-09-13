#ifndef __SETTINGS_H__
#define __SETTINGS_H__

#include "Vars.h"

#include <string>
#include <sstream>
#include <fstream>
#include <streambuf>

#include "Vars.h"
#include "FileSystem.h"
#include "Poco/Environment.h"
#include "Poco/Logger.h"

namespace SubutaiLauncher {

  struct InstallationSettings
  {
    bool installP2P;
    bool installTray;
    bool installE2E;
    bool installPeer;
    bool installRh;
    int cpuNum;
    int memSize;
    int diskSize;
  };

  class Settings {
  public:
    static const std::string CONFIG_FILE;
    static const std::string DEFAULT_PATH;
    static const std::string DEFAULT_TMP;
    Settings();
    ~Settings();
    std::string getInstallationPath() const;
    std::string getTmpPath() const;
    std::string getDefaultTmpPath() const;
    std::string getDefaultInstallationPath() const;
    void setInstallationPath(const std::string& path);
    void setTmpPath(const std::string& path);
    void setCoreNum(int num);
    void setMemSize(int size);
    void setDiskSize(int size);
    int getCoreNum();
    int getMemSize();
    int getDiskSize();

    void setInstallationP2P(bool p2p);
    void setInstallationTray(bool tray);
    void setInstallationE2E(bool e2e);
    void setInstallationPeer(bool peer);
    void setInstallationRH(bool rh);
    void setInstallationCpuNum(int num);
    void setInstallationMemSize(int mem);
    void setInstallationDiskSize(int size);
    InstallationSettings getInstallationSettings();
  private:
    InstallationSettings _installSettings;
    std::string _installationPath;
    std::string _tmpPath;
    int _coreNum;
    int _memSize;
    int _diskSize;
    Poco::Logger* _logger;
  };
}

#endif
