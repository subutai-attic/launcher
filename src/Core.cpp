#include "Core.h"

SubutaiLauncher::Core::Core(std::vector<std::string> args) : 
    _args(args),
    _running(false)
{

}

SubutaiLauncher::Core::~Core()
{
    if (_running) {
        while (!Session::instance()->getDownloader()->isDone()) {
            // Waiting
        };
    }

    Py_Finalize();
}

void SubutaiLauncher::Core::initializePython()
{
    Log::instance()->logger()->info() << "Initializing Python v" << PY_MAJOR_VERSION << "." << PY_MINOR_VERSION << std::endl;
    //Environment e;
    //e.setVar("PYTHONPATH", ".");
    //
#if PY_MAJOR_VERSION >= 3 
    PyImport_AppendInittab("subutai", &PyInit_Subutai);
#endif

    Py_Initialize();


#if PY_MAJOR_VERSION < 3
    Py_InitModule("subutai", SubutaiSLMethods);
#endif

    /*  
    char *path, *newpath; 
    path = Py_GetPath(); 
    newpath = new char[strlen(path)+4]; 
    strcpy(newpath, path); 
    strcat(newpath, ":."); 
    // ":." for unix, or ";." for windows 
    PySys_SetPath(newpath); 
    delete newpath;
    */
}

void SubutaiLauncher::Core::run()
{
    _running = true;
    Log::instance();
    initializePython();
    curl_global_init(CURL_GLOBAL_ALL);
    Session::instance();
    Session::instance()->getConfManager()->load();
    Session::instance()->getConfManager()->run();
    parseArgs();
}

void SubutaiLauncher::Core::parseArgs()
{
    for (auto it = _args.begin(); it != _args.end(); it++) {
        if (it->compare("test") == 0) {
            handleTest();
        }
    }
}

void SubutaiLauncher::Core::handleTest()
{
    std::printf("Checking configurations\n");
    auto confs = Session::instance()->getConfManager()->getConfigs();
    for (auto it = confs.begin(); it != confs.end(); it++) {
        std::printf("Configuration: %s, Desc: %s, File: %s\n", (*it).title.c_str(), (*it).description.c_str(), (*it).file.c_str());
    }
    return;

    std::printf("Testing VirtualBox implementation");
    VirtualBox vb;

    std::printf("Testing mode: HUB Rest API\n");

    Hub h;
    h.setLogin("msavochkin@optimal-dynamics.com");
    h.setPassword("");
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
    }
    catch (SubutaiException& e) {
        if (e.code() == 1) {
            std::printf("launcher-test.py file was not found\n");
            return;
        }
    }
    try {
        script.execute();
    }
    catch (SLException& e) {
        std::printf("SL Exception: %s\n", e.displayText().c_str());
    }
    catch (std::exception e) {
        std::printf("Exception: %s\n", e.what());
    }
}
