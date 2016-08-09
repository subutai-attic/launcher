#include "Session.h"

namespace SubutaiLauncher {

    Session* Session::_instance = NULL;

    Session::Session() {

    }

    Session::~Session() {

    }

    Session* Session::instance() {
        if (!_instance) _instance = new Session();
        return _instance;
    }

    void Session::destroyInstance() {
        delete this;
    }

};
