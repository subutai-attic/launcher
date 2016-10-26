#ifndef __FILE_SYSTEM_H__
#define __FILE_SYSTEM_H__

#include <string>
#include <fstream>
#include "Vars.h"

#if LAUNCHER_LINUX
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#elif LAUNCHER_WINDOWS
#error Not Implemented for this platform
#elif LAUNCHER_MACOS
#error Not Implemented for this platform
#else
#error Unknown Platform
#endif

namespace SubutaiLauncher {
    class FileSystem {
        public:
            static const std::string DELIM;
            FileSystem(const std::string& path = "/");
            ~FileSystem();
            void setPath(const std::string& path);
            bool isFileExists(const std::string& filename);
            void removeFile(const std::string& filename);
            void copyFile(const std::string& src, const std::string& dst);
        private:
            std::string _path;
    };
};

#endif
