#include "SLException.h"


SubutaiLauncher::SLException::SLException(const std::string& msg, int code) : SubutaiException(msg, code) {
    auto l = Log::instance()->logger();
    l->error() << "SLException " << std::endl;
}

const char* SubutaiLauncher::SLException::name() const throw() {
	
    return "Scripting Language Exception";
}

