#ifndef __SUBUTAI_EXCEPTION_H__
#define __SUBUTAI_EXCEPTION_H__

#include <stdexcept>
#include <string>

namespace SubutaiLauncher {

    class SubutaiException : public std::exception {
        public:
            SubutaiException(const std::string& msg, int code = 0);

            ~SubutaiException() throw();

            virtual const char* name() const throw();

            const std::string& message() const;

            int code() const;

            std::string displayText() const;
        protected:
            int _code;
            std::string _message;
    };

};

#endif
