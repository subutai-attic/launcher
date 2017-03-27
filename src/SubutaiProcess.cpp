#include "SubutaiProcess.h"

SubutaiLauncher::SubutaiProcess::SubutaiProcess()
{

}

SubutaiLauncher::SubutaiProcess::~SubutaiProcess()
{
    closeFds();
}

void SubutaiLauncher::SubutaiProcess::runBasic(const std::string& command, std::vector<std::string> args)
{

}

#if LAUNCHER_LINUX || LAUNCHER_MACOS
pid_t SubutaiLauncher::SubutaiProcess::launch(const std::string& cmd, std::vector<std::string> args, const std::string& dir)
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
	//throw SubutaiException("Can't fork process");
	return -1;
    }
    else if (pid == 0) {
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

#elif LAUNCHER_WINDOWS
SubutaiLauncher::pid_t SubutaiLauncher::SubutaiProcess::launch(const std::string& cmd, std::vector<std::string> args, const std::string& dir)
{
    std::string cmdLine = cmd;
    for (auto it = args.begin(); it != args.end(); it++)
    {
	cmdLine.append(" ");
	cmdLine.append(*it);
    }

    STARTUPINFOA si;
    GetStartupInfoA(&si);
    si.cb = sizeof(STARTUPINFOA);
    si.lpReserved = NULL;
    si.lpDesktop = NULL;
    si.lpTitle = NULL;
    si.dwFlags = STARTF_FORCEOFFFEEDBACK;
    si.cbReserved2 = 0;
    si.lpReserved2 = NULL;
    si.dwFlags |= STARTF_USESTDHANDLES;

    HANDLE hProc = GetCurrentProcess();
    bool inherit = false;

    if (_inRead)
    {
	DuplicateHandle(hProc, _inRead, hProc, &si.hStdInput, 0, TRUE, DUPLICATE_SAME_ACCESS);
	CloseHandle(_inRead);
	_inRead = INVALID_HANDLE_VALUE;
    }
    if (_outRead)
    {
	DuplicateHandle(hProc, _outWrite, hProc, &si.hStdOutput, 0, TRUE, DUPLICATE_SAME_ACCESS);
	CloseHandle(_outWrite);
	_outWrite = INVALID_HANDLE_VALUE;
    }
    if (_errRead)
    {
	DuplicateHandle(hProc, _errWrite, hProc, &si.hStdError, 0, TRUE, DUPLICATE_SAME_ACCESS);
	CloseHandle(_errWrite);
	_errWrite = INVALID_HANDLE_VALUE;
    }

    const char* workingDirectory = dir.empty() ? 0 : dir.c_str();
    const char* env = 0;

    PROCESS_INFORMATION pi;
    DWORD creationFlags = GetConsoleWindow() ? 0 : CREATE_NO_WINDOW;
    BOOL rc = CreateProcessA(
	NULL,
	const_cast<char*>(cmdLine.c_str()),
	NULL,
	NULL,
	true,
	creationFlags,
	(LPVOID) env,
	workingDirectory,
	&si,
	&pi
    );

    if (si.hStdInput) CloseHandle(si.hStdInput);
    if (si.hStdOutput) CloseHandle(si.hStdOutput);
    if (si.hStdError) CloseHandle(si.hStdError);
    if (rc)
    {
    	CloseHandle(pi.hThread);
	_process = pi.hProcess;
	return pi.dwProcessId;
    }
    //throw new SubutaiException("Can't launch process");
    return -1;
}

#elif LAUNCHER_MACOS
//#error Not implemented on this platform
#endif

int SubutaiLauncher::SubutaiProcess::wait()
{
#if LAUNCHER_LINUX
    int status, rc;
    do {
    	rc = waitpid(_pid, &status, 0);
    } while (rc < 0 && errno == EINTR);
    if (rc != _pid)
	//throw SubutaiException("Can't wait for process to finish");
	return -1;
    return WEXITSTATUS(status);
#elif LAUNCHER_WINDOWS
    DWORD rc = WaitForSingleObject(_process, INFINITE);
    if (rc != WAIT_OBJECT_0) throw SubutaiException("Wait failed for process");

    DWORD exitCode;
    if (GetExitCodeProcess(_process, &exitCode) == 0)
    	throw SubutaiException("Cannot get exit code");

    return exitCode;
#elif LAUNCHER_MACOS
//#error Not implemented on this platform
#endif
}

std::string SubutaiLauncher::SubutaiProcess::getOutputBuffer()
{
#if LAUNCHER_LINUX
    if (_outRead == -1) {
	//throw SubutaiException("Reading from closed pipe");
	return "Error: Reading from closed pipe";
    }
    char buffer[4096];
    memset(buffer, 0, 4096);
    int n;
    do {
    	read(_outRead, buffer, sizeof(buffer));
    } while (n < 0 && errno == EINTR);
	if (n >= 0) {
	return std::string(buffer);
    }
    //else throw SubutaiException("Failed to read output from anonymous pipe");
    else return "Error: Failed to read output from anonymous pipe";
#elif LAUNCHER_WINDOWS
    DWORD n = 0;
    char buffer[4096];
    BOOL ok = ReadFile(_outRead, buffer, sizeof(buffer), &n, NULL);
    if (ok || GetLastError() == ERROR_BROKEN_PIPE)
    	return std::string(buffer);
    else
	//throw SubutaiException("Read: Anonymous pipe");
	return "Error: Read: Anonymous pipe";
#elif LAUNCHER_MACOS
//#error hot implemented on this platform
#endif
}

std::string SubutaiLauncher::SubutaiProcess::getErrorBuffer()
{
#if LAUNCHER_LINUX
    if (_errRead == -1) {
	//throw SubutaiException("Reading from closed pipe");
	return "Error: Reading from closed pipe";
    }
    char buffer[4096];
    memset(buffer, 0, 4096);
    int n;
    do {
    	read(_errRead, buffer, sizeof(buffer));
    } while (n < 0 && errno == EINTR);
    if (n >= 0) {
	return std::string(buffer);
    }
    //else throw SubutaiException("Failed to read output from anonymous pipe");
    else return "Error: Failed to read output from anonymous pipe";
#elif LAUNCHER_WINDOWS
    DWORD n = 0;
    char buffer[4096];
    BOOL ok = ReadFile(_errRead, buffer, sizeof(buffer), &n, NULL);
    if (ok || GetLastError() == ERROR_BROKEN_PIPE)
    	return std::string(buffer);
    else
	//throw SubutaiException("Read: Anonymous pipe");
	return "Error: Read: Anonymous pipe";
#elif LAUNCHER_MACOS
//#error Not implemented on this platform
#endif
}

void SubutaiLauncher::SubutaiProcess::setupFds()
{
#if LAUNCHER_LINUX
	int ifds[2], ofds[2], efds[2];
	int rc = pipe(ifds);
	if (rc == 0) {
		_inRead = ifds[0];
		_inWrite = ifds[1];
	}
	else throw SubutaiException("Anonymous Pipe");
	rc = pipe(ofds);
	if (rc == 0) {
		_outRead = ofds[0];
		_outWrite = ofds[1];
	}
	else throw SubutaiException("Anonymous Pipe");
	rc = pipe(efds);
	if (rc == 0) {
		_errRead = efds[0];
		_errWrite = efds[1];
	}
	else throw SubutaiException("Anonymous Pipe");
#elif LAUNCHER_WINDOWS
	SECURITY_ATTRIBUTES attrs;
	attrs.nLength = sizeof(attrs);
	attrs.lpSecurityDescriptor = NULL;
	attrs.bInheritHandle = FALSE;

	if (!CreatePipe(&_inRead, &_inWrite, &attrs, 0)) {
		throw SubutaiException("Anonymous Pipe");
	}
	if (!CreatePipe(&_outRead, &_outWrite, &attrs, 0)) {
		throw SubutaiException("Anonymous Pipe");
	}
	if (!CreatePipe(&_outRead, &_outWrite, &attrs, 0)) {
		throw SubutaiException("Anonymous Pipe");
	}
#elif LAUNCHER_MACOS

#endif
}

void SubutaiLauncher::SubutaiProcess::closeFds()
{
#if LAUNCHER_LINUX
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
#elif LAUNCHER_WINDOWS
	if (_inRead != INVALID_HANDLE_VALUE)
	{
		CloseHandle(_inRead);
		_inRead = INVALID_HANDLE_VALUE;
	}
	if (_inWrite != INVALID_HANDLE_VALUE)
	{
		CloseHandle(_inWrite);
		_inWrite = INVALID_HANDLE_VALUE;
	}
	if (_outRead != INVALID_HANDLE_VALUE)
	{
		CloseHandle(_outRead);
		_outRead = INVALID_HANDLE_VALUE;
	}
	if (_outWrite != INVALID_HANDLE_VALUE)
	{
		CloseHandle(_outWrite);
		_outWrite = INVALID_HANDLE_VALUE;
	}
	if (_errRead != INVALID_HANDLE_VALUE)
	{
		CloseHandle(_errRead);
		_errRead = INVALID_HANDLE_VALUE;
	}
	if (_errWrite != INVALID_HANDLE_VALUE)
	{
		CloseHandle(_errWrite);
		_errWrite = INVALID_HANDLE_VALUE;
	}
#elif LAUNCHER_MACOS
//#error Not implemented on this platform
#endif
}

std::string SubutaiLauncher::SubutaiProcess::execute(const std::string& command, const std::string& cargs)
{
    SubutaiString str(cargs);
    std::vector<std::string> args;
    str.split(' ', args);
    SubutaiProcess p;
    p.launch(command, args, "/usr/bin");
    p.wait();
    std::string out = p.getOutputBuffer();
    Log::instance()->logger()->debug() << "SubutaiProcess::execute" << out << std::endl;
    return out;
}

