#include "Settings.h"

namespace SubutaiLauncher 
{

    const std::string Settings::CONFIG_FILE = "lnchr.conf";
#if LAUNCHER_LINUX
    const std::string Settings::DEFAULT_PATH = "/opt/subutai/";
#elif LAUNCHER_WINDOWS
    const std::string Settings::DEFAULT_PATH = "C:\Subutai";
#elif LAUNCHER_MACOS
    const std::string Settings::DEFAULT_PATH = "/usr/local/share/subutai/";
#endif

    const std::string Settings::DEFAULT_TMP = "/tmp/subutai/";

    Settings::Settings() :
        _coreNum(2),
        _memSize(2)
    {
        _logger = &Poco::Logger::get("subutai");
        _installSettings.installP2P = false;
        _installSettings.installTray = false;
        _installSettings.installE2E = false;
        _installSettings.installPeer = false;
        _installSettings.cpuNum = 2;
        _installSettings.memSize = 2;
        setInstallationPath(getDefaultInstallationPath());
        setTmpPath(getDefaultTmpPath());
        return;
        FileSystem fs(".");
        if (!fs.isFileExists(CONFIG_FILE)) {
            setInstallationPath(DEFAULT_PATH);
            setTmpPath(DEFAULT_TMP);
            save();
        }
        else {
            load();
        }
    }

    Settings::~Settings() {

    }

    void Settings::load() {
        return;
        std::ifstream file(CONFIG_FILE);
        std::string data;
        file.seekg(0, std::ios::end);
        data.reserve(file.tellg());
        file.seekg(0, std::ios::beg);

        data.assign((std::istreambuf_iterator<char>(file)),
                std::istreambuf_iterator<char>());

        std::stringstream reader(data);
        std::string line;
        while (std::getline(reader, line)) {
            std::istringstream rline(line);
            std::string key;
            if (std::getline(rline, key, '=')) {
                std::string val;
                if (std::getline(rline, val)) {
                    if (key == "install_path") {
                        setInstallationPath(val);
                    }
                    else if (key == "tmp_path") {
                        setTmpPath(val);
                    }
                }
            }
        }
    }

    void Settings::save() {
        std::stringstream conf;
        conf << "install_path=" << getInstallationPath() << "\n";
        conf << "tmp_path=" << getTmpPath() << "\n";
        std::ofstream out(CONFIG_FILE, std::fstream::app);
        if (!out) {
            std::printf("Couldn't open file %s for writing\n", CONFIG_FILE.c_str());
        }
        else {
            out << conf.str();
            out.close();
        }
    }

    std::string Settings::getInstallationPath() const {
        return _installationPath;
    }

    std::string Settings::getTmpPath() const {
        return _tmpPath;
    }

    std::string Settings::getDefaultTmpPath() const
    {
#if LAUNCHER_LINUX || LAUNCHER_MACOS
        return DEFAULT_TMP;
#else
        std::string drive;
        try
        {
            drive = Poco::Environment::get("USERPROFILE");
            drive.append("\\subutai\\");
            drive.append("\\tmp\\");
        }
        catch (Poco::NotFoundException& e)
        {
            std::printf("Failed to extract home directory: %s\n", e.displayText());
        }
        return drive;
#endif
    }

    std::string Settings::getDefaultInstallationPath() const
    {
#if LAUNCHER_LINUX || LAUNCHER_MACOS
        return DEFAULT_PATH;
#else
        std::string drive;
        try
        {
            drive = Poco::Environment::get("USERPROFILE");
            drive.append("\\subutai\\");
        }
        catch (Poco::NotFoundException& e)
        {
            std::printf("Failed to extract home directory: %s\n", e.displayText());
        }
        return drive;
#endif
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

    int Settings::getCoreNum()
    {
        return _coreNum;
    }

    int Settings::getMemSize()
    {
        return _memSize;
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

    InstallationSettings Settings::getInstallationSettings()
    {
        return _installSettings;
    }

}

