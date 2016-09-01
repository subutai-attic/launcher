#include "Settings.h"

namespace SubutaiLauncher {
    const std::string Settings::CONFIG_FILE = "lnchr.conf";
    const std::string Settings::DEFAULT_PATH = "/opt/subutai";
    const std::string Settings::DEFAULT_TMP = "/tmp";

    Settings::Settings() {
        FileSystem fs(".");
        if (!fs.isFileExists(CONFIG_FILE)) {
            setInstallationPath(DEFAULT_PATH);
            setTmpPath(DEFAULT_TMP);
            save();
        } else {
            load();
        }
    }

    Settings::~Settings() {

    }

    void Settings::load() {

    }

    void Settings::save() {

    }

    std::string Settings::getInstallationPath() const {
        return _installationPath;
    }

    std::string Settings::getTmpPath() const {
        return _tmpPath;
    }

    void Settings::setInstallationPath(const std::string& path) {
        _installationPath = path;
    }

    void Settings::setTmpPath(const std::string& path) {
        _tmpPath = path;
    }
};
