#include "FileSystem.h"

namespace SubutaiLauncher {

#if LAUNCHER_LINUX
    const std::string FileSystem::DELIM = "/";
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
#endif

        return false;
    }

};
