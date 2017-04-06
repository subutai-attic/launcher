#include "SubutaiException.h"

SubutaiLauncher::SubutaiException::SubutaiException(const std::string& msg, int code) : 
    _message(msg), 
    _code(code) 
{

}

SubutaiLauncher::SubutaiException::~SubutaiException() throw() 
{
    Poco::Logger::get("subutai").debug("SubutaiException thrown");
}

const char* SubutaiLauncher::SubutaiException::name() const throw() 
{
    std::string text = "Subutai Launcher Exception";
    text.append(": ");
    text.append(displayText());
    return text.c_str();
}

std::string SubutaiLauncher::SubutaiException::displayText() const 
{
    std::string text = name();
    text.append(": ");
    text.append(_message);
    Poco::Logger::get("subutai").debug("SubutaiException: %s", text);
    return text;
}

int SubutaiLauncher::SubutaiException::code() const 
{
    return _code;
}
