#ifndef __PROCESS_H__
#define __PROCESS_H__

#include "Vars.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <string.h>
#if LAUNCHER_LINUX
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#elif LAUNCHER_WINDOWS
#include <windows.h>
#endif
#include <errno.h>

#include "SubutaiException.h"



namespace SubutaiLauncher {
#ifdef LAUNCHER_WINDOWS
	typedef long pid_t;
#endif
    class SubutaiProcess {
        public:
			SubutaiProcess();
            ~SubutaiProcess();
            void runBasic(const std::string& command, std::vector<std::string> args);
            pid_t launch(const std::string& cmd, std::vector<std::string> args, const std::string& dir = "");
            int wait();
            std::string getOutputBuffer();
            std::string getErrorBuffer();
        protected:
            void setupFds();
            void closeFds();
        private:
#if LAUNCHER_LINUX
            int _inRead;
            int _inWrite;
            int _outRead;
            int _outWrite;
            int _errRead;
            int _errWrite;
#elif LAUNCHER_WINDOWS
			HANDLE _inRead;
			HANDLE _inWrite;
			HANDLE _outRead;
			HANDLE _outWrite;
			HANDLE _errRead;
			HANDLE _errWrite;
			HANDLE _process;
#elif LAUNCHER_MACOS
#error Not Implemented on this platform
#endif
            pid_t _pid;
    };
}

#endif
