#ifndef __SSH_H__
#define __SSH_H__

#include "Vars.h"

#include <thread>
#include <cstdlib>
#include <string>
#include <vector>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "Poco/FileStream.h"
#include "Poco/StreamCopier.h"
#include "Poco/Logger.h"
#include "Poco/String.h"

#if LAUNCHER_WINDOWS
#include <io.h>
#endif 

#include <libssh/callbacks.h>
#include "libssh/libssh.h"
#include "FileSystem.h"
#include "SSHException.h"
#include "Environment.h"

namespace SubutaiLauncher {
    struct ssh_threads_callbacks_struct * ssh_threads_get_std_threads(void);

    typedef enum {
        E_NOERR = 0,
        E_CHAN_OPEN_FAILED,
        E_SESS_OPEN_FAILED,
        E_CMD_EXEC_FAILED,
        E_EMPTY_OUTPUT_CHAN,
        E_PTY_REQUEST_FAILED,
        E_PTY_SIZE_FAILED,
        E_SHELL_REQUEST_FAILED
    } SSHErrorCode;

    class SSH {
        public:
            static const std::string BIN;
            static void initialize();
            static void deinitialize();
            SSH(bool init = true);
            ~SSH();
            void authenticate();
            void closeChannel();
            void connect();
            void disconnect();
            void openShell();
            void closeShell();
            std::string execute(const std::string& command);
            std::string executeInShell(const std::string& command);
            std::thread executeInThread(const std::string& command);
            bool findInstallation();
            static std::string getPublicKey();
            bool isAuthenticated();
            bool isConnected();
            int openChannel();
            void setHost(const std::string& host, long port = 22);
            void setUsername(const std::string& username, const std::string& password);
            int verifyHost();
        private:
            Poco::Logger* _logger;
            ssh_session _ssh;
            ssh_key _ssh_key;
            std::string _path;
            std::string _location;
            std::string _username;
            std::string _password;
            std::string _host;
            long _port;
            bool _installed;
            bool _connected;
            bool _authenticated;
            bool _bChanOpen;
            bool _bShellOpen;
            ssh_channel _channel; 
    };
}

#endif
