#include "Core.h"

SubutaiLauncher::Core::Core(std::vector<std::string> args) : 
    _args(args),
    _running(false)
{
    setupLogger();
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
    Poco::Logger::get("subutai").information("Initializing Python v%d.%d", PY_MAJOR_VERSION, PY_MINOR_VERSION);
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
    FileSystem fs("/");
    try 
    {
        if (! fs.isFileExists("/tmp/subutai"))
        {
            //create /tmp/subutai
            fs.createDirectory("/tmp/subutai");
        }
    }
    catch(SubutaiException e) 
    {
        return ;
    }
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

void SubutaiLauncher::Core::setupLogger()
{
    Poco::AutoPtr<Poco::FileChannel>            pChannel(new Poco::FileChannel);
    Poco::AutoPtr<Poco::ConsoleChannel>         cConsole(new Poco::ConsoleChannel);
    Poco::AutoPtr<Poco::SplitterChannel>        pSplitter(new Poco::SplitterChannel);
    Poco::AutoPtr<Poco::PatternFormatter>       pFormatter(new Poco::PatternFormatter);
    pFormatter->setProperty("pattern", "%Y-%m-%d %H:%M:%S [%p]: %t");
    pFormatter->setProperty("times", "local");
    pSplitter->addChannel(pChannel);
    pSplitter->addChannel(cConsole);
#if LAUNCHER_MACOS
    pChannel->setProperty("path", "/usr/local/share/subutai/log/subutai-launcher.log");
#else
    pChannel->setProperty("path", "subutai-launcher.log");
#endif
    pChannel->setProperty("rotation", "5 M");
    Poco::AutoPtr<Poco::FormattingChannel> pFormatChannel(new Poco::FormattingChannel(pFormatter, pSplitter));
    Poco::Logger& log = Poco::Logger::get("subutai");
#ifdef BUILD_SCHEME_PRODUCTION
    log.setLevel("information");
#endif
#ifdef BUILD_SCHEME_MASTER
    log.setLevel("debug");
#endif
#ifdef BUILD_SCHEME_DEV
    log.setLevel("trace");
#endif
    log.setChannel(pFormatChannel);
    log.information("Logging system initialized");
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
