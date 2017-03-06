#include "SubutaiException.h"

SubutaiLauncher::SubutaiException::SubutaiException(const std::string& msg, int code) : _message(msg), _code(code) {

}

SubutaiLauncher::SubutaiException::~SubutaiException() throw() {

    Log::instance()->logger()->debug() << "SubutaiException throw "  <<  std::endl;
}

const char* SubutaiLauncher::SubutaiException::name() const throw() {
    std::string text = "Subutai Launcher Exception";
    text.append(": ");
    text.append(displayText());
    return text.c_str();
    //return "Subutai Launcher Exception";
}

std::string SubutaiLauncher::SubutaiException::displayText() const {
    std::string text = name();
    text.append(": ");
    text.append(_message);
    Log::instance()->logger()->debug() << "SubutaiException text "  << text << std::endl;
    return text;
}

int SubutaiLauncher::SubutaiException::code() const {
    return _code;
}
