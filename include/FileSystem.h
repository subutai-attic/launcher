#ifndef __FILE_SYSTEM_H__
#define __FILE_SYSTEM_H__

#include <string>
#include "Vars.h"

#include <Poco/File.h>

#include "SubutaiException.h"

namespace SubutaiLauncher {
    class FileSystem {
        public:
            static const std::string DELIM;
            FileSystem(const std::string& path = "/");
            ~FileSystem();
            void setPath(const std::string& path);
            bool isFileExists(const std::string& filename);
            void removeFile(const std::string& filename);
            void createDirectory(const std::string& dir);
            void copyFile(const std::string& src, const std::string& dst);
        private:
            std::string _path;
    };
}

#endif
