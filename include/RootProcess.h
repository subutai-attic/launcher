#ifndef __ROOT_PROCESS_H__
#define __ROOT_PROCESS_H__

#include "Vars.h"

#include <vector>
#include <string>

#include <Poco/UUID.h>
#include <Poco/UUIDGenerator.h>
#include <Poco/FileStream.h>
#include <Poco/File.h>
#include <Poco/Process.h>
#include "Poco/Logger.h"

namespace SubutaiLauncher {
    class RootProcess {
        public:
            RootProcess();
            ~RootProcess();
            void addCommand(const std::string& cmd);
            void execute(const std::string& msg);
        private:
            Poco::Logger* _logger;
            std::vector<std::string> _commands;
            std::string _filename;
    };
};

#endif
