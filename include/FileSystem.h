#ifndef __FILE_SYSTEM_H__
#define __FILE_SYSTEM_H__

#include <string>
#include "Vars.h"

#if LAUNCHER_LINUX
#include <sys/stat.h>
#include <sys/types.h>
#endif

namespace SubutaiLauncher {
    class FileSystem {
        public:
            static const std::string DELIM;
            FileSystem(const std::string& path = "/");
            ~FileSystem();
            void setPath(const std::string& path);
            bool isFileExists(const std::string& filename);
        private:
            std::string _path;
    };
};

#endif
