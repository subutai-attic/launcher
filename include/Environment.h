#ifndef __ENVIRONMENT_H__
#define __ENVIRONMENT_H__

#include <string>
#include <cstdlib>

namespace SubutaiLauncher {

    class Environment {
        public:
            Environment();
            ~Environment();
            unsigned processorNum();
            std::string getVar(const std::string& name, const std::string& defaultValue);
    };

};

#endif
