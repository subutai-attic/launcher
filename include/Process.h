#ifndef __PROCESS_H__
#define __PROCESS_H__

#include <iostream>
#include <string>
#include <vector>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>

#include "SubutaiException.h"

namespace SubutaiLauncher {
    class Process {
        public:
            Process();
            ~Process();
            void runBasic(const std::string& command, std::vector<std::string> args);
            pid_t launch(const std::string& cmd, std::vector<std::string> args, const std::string& dir = "");
            int wait();
            std::string getOutputBuffer();
            std::string getErrorBuffer();
        protected:
            void setupFds();
            void closeFds();
        private:
            int _inRead;
            int _inWrite;
            int _outRead;
            int _outWrite;
            int _errRead;
            int _errWrite;
            pid_t _pid;
    };
};

#endif