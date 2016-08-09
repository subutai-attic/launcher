#include "String.h"

namespace SubutaiLauncher {

    String::String(const std::string& str) : _str(str) {

    }

    String::~String() {

    }

    void String::split(char delim, std::vector<std::string> &items) {
        std::stringstream ss(_str);
        std::string item;
        while (getline(ss, item, delim)) {
            items.push_back(item);
        }
    }

}
