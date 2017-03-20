#include "SSH.h"
#define MAX_XFER_BUF_SIZE 16384

const std::string SubutaiLauncher::SSH::BIN = "ssh";

SubutaiLauncher::SSH::SSH() :
    _username("root"),
    _password(""),
    _host("127.0.0.1"),
    _port(22)
{
    _ssh = ssh_new();
    if (_ssh == NULL) {
        throw SubutaiException("Failed to start SSH session");
    }
}

SubutaiLauncher::SSH::~SSH()
{
    Log::instance()->logger()->debug() << "SSH::delete ssh session start"  << std::endl;
    if (_connected) ssh_disconnect(_ssh);
    if (_ssh != NULL) {
        ssh_free(_ssh);
        Log::instance()->logger()->debug() << "SSH::delete free" << std::endl;
    }
    Log::instance()->logger()->debug() << "SSH::delete ssh session end" << std::endl;
}

void SubutaiLauncher::SSH::disconnect()
{
    Log::instance()->logger()->debug() << "SSH::disconnect ssh session start  "  << std::endl;
    if (_connected) {
        ssh_disconnect(_ssh);
        //ssh_free(_ssh);
        //Log::instance()->logger()->debug() << "SSH::disconnect free after "  << std::endl;
        _connected = false;
    }
}

bool SubutaiLauncher::SSH::findInstallation()
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

void SubutaiLauncher::SSH::setUsername(const std::string& username, const std::string& password)
{
    _username = username;
    _password = password;
}

void SubutaiLauncher::SSH::setHost(const std::string& host, long port)
{
    _host = host;
    _port = port;
}

std::string SubutaiLauncher::SSH::run(const std::string& command) const
{

    std::vector<std::string> args;
#if LAUNCHER_WINDOWS
    char usr[512];
    char port[64];
    sprintf_s(port, sizeof(port), "%lu", _port);
    sprintf_s(usr, sizeof(usr), "%s@%s", _username.c_str(), _host.c_str());
#else
    char* usr;
    char* port;
    std::sprintf(port, "%lu", _port);
    std::sprintf(usr, "%s@%s", _username.c_str(), _host.c_str());
#endif

    args.push_back("-p");
    args.push_back(port);
    args.push_back(std::string(usr));
    args.push_back(command);

    SubutaiProcess p;
    p.launch(BIN, args, _location);
    p.wait();
    return p.getOutputBuffer();
}

void SubutaiLauncher::SSH::connect()
{
    int rc;
    ssh_options_set(_ssh, SSH_OPTIONS_HOST, _host.c_str());
    ssh_options_set(_ssh, SSH_OPTIONS_PORT, &_port);
    ssh_options_set(_ssh, SSH_OPTIONS_USER, _username.c_str());

    rc = ssh_connect(_ssh);

    Log::instance()->logger()->debug() << "SSH::connect  " << rc << std::endl;
    if (rc != SSH_OK) {
        //throw new SubutaiException("Failed to establish SSH connection to the host");
        _connected = false;
        Log::instance()->logger()->debug() << "SSH::connect failed" << std::endl;
        return;
    }
    _connected = true;
}

void SubutaiLauncher::SSH::authenticate()
{
    int rc = ssh_userauth_password(_ssh, NULL, _password.c_str());
    Log::instance()->logger()->debug() << "SSH::authenticate  " << rc << std::endl;
    if (rc == SSH_AUTH_ERROR) {
        _authenticated = false;
        return;
    }
    _authenticated = true;
}

int SubutaiLauncher::SSH::verifyHost()
{
    int state, hlen;
    unsigned char* hash = NULL;

    state = ssh_is_server_known(_ssh);

    hlen = ssh_get_pubkey_hash(_ssh, &hash);
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
                //throw SubutaiException("Failed to add SSH key");
                return -1;
            }
            break;

        case SSH_SERVER_ERROR:
            free(hash);
            //throw SubutaiException("Undefined SSH error");
            return -1;
            break;
    };
    free(hash);
    return 0;
}

