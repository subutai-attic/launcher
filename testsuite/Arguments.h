#ifndef __ARGUMENTS_H__
#define __ARGUMENTS_H__

#include <cstdio>
#include <vector>
#include <string>

class Arguments
{
    public:
        ~Arguments();
        static Arguments* instance();
        void set(int argc, char* argv[]);
        std::vector<std::string> get();
        bool find(const std::string& arg);
    protected:
        Arguments();
        static Arguments* _instance;
        std::vector<std::string> _args;
};

#endif
