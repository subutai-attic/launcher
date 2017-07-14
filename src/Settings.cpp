#include "Settings.h"

const std::string SubutaiLauncher::Settings::CONFIG_FILE = "lnchr.conf";
#if LAUNCHER_LINUX
const std::string SubutaiLauncher::Settings::DEFAULT_PATH = "/opt/subutai/";
#elif LAUNCHER_WINDOWS
const std::string SubutaiLauncher::Settings::DEFAULT_PATH = "C:\Subutai";
#elif LAUNCHER_MACOS
const std::string SubutaiLauncher::Settings::DEFAULT_PATH = "/usr/local/share/subutai/";
#endif

const std::string SubutaiLauncher::Settings::DEFAULT_TMP = "/tmp/subutai/";

SubutaiLauncher::Settings::Settings() :
	_coreNum(2),
	_memSize(2)
{
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

SubutaiLauncher::Settings::~Settings() {

}

void SubutaiLauncher::Settings::load() {
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

void SubutaiLauncher::Settings::save() {
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

std::string SubutaiLauncher::Settings::getInstallationPath() const {
    return _installationPath;
}

std::string SubutaiLauncher::Settings::getTmpPath() const {
    return _tmpPath;
}

std::string SubutaiLauncher::Settings::getDefaultTmpPath() const
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

std::string SubutaiLauncher::Settings::getDefaultInstallationPath() const
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

void SubutaiLauncher::Settings::setInstallationPath(const std::string& path) {
    _installationPath = path;
}

void SubutaiLauncher::Settings::setTmpPath(const std::string& path) {
    _tmpPath = path;
}

void SubutaiLauncher::Settings::setCoreNum(int num)
{
	_coreNum = num;
}

void SubutaiLauncher::Settings::setMemSize(int size)
{
	_memSize = size;
}

int SubutaiLauncher::Settings::getCoreNum()
{
	return _coreNum;
}

int SubutaiLauncher::Settings::getMemSize()
{
	return _memSize;
}
