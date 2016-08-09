#include "Core.h"

namespace SubutaiLauncher {

    Core::Core(std::vector<std::string> args) : _args(args) {

    }

    Core::~Core() {
        Py_Finalize();
    }

    void Core::initializePython() {
        std::printf("Initializing Python\n");
        setenv("PYTHONPATH", ".", 1);
        Py_Initialize();
        Py_InitModule("subutai", SubutaiSLMethods);
    }

    void Core::run() {
        initializePython();
        curl_global_init(CURL_GLOBAL_ALL);
        parseArgs();
    }

    void Core::parseArgs() {
        for (auto it = _args.begin(); it != _args.end(); it++) {
            if (it->compare("test") == 0) {
                handleTest();
            }
        }
    }

    void Core::handleTest() {
        std::printf("Testing VirtualBox implementation");
        VirtualBox vb;

        std::printf("Testing mode: HUB Rest API\n");

        Hub h;
        h.setLogin("msavochkin@optimal-dynamics.com");
        h.setPassword("GNfH(e\"L22cR$RVx");
        if (h.auth()) {
            std::printf("[TEST] Hub Auth: OK\n");
        }
        if (h.balance()) {
            std::printf("[TEST] Hub Balance: OK\n");
        }

        
        std::printf("Testing mode: Looking for a test script launcher-test.py\n");
        SL script;
        try {
            script.open("launcher-test");
        } catch (SubutaiException& e) {
            if (e.code() == 1) {
                std::printf("launcher-test.py file was not found\n");
                return;
            }
        }
        try {
            script.execute();
        } catch (SLException& e) {
            std::printf(e.what());
        } catch (std::exception e) {
            std::printf(e.what());
        }
    }

};
