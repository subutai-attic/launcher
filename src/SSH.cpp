#include "SSH.h"
#define MAX_XFER_BUF_SIZE 16384

namespace SubutaiLauncher 
{

    const std::string SSH::BIN = "ssh";

    SSH::SSH() :
        _username("root"),
        _password(""),
        _host("127.0.0.1"),
        _port(22),
        _installed(false),
        _connected(false),
        _authenticated(false),
        _bChanOpen(false)
    {
        _logger = &Poco::Logger::get("subutai");
        _ssh = ssh_new();
        if (_ssh == NULL) {
            throw SubutaiException("Failed to start SSH session");
        }
    }

    SSH::~SSH()
    {
        _logger->debug("Destructing SSH instance");
        if (_connected) ssh_disconnect(_ssh);
        if (_ssh != NULL) {
            _logger->trace("Freeing SSH object");
            ssh_free(_ssh);
        }
        _logger->debug("SSH instance destructed");
    }

    void SSH::authenticate()
    {
        int rc = ssh_userauth_password(_ssh, NULL, _password.c_str());
        _logger->debug("SSH Authentication status: %d", rc);
        if (rc == SSH_AUTH_ERROR) {
            _authenticated = false;
            return;
        }
        _authenticated = true;
    }

    void SSH::closeChannel()
    {
        if (_bChanOpen)
        {
            _bChanOpen = false;
            ssh_channel_close(_channel);
            ssh_channel_send_eof(_channel);
            ssh_channel_free(_channel);
        }
    }

    void SSH::closeShell()
    {
        if (_bShellOpen) closeChannel();
    }

    void SSH::disconnect()
    {
        if (_connected) {
            _logger->debug("Disconnecting SSH");
            ssh_disconnect(_ssh);
            _connected = false;
        }
    }

    std::string SSH::execute(const std::string& command)
    {
        _logger->trace("Executing SSH command: %s", command);
        int rc;
        char buffer[1024];
        int nbytes;
        memset(buffer, 0, 1024);

        if (!_bChanOpen)
        {
            rc = openChannel();
            if (rc != E_NOERR)
            {
                _logger->error("Couldn't open SSH channel: %d", rc);
                return "";
            }
        }

        _logger->trace("Starting SSH command execution");
        rc = ssh_channel_request_exec(_channel, command.c_str());
        if (rc != SSH_OK) {
            closeChannel();
            throw SSHException("Failed to execute SSH command", SSHErrorCode::E_CMD_EXEC_FAILED);
        }

        _logger->trace("Reading from channel");
        nbytes = ssh_channel_read(_channel, buffer, sizeof(buffer), 0);

        std::string pBuffer("");

        while (nbytes > 0)
        {
            pBuffer.append(buffer);
            nbytes = ssh_channel_read(_channel, buffer, sizeof(buffer), 0);
        }

        if (nbytes < 0) 
        {
            closeChannel();
            throw SSHException("Output channel is empty", SSHErrorCode::E_EMPTY_OUTPUT_CHAN);
        }

        closeChannel();
        _logger->debug("SSH Execution output: %s", pBuffer);
        return Poco::trim(pBuffer);
    }

    std::string SSH::executeInShell(const std::string& command)
    {
        int nbytes;

        return "";
    }

    bool SSH::findInstallation()
    {
        auto env = new Environment();
        SubutaiLauncher::SubutaiString pathVar(env->getVar("PATH", ""));
        std::vector<std::string> path;
        pathVar.split(':', path);
        FileSystem fs;
        for (auto it = path.begin(); it != path.end(); it++) {
            fs.setPath((*it));
            if (fs.isFileExists(BIN)) {
                _installed = true;
                _path = (*it);
                _location = _path;
                _path.append(FileSystem::DELIM);
                _path.append(BIN);
                return true;
            }
        }
        return false;
    }

    void SSH::setUsername(const std::string& username, const std::string& password)
    {
        _username = username;
        _password = password;
    }

    void SSH::setHost(const std::string& host, long port)
    {
        _host = host;
        _port = port;
    }

