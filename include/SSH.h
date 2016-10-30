#ifndef __SSH_H__
#define __SSH_H__

#include <cstdlib>
#include <string>
#include <vector>
#include <io.h>
#include <libssh/libssh.h>
#include "Environment.h"
#include "SubutaiProcess.h"
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
            std::string run(const std::string& command) const;
            void connect();
            void authenticate();
            int verifyHost();
            void execute(const std::string& command);
            bool isConnected();
            bool isAuthenticated();

        private:
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
