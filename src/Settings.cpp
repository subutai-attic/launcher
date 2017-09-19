#include "Settings.h"

namespace SubutaiLauncher 
{

    const std::string Settings::CONFIG_FILE = "lnchr.conf";
#if LAUNCHER_LINUX
    const std::string Settings::DEFAULT_PATH = "/opt/subutai/";
    const std::string Settings::DEFAULT_TMP = "/tmp/subutai/";
#elif LAUNCHER_WINDOWS
    const std::string Settings::DEFAULT_PATH = "C:\\ProgramData\\subutai\\";
    const std::string Settings::DEFAULT_TMP = "C:\\ProgramData\\subutai\\tmp\\";
#elif LAUNCHER_MACOS
    const std::string Settings::DEFAULT_PATH = "/usr/local/share/subutai/";
    const std::string Settings::DEFAULT_TMP = "/tmp/subutai/";
#endif

    Settings::Settings() :
        _coreNum(2),
        _memSize(2)
    {
        _logger = &Poco::Logger::get("subutai");
        _installSettings.installP2P = false;
        _installSettings.installTray = false;
        _installSettings.installE2E = false;
        _installSettings.installPeer = false;
		_installSettings.installRh = false;
        _installSettings.cpuNum = 2;
        _installSettings.memSize = 2;
        setInstallationPath(getDefaultInstallationPath());
        setTmpPath(getDefaultTmpPath());
    }

    Settings::~Settings() {

    }

    std::string Settings::getInstallationPath() const {
        return _installationPath;
    }

    std::string Settings::getTmpPath() const {
        return _tmpPath;
    }

    std::string Settings::getDefaultTmpPath() const
    {
        return DEFAULT_TMP;
    }

    std::string Settings::getDefaultInstallationPath() const
    {
        return DEFAULT_PATH;
    }

    void Settings::setInstallationPath(const std::string& path) {
        _installationPath = path;
    }

    void Settings::setTmpPath(const std::string& path) {
        _tmpPath = path;
    }

    void Settings::setCoreNum(int num)
    {
        _coreNum = num;
    }

    void Settings::setMemSize(int size)
    {
        _memSize = size;
    }

    void Settings::setDiskSize(int size)
    {
        _diskSize = size;
    }

    int Settings::getCoreNum()
    {
        return _coreNum;
    }

    int Settings::getMemSize()
    {
        return _memSize;
    }

    int Settings::getDiskSize()
    {
        return _diskSize;
    }

    void Settings::setInstallationP2P(bool p2p)
    {
        if (p2p)
        {
            _logger->trace("Settings -> Activating P2P");
        }
        else
        {
            _logger->trace("Settings -> Deactivating P2P");
        }
        _installSettings.installP2P = p2p;
    }

    void Settings::setInstallationTray(bool tray)
    {
        if (tray)
        {
            _logger->trace("Settings -> Activating Tray");
        }
        else
        {
            _logger->trace("Settings -> Deactivating Tray");
        }
        _installSettings.installTray = tray;
    }

    void Settings::setInstallationE2E(bool e2e)
    {
        if (e2e)
        {
            _logger->trace("Settings -> Activating E2E");
        }
        else
        {
            _logger->trace("Settings -> Deactivating E2E");
        }
        _installSettings.installE2E = e2e;
    }


    void Settings::setInstallationPeer(bool peer)
    {
        if (peer)
        {
            _logger->trace("Settings -> Activating Peer");
        }
        else
        {
            _logger->trace("Settings -> Deactivating Peer");
        }
        _installSettings.installPeer = peer;
    }

    void Settings::setInstallationRH(bool rh)
    {
      _logger->trace(rh ? "Settings -> Activating RH" : "Settings -> Deactivating RH");
      _installSettings.installRh = rh;
    }

    void Settings::setInstallationCpuNum(int num)
    {
        _logger->trace("Settings -> Setting CPU to %d", num);
        _installSettings.cpuNum = num;
    }

    void Settings::setInstallationMemSize(int size)
    {
        _logger->trace("Settings -> Setting memory size ot %d", size);
        _installSettings.memSize = size;
    }

    void Settings::setInstallationDiskSize(int size)
    {
        _logger->trace("Settings -> Setting disk size ot %d", size);
        _installSettings.diskSize = size;
    }

    InstallationSettings Settings::getInstallationSettings()
    {
        return _installSettings;
    }

}

