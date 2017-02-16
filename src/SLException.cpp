#include "SLException.h"


SubutaiLauncher::SLException::SLException(const std::string& msg, int code) : SubutaiException(msg, code) {

}

const char* SubutaiLauncher::SLException::name() const throw() {
	
    Log::instance()->logger()->debug() << "SSLException "  << " code: "  << std::endl;
    return "Scripting Language Exception";
}

