#include "SLException.h"


SubutaiLauncher::SLException::SLException(const std::string& msg, int code) : SubutaiException(msg, code) 
{
    Poco::Logger::get("subutai").error("SLException");
}

const char* SubutaiLauncher::SLException::name() const throw() 
{
    return "Scripting Language Exception";
}

