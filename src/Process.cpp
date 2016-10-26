#include "Process.h"

namespace SubutaiLauncher {

    Process::Process()
    {

    }

    Process::~Process()
    {
        closeFds();
    }

    void Process::runBasic(const std::string& command, std::vector<std::string> args) 
    {

    }

    pid_t Process::launch(const std::string& cmd, std::vector<std::string> args, const std::string& dir) 
    {
        std::vector<char> env;
        std::vector<char*> argv(args.size() + 2);
        int i = 0;
        argv[i++] = const_cast<char*>(cmd.c_str());
        for (auto it = args.begin(); it != args.end(); it++)
            argv[i++] = const_cast<char*>(it->c_str());
        argv[i] = NULL;

        const char* initDir = dir.empty() ? 0 : dir.c_str();

        setupFds();
        int pid = fork();
        if (pid < 0) {
            throw SubutaiException("Can't fork process");
        } else if (pid == 0) {
            if (initDir) {
                if (chdir(initDir) != 0) {
                    _exit(72);
                }
            }
            dup2(_inRead, STDIN_FILENO);
            close(_inRead);
            close(_inWrite);
            _inRead = -1;
            _inWrite = -1;
            dup2(_outWrite, STDOUT_FILENO);
            dup2(_errWrite, STDERR_FILENO);
            closeFds();
            for (int i = 3; i < sysconf(_SC_OPEN_MAX); ++i)
                close(i);

            execvp(argv[0], &argv[0]);
            _exit(72);
        }

        close(_inRead);
        _inRead = -1;
        close(_outWrite);
        _outWrite = -1;
        close(_errWrite);
        _errWrite = -1;
        _pid = pid;
        return pid;
    }

    int Process::wait()
    {
        int status, rc;
        do {
            rc = waitpid(_pid, &status, 0);
        } while (rc < 0 && errno == EINTR);
        if (rc != _pid)
            throw SubutaiException("Can't wait for process to finish");
        return WEXITSTATUS(status);
    }

    std::string Process::getOutputBuffer()
    {
        if (_outRead == -1) {
            throw SubutaiException("Reading from closed pipe");
        }
        char buffer[4096];
        int n;
        do {
            read(_outRead, buffer, sizeof(buffer));
        } while (n < 0 && errno == EINTR);
        if (n >= 0) {
            return std::string(buffer);
        } else throw SubutaiException("Failed to read output from anonymous pipe");
    }

    std::string Process::getErrorBuffer()
    {
        if (_errRead == -1) {
            throw SubutaiException("Reading from closed pipe");
        }
        char buffer[4096];
        int n;
        do {
            read(_errRead, buffer, sizeof(buffer));
        } while (n < 0 && errno == EINTR);
        if (n >= 0) {
            return std::string(buffer);
        } else throw SubutaiException("Failed to read output from anonymous pipe");
    }

    void Process::setupFds()
    {
        int ifds[2], ofds[2], efds[2];
        int rc = pipe(ifds);
        if (rc == 0) {
            _inRead = ifds[0];
            _inWrite = ifds[1];
        } else throw SubutaiException("Anonymous Pipe");
        rc = pipe(ofds);
        if (rc == 0) {
            _outRead = ofds[0];
            _outWrite = ofds[1];
        } else throw SubutaiException("Anonymous Pipe");
        rc = pipe(efds);
        if (rc == 0) {
            _errRead = efds[0];
            _errWrite = efds[1];
        } else throw SubutaiException("Anonymous Pipe");
    }

    void Process::closeFds()
    {
        if (_inRead != -1) {
            close(_inRead);
            _inRead = -1;
        }
        if (_inWrite != -1) {
            close(_inWrite);
            _inWrite = -1;
        }
        if (_outRead != -1) {
            close(_outRead);
            _outRead = -1;
        }
        if (_outWrite != -1) {
            close(_outWrite);
            _outWrite = -1;
        }
        if (_errRead != -1) {
            close(_errRead);
            _errRead = -1;
        }
        if (_errWrite != -1) {
            close(_errWrite);
            _errWrite = -1;
        }
    }
};
