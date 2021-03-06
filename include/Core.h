#ifndef __CORE_H__
#define __CORE_H__

#include "Vars.h"

#include <iostream>
#include <vector>
#include <string>
#include <Python.h>

#include "Poco/Message.h"
#include "Poco/Channel.h"
#include "Poco/ConsoleChannel.h"
#if LAUNCHER_WINDOWS
#include "Poco/WindowsConsoleChannel.h"
#endif
#include "Poco/FileChannel.h"
#include "Poco/SplitterChannel.h"
#include "Poco/FormattingChannel.h"
#include "Poco/PatternFormatter.h"
#include "Poco/Logger.h"
#include "Poco/LogStream.h"
#include "Poco/AutoPtr.h"
#include "Poco/Net/SSLManager.h"
#include "Poco/Net/AcceptCertificateHandler.h"
#include "Poco/Timestamp.h"
#include "Poco/DateTimeFormat.h"
#include "Poco/DateTimeFormatter.h"

#include "SL.h"
#include "SSH.h"
#include "SLFunctions.h"
#include "Hub.h"
#include "VirtualBox.h"
#include "Session.h"
#include "Environment.h"
#include "Browser.h"
#include "HubChannel.h"

namespace SubutaiLauncher 
{

    class Core 
    {
        public:
            Core(std::vector<std::string> args);
			Core();
            Core(const std::string& appName);
            ~Core();
            void initializePython();
            void initializeSSL();
            void initializeSSH();
            void run();
            void parseArgs();
            void setupLogger();
        private:
            std::vector<std::string> _args;
            std::string _appName;
            bool _running;
            bool _noValidate;
    };

}

#endif
