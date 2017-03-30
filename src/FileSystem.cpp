#include "FileSystem.h"


#if LAUNCHER_LINUX
const std::string SubutaiLauncher::FileSystem::DELIM = "/";
#elif LAUNCHER_WINDOWS
const std::string SubutaiLauncher::FileSystem::DELIM = "\\";
#elif LAUNCHER_MACOS
const std::string SubutaiLauncher::FileSystem::DELIM = "/";
#else
#error Unknown Platform
#endif

SubutaiLauncher::FileSystem::FileSystem(const std::string& path) : _path(path) {

}

SubutaiLauncher::FileSystem::~FileSystem() {

}

void SubutaiLauncher::FileSystem::setPath(const std::string& path) {
	_path = path;
}

bool SubutaiLauncher::FileSystem::isFileExists(const std::string& filename) {
	std::string fullpath(_path);
	fullpath.append(DELIM);
	fullpath.append(filename);
    Poco::File f(fullpath);
    return f.exists();
}

void SubutaiLauncher::FileSystem::removeFile(const std::string& filename) {
	std::string fullpath(_path);
	fullpath.append(DELIM);
	fullpath.append(filename);
    Poco::File f(fullpath);
    if (f.exists()) f.remove();
}

void SubutaiLauncher::FileSystem::createDirectory(const std::string& dir)
{
    std::string fullpath(_path);
	fullpath.append(DELIM);
	fullpath.append(dir);
    Poco::File f(fullpath);
    if (!f.exists()) {
        f.createDirectories();
    }
}

void SubutaiLauncher::FileSystem::copyFile(const std::string& src, const std::string& dst)
{
    std::string fullpath(_path);
	fullpath.append(DELIM);
	fullpath.append(src);
    Poco::File f(fullpath);
    f.copyTo(dst);
}
