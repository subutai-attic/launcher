#include <iostream>
#include <cstdio>

#include "Core.h"

using namespace SubutaiLauncher;

int main(int argc, char* argv[]) {
    // TODO: Add library and components checks
    std::vector<std::string> args;
    for (int i = 0; i < argc; i++) {
        args.push_back(std::string(argv[i]));
    }
    Core c(args);
    c.run();
    return 0;
}