    void SSH::connect()
    {
        int rc;
        ssh_options_set(_ssh, SSH_OPTIONS_HOST, _host.c_str());
        ssh_options_set(_ssh, SSH_OPTIONS_PORT, &_port);
        ssh_options_set(_ssh, SSH_OPTIONS_USER, _username.c_str());

        rc = ssh_connect(_ssh);

        _logger->debug("SSH connect status: %d", rc);
        if (rc != SSH_OK) {
            _connected = false;
            _logger->debug("SSH Connection failed");
            return;
        }
        _connected = true;
    }

    int SSH::verifyHost()
    {
        int state;
        size_t hlen;
        unsigned char* hash = NULL;
        int gpkh_res;

        state = ssh_is_server_known(_ssh);

        //    hlen = ssh_get_pubkey_hash(_ssh, &hash);
        if ((gpkh_res = ssh_get_publickey_hash(_ssh_key, SSH_PUBLICKEY_HASH_SHA1, &hash, &hlen)))
            return gpkh_res;


        if (hlen < 0) {
            return -1;
        }

        switch (state) {
            case SSH_SERVER_KNOWN_OK:
                break;

            case SSH_SERVER_KNOWN_CHANGED:
                break;

            case SSH_SERVER_FOUND_OTHER:
                free(hash);
                return -1;
                break;

            case SSH_SERVER_FILE_NOT_FOUND:

            case SSH_SERVER_NOT_KNOWN:

                if (ssh_write_knownhost(_ssh) < 0) {
                    free(hash);
                    return -1;
                }
                break;

            case SSH_SERVER_ERROR:
                free(hash);
                return -1;
                break;
        };
        free(hash);
        return 0;
    }

    bool SSH::isConnected()
    {
        return _connected;
    }

    bool SSH::isAuthenticated()
    {
        return _authenticated;
    }

    std::string SSH::getPublicKey()
    {
        Environment env;
        auto var = env.getVar("HOME", "~");
        var.append("/.ssh/id_rsa.pub");
        try {
            Poco::FileInputStream fs(var);
            std::string buffer;
            Poco::StreamCopier::copyToString(fs, buffer);
            return buffer;
        }
        catch (Poco::FileNotFoundException e) {
            Poco::Logger::get("subutai").error("id_rsa.pub not found");
            return "";
        }
        catch (std::exception e) {
            Poco::Logger::get("subutai").error("Exception: %s", e.what());
            return "";
        }
    }

    int SSH::openChannel()
    {
        int rc;
        _logger->trace("Opening SSH channel");
        _channel = ssh_channel_new(_ssh);
        if (_channel == NULL)
        {
            _logger->error("Failed to open SSH channel");
            throw SSHException("Failed to open SSH channel", SSHErrorCode::E_CHAN_OPEN_FAILED);
        }

        rc = ssh_channel_open_session(_channel);
        if (rc != SSH_OK)
        {
            ssh_channel_close(_channel);
            ssh_channel_free(_channel);
            throw SSHException("Failed to open SSH session", SSHErrorCode::E_SESS_OPEN_FAILED);
        }

        _bChanOpen = true;
        return SSHErrorCode::E_NOERR;
    }

    void SSH::openShell()
    {
        if (_bShellOpen)
        {
            return;
        }
        if (!_bChanOpen)
        {
            openChannel();
        }
        int rc;
        rc = ssh_channel_request_pty(_channel);
        if (rc != SSH_OK) 
        {
            throw SSHException("Failed to request PTY from remote host", SSHErrorCode::E_PTY_REQUEST_FAILED);
        }
        rc = ssh_channel_change_pty_size(_channel, 80, 24);
        if (rc != SSH_OK) 
        {
            throw SSHException("Failed to change PTY size", SSHErrorCode::E_PTY_SIZE_FAILED);
        }
        rc = ssh_channel_request_shell(_channel);
        if (rc != SSH_OK) 
        {
            throw SSHException("Failed to request remote shell", SSHErrorCode::E_SHELL_REQUEST_FAILED);
        }
        _bShellOpen = true;
    }

}
