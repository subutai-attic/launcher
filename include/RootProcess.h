#ifndef __ROOT_PROCESS_H__
#define __ROOT_PROCESS_H__

#include <vector>
#include <string>

#include <Poco/UUID.h>
#include <Poco/UUIDGenerator.h>
#include <Poco/FileStream.h>
#include <Poco/File.h>
#include <Poco/Process.h>

namespace SubutaiLauncher {
    class RootProcess {
        public:
            RootProcess();
            ~RootProcess();
            void addCommand(const std::string& cmd);
            void execute(const std::string& msg);
        private:
            std::vector<std::string> _commands;
            std::string _filename;
    };
};

#endif
