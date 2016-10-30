#include "FileSystem.h"


#if LAUNCHER_LINUX
const std::string SubutaiLauncher::FileSystem::DELIM = "/";
#elif LAUNCHER_WINDOWS
const std::string SubutaiLauncher::FileSystem::DELIM = "\\";
#elif LAUNCHER_MACOS
#error Not Implemented for this platform
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
#if LAUNCHER_LINUX
	struct stat st;
	return stat(fullpath.c_str(), &st) == 0;
#elif LAUNCHER_WINDOWS
	DWORD attr = GetFileAttributes(fullpath.c_str());
	if (attr == INVALID_FILE_ATTRIBUTES)
	{
		switch (GetLastError()) {
		case ERROR_FILE_NOT_FOUND:
		case ERROR_PATH_NOT_FOUND:
		case ERROR_NOT_READY:
		case ERROR_INVALID_DRIVE:
			return false;
		default:
			throw SubutaiException("Unknown windows file error");
		};
	}
	return true;
#elif LAUNCHER_MACOS
#error Not Implemented for this platform
#else
#error Unknown Platform
#endif

	return false;
}

void SubutaiLauncher::FileSystem::removeFile(const std::string& filename) {
	int rc;
	std::string fullpath(_path);
	fullpath.append(DELIM);
	fullpath.append(filename);
#if LAUNCHER_LINUX
	rc = unlink(fullpath.c_str());
#elif LAUNCHER_WINDOWS
	rc = _unlink(fullpath.c_str());
#elif LAUNCHER_MACOS
#error Not Implemented for this platform
#else
#error Unknown Platform
#endif
	if (rc != 0) {
		std::printf("Failed to remove %s\n", fullpath.c_str());
	}
}

#if LAUNCHER_LINUX
void SubutaiLauncher::FileSystem::copyFile(const std::string& src, const std::string& dst)
{
	std::ifstream s(src, std::ios::binary);
	std::ofstream d(dst, std::ios::binary);

	d << s.rdbuf();
}
#elif LAUNCHER_WINDOWS
void SubutaiLauncher::FileSystem::copyFile(LPCSTR in_file, LPCSTR out_file) {
	if (!CopyFile(in_file, out_file, true))
	{
		throw SubutaiException("Failed to copy file");
	}
}
#elif LAUNCHER_MACOS
#error Not Implemented for this platform
#endif