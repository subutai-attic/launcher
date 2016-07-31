#include "SubutaiException.h"

SubutaiException::SubutaiException(const std::string& msg, int code) : _message(msg), _code(code) {

}

SubutaiException::~SubutaiException() throw() {

}

const char* SubutaiException::name() const throw() {
    return "Subutai Launcher Exception";
}

std::string SubutaiException::displayText() const {
    std::string text = name();
    text.append(": ");
    text.append(_message);
    return text;
}

int SubutaiException::code() const {
    return _code;
}
