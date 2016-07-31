#include "P2P.h"

namespace SubutaiLauncher {

    P2P::P2P() {

    }

    P2P::~P2P() {

    }

    bool P2P::findInstallation() {
        auto env = new Environment();
        auto path = env->getVar("PATH", "");
        if (path == "") {
            std::printf("Failed to extract PATH environment variable\n");
            return false;
        }

    }

};
