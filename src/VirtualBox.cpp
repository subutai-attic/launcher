#include "VirtualBox.h"

namespace SubutaiLauncher {

    VirtualBox::VirtualBox() {
        auto env = new Environment();
        auto path = env->getVar("PATH", "");
    }

    VirtualBox::~VirtualBox() {

    }



};
