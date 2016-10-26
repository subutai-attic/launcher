#include "FileSystem.h"

namespace SubutaiLauncher {

#if LAUNCHER_LINUX
    const std::string FileSystem::DELIM = "/";
#elif LAUNCHER_WINDOWS
#error Not Implemented for this platform
#elif LAUNCHER_MACOS
#error Not Implemented for this platform
#else
#error Unknown Platform
#endif
    
    FileSystem::FileSystem(const std::string& path) : _path(path) {

    }

    FileSystem::~FileSystem() {

    }

    void FileSystem::setPath(const std::string& path) {
        _path = path;
    }

    bool FileSystem::isFileExists(const std::string& filename) {
        std::string fullpath(_path);
        fullpath.append(DELIM);
        fullpath.append(filename);
#if LAUNCHER_LINUX
        struct stat st;
        return stat(fullpath.c_str(), &st) == 0;
#elif LAUNCHER_WINDOWS
#error Not Implemented for this platform
#elif LAUNCHER_MACOS
#error Not Implemented for this platform
#else
#error Unknown Platform
#endif

        return false;
    }

    void FileSystem::removeFile(const std::string& filename) {
        int rc;
        std::string fullpath(_path);
        fullpath.append(DELIM);
        fullpath.append(filename);
#if LAUNCHER_LINUX
        rc = unlink(fullpath.c_str());
#elif LAUNCHER_WINDOWS
#error Not Implemented for this platform
#elif LAUNCHER_MACOS
#error Not Implemented for this platform
#else
#error Unknown Platform
#endif
        if (rc != 0) {
            std::printf("Failed to remove %s\n", fullpath.c_str());
        }
    }

    void FileSystem::copyFile(const std::string& src, const std::string& dst)
    {
        std::ifstream s(src, std::ios::binary);
        std::ofstream d(dst, std::ios::binary);

        d << s.rdbuf();
    }

};
