#include "Core.h"

#if LAUNCHER_WINDOWS
#pragma comment (lib, "crypt32")
#endif

SubutaiLauncher::Core::Core(std::vector<std::string> args) : 
    _args(args),
    _running(false),
    _noValidate(false),
    _appName("")
{
#if LAUNCHER_MACOS
    chdir("/usr/local/share/subutai");
#endif
    setupLogger();
    Poco::Logger::get("subutai").information("Subutai Launcher " + std::string(LAUNCHER_VERSION));
}

SubutaiLauncher::Core::Core() :
    _args(std::vector<std::string>()),
    _running(false)
{
#if LAUNCHER_MACOS
    chdir("/usr/local/share/subutai");
#endif
    setupLogger();
    Poco::Logger::get("subutai").information("Subutai Launcher " + std::string(LAUNCHER_VERSION));
}

SubutaiLauncher::Core::Core(const std::string& appName) : 
    _appName(appName)
{
    
}

SubutaiLauncher::Core::~Core()
{
    Poco::Logger::get("subutai").information("Stopping Subutai Launcher Core");
    if (_running) {
        while (Session::instance()->getDownloader()->isRunning() && !Session::instance()->getDownloader()->isDone()) {
            // Waiting
            // TODO: Put timeout here
        };
    }
    Poco::Logger::get("subutai").information("Subutai Launcher Core stopped");

    Py_Finalize();
    SSH::deinitialize();
}

void SubutaiLauncher::Core::initializePython()
{
    Poco::Logger::get("subutai").information("Initializing Python v%d.%d", PY_MAJOR_VERSION, PY_MINOR_VERSION);

#if LAUNCHER_WINDOWS
    std::string pNewPythonPath = "D:\\Projects\cpython";
    std::string pNewPythonHome = "D:\\Projects\cpython";
    Py_SetPythonHome(L".");
#endif

#if PY_MAJOR_VERSION >= 3 
    PyImport_AppendInittab("subutai", &PyInit_Subutai);
#endif

    if (_args.size() > 0)
    {
        std::string pName = _args.at(0);
#if LAUNCHER_LINUX
        // This is a workaround for SubutaiLauncher symlink file which gaves us
        // not full path to application in argv[0]
        if (pName == "SubutaiLauncher") 
        {
            pName = "/opt/subutai/bin/SubutaiLauncher";
        }
#endif
        Poco::Logger::get("subutai").information("Setting program: %s", pName);
        std::wstring pwName = std::wstring(pName.begin(), pName.end());
        const wchar_t* n = std::wstring(pName.begin(), pName.end()).c_str();
        Py_SetProgramName(const_cast<wchar_t*>(n));
//#endif
    }
    else
    {
        Poco::Logger::get("subutai").error("Failed to receive command line arguments");
    }
    Py_Initialize();


#if PY_MAJOR_VERSION < 3
#error Python versions below 3.5 is not supported
    Py_InitModule("subutai", SubutaiSLMethods);
#endif
}

void SubutaiLauncher::Core::initializeSSL()
{
    Poco::Logger::get("subutai").debug("Configuring SSL Client context");

    Poco::Net::Context* pContext = new Poco::Net::Context(
            Poco::Net::Context::CLIENT_USE,
            "",
            Poco::Net::Context::VERIFY_NONE,
            9,
            true,
            "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH"
            );
    Poco::Net::SSLManager::InvalidCertificateHandlerPtr ptrHandler ( new Poco::Net::AcceptCertificateHandler(false) );
    Poco::Net::SSLManager::instance().initializeClient(0, ptrHandler, pContext);
}

void SubutaiLauncher::Core::initializeSSH()
{
    SSH::initialize();
}

void SubutaiLauncher::Core::run()
{
    Poco::File f;
#if LAUNCHER_LINUX || LAUNCHER_MACOS
    f = Poco::File("/tmp/subutai");
#elif LAUNCHER_WINDOWS
    std::string path = "C:\\";
    try
    {
        std::string drive = Poco::Environment::get("USERPROFILE");
        path = drive;
        path.append("\\subutai\\");
        path.append("\\tmp\\");
    }
    catch (Poco::NotFoundException& e)
    {
        std::printf("Failed to extract home directory: %s\n", e.displayText());
    }
    f = Poco::File(path);
#endif
    try 
    {
        if (!f.exists())
        {
            try
            {
                f.createDirectories();
            }
            catch (std::exception e)
            {
                std::printf("%s\n", e.what());
            }
        }
    }
    catch (Poco::FileException& e)
    {
        e.rethrow();
    }
    _running = true;
    initializePython();
    initializeSSL();
    initializeSSH();
    Session::instance();
    // Configure Hub channel
    parseArgs();
}

void SubutaiLauncher::Core::parseArgs()
{
    for (auto it = _args.begin(); it != _args.end(); it++) 
    {
        if (it->compare("test") == 0) 
        {
        }
        if (it->compare("--no-validate") == 0)
        {
            _noValidate = true;
            Session::instance()->getDownloader()->setNoValidate(true);
        }
    }
}

void SubutaiLauncher::Core::setupLogger()
{
    Poco::AutoPtr<Poco::FileChannel>            pChannel(new Poco::FileChannel);
#if LAUNCHER_WINDOWS
	Poco::AutoPtr<Poco::WindowsConsoleChannel>  cConsole(new Poco::WindowsConsoleChannel);
#else
    Poco::AutoPtr<Poco::ConsoleChannel>         cConsole(new Poco::ConsoleChannel);
#endif
    Poco::AutoPtr<HubChannel>                   pHub(new HubChannel);
    Poco::AutoPtr<Poco::SplitterChannel>        pSplitter(new Poco::SplitterChannel);
    Poco::AutoPtr<Poco::PatternFormatter>       pFormatter(new Poco::PatternFormatter);
    pFormatter->setProperty("pattern", "%Y-%m-%d %H:%M:%S [%p]: %t");
    pFormatter->setProperty("times", "local");
    pSplitter->addChannel(pChannel);
    pSplitter->addChannel(cConsole);
    pSplitter->addChannel(pHub);
    Poco::DateTime dt;
    Poco::Timestamp now;
    std::string filename = "subutai-launcher-" + Poco::DateTimeFormatter::format(now, "%Y-%m-%d_%H-%M-%S") + ".log";
#if LAUNCHER_MACOS
    pChannel->setProperty("path", "/usr/local/share/subutai/log/" + filename);
#elif LAUNCHER_LINUX
    pChannel->setProperty("path", "/opt/subutai/log/" + filename);
#else
    std::string path = "C:\\Subutai";
    try
    {
        std::string drive = Poco::Environment::get("USERPROFILE");
        path = drive;
        path.append("\\subutai\\log");
        Poco::File f(path);
        if (!f.exists()) f.createDirectories();
        path.append("\\" + filename);
    }
    catch (Poco::NotFoundException& e)
    {
        std::printf("Couldn't find home directory: %s\n", e.displayText());
    }
    try 
    {
        pChannel->setProperty("path", path);
    }
    catch (Poco::OpenFileException& e)
    {
        std::printf("Can't create log file: %s\n", e.displayText());
    }
#endif
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
