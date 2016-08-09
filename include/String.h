#ifndef __STRING_H__
#define __STRING_H__

#include <string>
#include <vector>
#include <sstream>

namespace SubutaiLauncher {

    class String {
        public:
            String(const std::string& str);
            ~String();
            void split(char delim, std::vector<std::string> &items);
private:
            std::string _str;
    };

};

#endif
