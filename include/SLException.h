#ifndef __SL_EXCEPTION_H__
#define __SL_EXCEPTION_H__

#include "SubutaiException.h"
#include "SubutaiLog.h"
namespace SubutaiLauncher {

    class SLException : public SubutaiException {
        public:
            SLException(const std::string& msg, int code = 0);
            const char* name() const throw();
        protected:
            int _code;
            std::string _message;
    };

}

#endif
