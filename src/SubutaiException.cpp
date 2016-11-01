#include "SubutaiException.h"

SubutaiLauncher::SubutaiException::SubutaiException(const std::string& msg, int code) : _message(msg), _code(code) {

}

SubutaiLauncher::SubutaiException::~SubutaiException() throw() {

}

const char* SubutaiLauncher::SubutaiException::name() const throw() {
	return "Subutai Launcher Exception";
}

std::string SubutaiLauncher::SubutaiException::displayText() const {
	std::string text = name();
	text.append(": ");
	text.append(_message);
	return text;
}

int SubutaiLauncher::SubutaiException::code() const {
	return _code;
}
