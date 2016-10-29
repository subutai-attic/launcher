#include "Settings.h"

const std::string SubutaiLauncher::Settings::CONFIG_FILE = "lnchr.conf";
const std::string SubutaiLauncher::Settings::DEFAULT_PATH = "/opt/subutai";
const std::string SubutaiLauncher::Settings::DEFAULT_TMP = "/tmp";

SubutaiLauncher::Settings::Settings() {
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

void SubutaiLauncher::Settings::setInstallationPath(const std::string& path) {
	_installationPath = path;
}

void SubutaiLauncher::Settings::setTmpPath(const std::string& path) {
	_tmpPath = path;
}
