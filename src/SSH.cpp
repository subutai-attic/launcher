#include "SSH.h"

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
	if (_connected) ssh_disconnect(_ssh);
	if (_ssh != NULL) ssh_free(_ssh);
}

bool SubutaiLauncher::SSH::findInstallation()
{
	auto env = new Environment();
	String pathVar(env->getVar("PATH", ""));
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
	char* usr;
	char* port;
	std::vector<std::string> args;

	std::sprintf(port, "%lu", _port);
	std::sprintf(usr, "%s@%s", _username.c_str(), _host.c_str());

	args.push_back("-p");
	args.push_back(port);
	args.push_back(std::string(usr));
	args.push_back(command);

	Process p;
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
	if (rc != SSH_OK) {
		throw new SubutaiException("Failed to establish SSH connection to the host");
		return;
	}
	_connected = true;

}

void SubutaiLauncher::SSH::authenticate()
{
	int rc = ssh_userauth_password(_ssh, NULL, _password.c_str());
	if (rc != SSH_AUTH_SUCCESS) {
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
		break;

	case SSH_SERVER_FILE_NOT_FOUND:

	case SSH_SERVER_NOT_KNOWN:
		if (ssh_write_knownhost(_ssh) < 0) {
			free(hash);
			throw SubutaiException("Failed to add SSH key");
			return -1;
		}
		break;

	case SSH_SERVER_ERROR:
		throw SubutaiException("Undefined SSH error");
		break;
	};
	free(hash);
	return 0;
}

void SubutaiLauncher::SSH::execute(const std::string& command)
{
	ssh_channel chan;
	int rc;
	char buffer[256];
	int nbytes;

	chan = ssh_channel_new(_ssh);
	if (chan == NULL) throw SubutaiException("Failed to open channel");

	rc = ssh_channel_open_session(chan);
	if (rc != SSH_OK) {
		ssh_channel_close(chan);
		ssh_channel_free(chan);
		throw SubutaiException("Failed to open SSH session");
	}

	rc = ssh_channel_request_exec(chan, command.c_str());
	if (rc != SSH_OK) {
		ssh_channel_close(chan);
		ssh_channel_free(chan);
		throw SubutaiException("Failed to execute command");
	}

	nbytes = ssh_channel_read(chan, buffer, sizeof(buffer), 0);
	while (nbytes > 0) {
		if (write(1, buffer, nbytes) != (unsigned int)nbytes) {
			ssh_channel_close(chan);
			ssh_channel_free(chan);
			throw SubutaiException("Failed to write to channel");
		}
		nbytes = ssh_channel_read(chan, buffer, sizeof(buffer), 0);
	}

	if (nbytes < 0) {
		ssh_channel_close(chan);
		ssh_channel_free(chan);
		throw SubutaiException("Empty from channel");
	}

	ssh_channel_send_eof(chan);
	ssh_channel_close(chan);
	ssh_channel_free(chan);
}

bool SubutaiLauncher::SSH::isConnected()
{
	return _connected;
}

bool SubutaiLauncher::SSH::isAuthenticated()
{
	return _authenticated;
}
