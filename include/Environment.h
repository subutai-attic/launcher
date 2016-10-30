#ifndef __ENVIRONMENT_H__
#define __ENVIRONMENT_H__

#include "Vars.h"

#include <string>
#include <cstdlib>
#if LAUNCHER_WINDOWS
#include <windows.h>
#endif

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
