#include "SLException.h"

namespace SubutaiLauncher {

    SLException::SLException(const std::string& msg, int code) : SubutaiException(msg, code) {

    }

    const char* SLException::name() const throw() {
        return "Scripting Language Exception";
    }

};