std::string SubutaiLauncher::SSH::execute(const std::string& command){
    ssh_channel chan;
    int rc;
    char buffer[1024];
    int nbytes;
    memset(buffer, 0, 1024);

    chan = ssh_channel_new(_ssh);
    if (chan == NULL) 
        return "Error: Failed to open channel ";
    //throw SubutaiException("Failed to open channel");

    rc = ssh_channel_open_session(chan);
    if (rc != SSH_OK) {
        ssh_channel_close(chan);
        ssh_channel_free(chan);
        return "Error: Failed to open SSH session ";
        //throw SubutaiException("Failed to open SSH session");
    }

    rc = ssh_channel_request_exec(chan, command.c_str());
    if (rc != SSH_OK) {
        ssh_channel_close(chan);
        ssh_channel_free(chan);
        return "Error: Failed to execute command ";
        //throw SubutaiException("Failed to execute command");
    }

    nbytes = ssh_channel_read(chan, buffer, sizeof(buffer), 1);

    while (nbytes > 0) {
#if LAUNCHER_WINDOWS
        if (_write(1, buffer, nbytes) != (unsigned int)nbytes) {
#else
            if (fwrite(buffer, 1, nbytes, stdout) != nbytes) {
#endif
                ssh_channel_close(chan);
                ssh_channel_free(chan);
                return "Error: Failed to wtite to channel ";
                //throw SubutaiException("Failed to write to channel");
            }
            nbytes = ssh_channel_read(chan, buffer, sizeof(buffer), 1);
        }

        if (nbytes < 0) {
            ssh_channel_close(chan);
            ssh_channel_free(chan);
            return "Error: Output channel is empty ";
            //throw SubutaiException("Empty from channel");
        }

        ssh_channel_send_eof(chan);
        ssh_channel_close(chan);
        //ssh_channel_read_exit_status(chan);
        ssh_channel_free(chan);
        std::string ret(buffer);
        Log::instance()->logger()->debug() << "SSH::execute buffer: /" << buffer  << "/" << std::endl;
        Log::instance()->logger()->debug() << "SSH::execute ret: /" << ret  << "/" << std::endl;
        return ret;
    }

    long SubutaiLauncher::SSH::scpWrite(const std::string& src)
    {
        ssh_scp scp;
        int rc;
        //scp = ssh_scp_new
        //    (session, SSH_SCP_WRITE | SSH_SCP_RECURSIVE, ".");
        scp = ssh_scp_new
            (_ssh, SSH_SCP_WRITE | SSH_SCP_RECURSIVE, ".");
        if (scp == NULL)
        {

            //fprintf(stderr, "Error allocating scp session: %s\n",
            //    ssh_get_error(_ssh));
            return SSH_ERROR;
        }
        rc = ssh_scp_init(scp);
        if (rc != SSH_OK)
        {
            Log::instance()->logger()->debug() << "SSH::scpWrite scp init rc = " << rc << std::endl;
            //fprintf(stderr, "Error initializing scp session: %s\n",
            //    ssh_get_error(session));
            ssh_scp_free(scp);
            return rc;
        }

        ssh_scp_close(scp);
        ssh_scp_free(scp);
        return SSH_OK;
    }

    long SubutaiLauncher::SSH::sftpWrite(const std::string& src)
    {
        Log::instance()->logger()->debug() << "SSH::sftpWrite start" << std::endl;
        char buffer[MAX_XFER_BUF_SIZE];
        sftp_session sftp;
        int rc, nwritten, nbytes, access_type;
        sftp = sftp_new(_ssh);
        if (sftp == NULL)
        {
            Log::instance()->logger()->debug() << "SSH::sftpWrite sftp_new failed = " << ssh_get_error(_ssh) << std::endl;
            //fprintf(stderr, "Error allocating SFTP session: %s\n",
            //        ssh_get_error(session));
            return SSH_ERROR;
        }

        Log::instance()->logger()->debug() << "SSH::sftpWrite sftp_new " << std::endl;

        rc = sftp_init(sftp);
        if (rc != SSH_OK)
        {
            Log::instance()->logger()->debug() << "SSH::sftpWrite  sftp init failed rc = " << rc << std::endl;
            //fprintf(stderr, "Error initializing SFTP session: %s.\n",
            //        sftp_get_error(sftp));
            sftp_free(sftp);
            return rc;
        }
        Log::instance()->logger()->debug() << "SSH::sftpWrite sftp init rc = " << rc << std::endl;
        access_type = O_WRONLY | O_CREAT | O_TRUNC;

        std::string fDest = "/tmp/";
        fDest.append(src);

        sftp_file sfile = sftp_open(sftp, fDest.c_str(),  access_type, S_IRWXU);
        if (sfile == NULL)
        {
            Log::instance()->logger()->error() << "SSH::sftpWrite sftp_open failed: " << fDest << std::endl;
            //fprintf(stderr, "Can't open file for writing: %s\n",
            //        ssh_get_error(session));
            return SSH_ERROR;
        }

        Log::instance()->logger()->debug() << "SSH::sftpWrite sftp_open succeded: " << fDest << std::endl;

        std::string fSrc = "/tmp/subutai/";
        fSrc.append(src);
        Log::instance()->logger()->debug() << "SSH::sftpWrite source is  fSrc = " << fSrc << std::endl;
        int fd = open(fSrc.c_str(), O_RDONLY);
        if (fd < 0) {
            Log::instance()->logger()->error() << "SSH::sftpWrite sftp init rcopen source file failed = " << fd << std::endl;
            return SSH_ERROR;
        }

        for (;;) {
            //nbytes = sftp_read(file, buffer, sizeof(buffer));
            nbytes = read(fd, buffer, sizeof(buffer));
            Log::instance()->logger()->debug() << "SSH::sftpWrite bytes read = " << nbytes << std::endl;
            if (nbytes == 0) {
                Log::instance()->logger()->debug() << "SSH::sftpWrite bytes read = EOF " << nbytes << std::endl;
                break; // EOF
            } else if (nbytes < 0) {
                Log::instance()->logger()->error() << "SSH::sftpWrite sftp error reading source file = " << nbytes << std::endl;
                //fprintf(stderr, "Error while reading file: %s\n",
                //          ssh_get_error(session));
                close(fd);
                sftp_close(sfile);
                return SSH_ERROR;
            }

            nwritten = sftp_write(sfile, buffer, nbytes);
            Log::instance()->logger()->debug() << "SSH::sftpWrite bytes written = " << nwritten << std::endl;
            if (nwritten != nbytes) {
                Log::instance()->logger()->error() << "SSH::sftpWrite sftp error writing dest file = " << nbytes << std::endl;
                //fprintf(stderr, "Error writing: %s\n",
                //    strerror(errno));
                sftp_close(sfile);
                return SSH_ERROR;
            }
        }
        /*
           nwritten = sftp_write(file, helloworld, length);
           if (nwritten != length)
           {
           Log::instance()->logger()->error() << "SSH::sftpWrite write data written = " << written << std::endl;
        //fprintf(stderr, "Can't write data to file: %s\n",
        //        ssh_get_error(session));
        sftp_close(file);
        return SSH_ERROR;
        }
        */
        Log::instance()->logger()->debug() << "SSH::sftpWrite bytes EOF reached "  << std::endl;
        rc = sftp_close(sfile);
        if (rc != SSH_OK)
        {
            Log::instance()->logger()->error() << "SSH::sftpWrite can't close written file = " << ssh_get_error(_ssh)  << std::endl;
            //fprintf(stderr, "Can't close the written file: %s\n",
            //        ssh_get_error(session));
            return rc;
        }

        sftp_free(sftp);
        return SSH_OK;
    }


    bool SubutaiLauncher::SSH::isConnected()
    {
        return _connected;
    }

    bool SubutaiLauncher::SSH::isAuthenticated()
    {
        return _authenticated;
    }

    std::string SubutaiLauncher::SSH::getPublicKey()
    {
        Environment env;
        auto var = env.getVar("HOME", "~");
        var.append("/.ssh/id_rsa.pub");
        try {
            Poco::FileInputStream fs(var);
            std::string buffer;
            Poco::StreamCopier::copyToString(fs, buffer);
            return buffer;
        } catch (Poco::FileNotFoundException e) {
            Log::instance()->logger()->error() << "id_rsa.pub file not found"  << std::endl;
            return "";
        } catch (std::exception e) {
            Log::instance()->logger()->error() << "exception"  << std::endl;
            return "";
        }
    }
