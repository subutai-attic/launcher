/*
#include "Arguments.h"

Arguments* Arguments::_instance = NULL;

Arguments::Arguments()
{

}

Arguments::~Arguments()
{

}

Arguments* Arguments::instance()
{
    if (!_instance) _instance = new Arguments();
    return _instance;
}

void Arguments::set(int argc, char* argv[])
{
    for (int i = 0; i < argc; i++)
    {
        _args.push_back(argv[i]);
    }
}

std::vector<std::string> Arguments::get()
{
    return _args;
}

bool Arguments::find(const std::string& arg)
{
    for (auto it = _args.begin(); it != _args.end(); it++)
    {
        if ((*it) == arg) return true;
    }
    return false;
}

*/
