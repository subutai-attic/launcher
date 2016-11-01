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
//#include <string.h>
#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <atlstr.h>
#include <vector>
#include <stdlib.h>
#include "winnls.h"
#include "shobjidl.h"
#include "objbase.h"
#include "objidl.h"
#include "shlguid.h"
#include <shlobj.h>
#elif LAUNCHER_MACOS
#error Not Implemented for this platform
#else
#error Unknown Platform
#endif

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
#if LAUNCHER_LINUX
            void copyFile(const std::string& src, const std::string& dst);
#elif LAUNCHER_WINDOWS
			void copyFile(LPCSTR in_file, LPCSTR out_file);
#elif LAUNCHER_MACOS

#endif
        private:
            std::string _path;
    };
}

#endif
