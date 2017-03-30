#ifndef __CORE_H__
#define __CORE_H__

#include <iostream>
#include <vector>
#include <string>
#include <Python.h>
#include <curl/curl.h>

#include <Poco/Message.h>
#include <Poco/Channel.h>
#include <Poco/ConsoleChannel.h>
#include <Poco/FileChannel.h>
#include <Poco/SplitterChannel.h>
#include <Poco/FormattingChannel.h>
#include <Poco/PatternFormatter.h>
#include <Poco/Logger.h>
#include <Poco/LogStream.h>
#include <Poco/AutoPtr.h>

#include "SL.h"
#include "SLFunctions.h"
#include "Hub.h"
#include "VirtualBox.h"
#include "Session.h"
#include "Environment.h"
#include "SubutaiLog.h"
#include "Browser.h"

namespace SubutaiLauncher 
{

    class Core 
    {
        public:
            Core(std::vector<std::string> args);
            ~Core();
            void initializePython();
            void run();
            void parseArgs();
            void setupLogger();
        private:
            std::vector<std::string> _args;
            bool _running;
        protected:
            void handleTest();
    };

}

#endif
