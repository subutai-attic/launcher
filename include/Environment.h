#ifndef __ENVIRONMENT_H__
#define __ENVIRONMENT_H__

#include <string>
#include <cstdlib>
#include <windows.h>

#include "Vars.h"
#include "SubutaiException.h"

namespace SubutaiLauncher {

    class Environment {
        public:
            Environment();
            ~Environment();
            unsigned processorNum();
            std::string getVar(const std::string& name, const std::string& defaultValue);
			std::string setVar(const std::string& name, const std::string& value);
    };

}

#endif
