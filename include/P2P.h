#ifndef __P2P_H__
#define __P2P_H__

#include "Environment.h"

class P2P {
    public:
        P2P();
        ~P2P();
        bool findInstallation();
        std::string extractVersion();
    private:
        std::string _version;
        std::string _location;
};

#endif
