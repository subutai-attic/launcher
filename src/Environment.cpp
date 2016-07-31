#include "Environment.h"

Environment::Environment() {

}

Environment::~Environment() {

}

unsigned Environment::processorNum() {
#if defined (_SC_NPROCESSORS_ONLN)
    auto count = sysconf(_SC_NPROCESSORS_ONLN);
    if (count <= 0) count = 1;
    return static_cast<int>(count);
#endif
    return 1;
}

std::string Environment::getVar(const std::string& name, const std::string& defaultValue) {
    const char* value = getenv(name.c_str());
    if (value) {
        return std::string(value);
    } 
    return defaultValue;
}
