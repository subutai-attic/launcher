#ifndef __SSH_H__
#define __SSH_H__

#include "Vars.h"

#include <cstdlib>
#include <string>
#include <vector>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "Poco/FileStream.h"
#include "Poco/StreamCopier.h"
#include "Poco/Logger.h"

#if LAUNCHER_WINDOWS
#include <io.h>
#endif 

#include "libssh/libssh.h"
#include "Environment.h"
#include "FileSystem.h"
#include "SubutaiString.h"

namespace SubutaiLauncher {
    class SSH {
        public:
            static const std::string BIN;
            SSH();
            ~SSH();
            bool findInstallation();
            void setUsername(const std::string& username, const std::string& password);
            void setHost(const std::string& host, long port = 22);
            //std::string run(const std::string& command) const;
            void connect();
            void disconnect();
            void authenticate();
            int verifyHost();
            std::string execute(const std::string& command);
            bool isConnected();
            bool isAuthenticated();
            static std::string getPublicKey();
        private:
            Poco::Logger* _logger;
            ssh_session _ssh;
            std::string _path;
            std::string _location;
            std::string _username;
            std::string _password;
            std::string _host;
            long _port;
            bool _installed;
            bool _connected;
            bool _authenticated;
    };
}

#endif
