#define DONT_SET_USING_JUCE_NAMESPACE 1

#include "Main.h"
#include <functional>

class InitTimer : public juce::Timer {
    private:
        void (UIApplication::*m_timerCallbackHandler)();
        UIApplication* app_instance;

    public:
        // Timer interface
        virtual void timerCallback() {
            if (m_timerCallbackHandler) {
                (app_instance->*m_timerCallbackHandler)();
            }
        }

        InitTimer(void (UIApplication::*pf)(),
                UIApplication* instance) : m_timerCallbackHandler(pf),
        app_instance(instance){}
        virtual ~InitTimer() {}
};
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

UIApplication::UIApplication() :
    _core(nullptr),
    _initTimer(nullptr)
{
    /*
       SubutaiLauncher::AssetsManager pAssets;
       try
       {
       pAssets.verify();
       }
       catch (std::exception& e)
       {
       Poco::Logger::get("subutai").error("Failed to download assets");
       SubutaiLauncher::Session::instance()->getDownloader()->reset();
       }
       */
}

const juce::String UIApplication::getApplicationName()
{
    return "Subutai Launcher";
}

const juce::String UIApplication::getApplicationVersion()
{
    return LAUNCHER_VERSION;
}

bool UIApplication::moreThanOneInstanceAllowed()
{
    return false;
}

void UIApplication::initialise(const juce::String& commandLine)
{    
    auto args = getCommandLineParameterArray();
    std::vector<std::string> pArgs;
    for (auto it = args.begin(); it != args.end(); it++)
    {
        pArgs.push_back((*it).toStdString());
    }
    try
    {
        _core = new SubutaiLauncher::Core(pArgs);
    }
    catch (Poco::OpenFileException& e)
    {
        std::printf("Couldn't open log file: %s", e.displayText().c_str());
    }

    _logger = &Poco::Logger::get("subutai");
    _logger->trace("UI Started with: %s", commandLine.toStdString());

    try
    {
        _core->run();
    }
    catch (Poco::OpenFileException& e)
    {
        _logger->error(e.displayText());
    }
    catch (Poco::FileException& e)
    {
        _logger->error(e.displayText());
    }
    catch (Poco::PathSyntaxException& e)
    {
        _logger->error(e.displayText());
    }
    catch (Poco::Exception& e)
    {
        _logger->error(e.displayText());
    }
    _logger->information("Starting up the UI");
    SubutaiLauncher::Environment env;
    env.updatePath(SubutaiLauncher::Session::instance()->getSettings()->getInstallationPath() + "bin");

    Poco::Logger::get("subutai").information("Downloading assets");

    _initTimer = new InitTimer(&UIApplication::checkInitialization, this);
    SubutaiLauncher::AssetsManager pAssets;
    pAssets.download("launcher-splash.png");
    std::string pLogoFile(SubutaiLauncher::Session::instance()->getSettings()->getTmpPath() +
            "launcher-splash.png");
    Poco::File pLogo(pLogoFile);

    if (pLogo.exists()) {
        _splashScreen = new juce::SplashScreen(
                getApplicationName(),
                juce::ImageCache::getFromFile(juce::File(pLogoFile)),
                true);
        runSplashBackgroundTask().detach();
    }
    //_logger->debug("UI Initialization completed");
}

void UIApplication::shutdown()
{
    _logger->debug("Started UI shutdown procedure");
    if (_initTimer) delete _initTimer;
    if (_core) delete _core;
#if LIGHT_MODE
    _wizardWindow = nullptr;
#else
    _mainWindow = nullptr;
#endif

    _logger->debug("UI Destruction completed");
}

void UIApplication::systemRequestedQuit()
{
    _logger->debug("System Requested Quit");
    quit();
    _logger->debug("Quit Handled");
}

void UIApplication::anotherInstanceStarted(const juce::String& commandLine)
{

}

void UIApplication::startMainWindow()
{    
    std::string pTitle = "SubutaiLauncher ";
    pTitle.append(LAUNCHER_VERSION);
#if !BUILD_SCHEME_PRODUCTION
    pTitle.append(BUILD_SCHEME);
#endif

    _splashScreen->deleteAfterDelay(juce::RelativeTime::seconds(1), false);
#if LIGHT_MODE
    _wizardWindow = new WizardWindow(pTitle);
#else
    _mainWindow = new MainWindow(pTitle);
#endif
}

volatile bool initializationFinished = false;
void UIApplication::checkInitialization()
{
    if (!initializationFinished) return;
    _initTimer->stopTimer();
    startMainWindow();
}

std::thread UIApplication::runSplashBackgroundTask()
{
    _initTimer->startTimer(1000); //run timer in main thread. then run initialization thread
    return std::thread([=] {
            runSplashBackgroundTaskImpl();
            initializationFinished = true;
            });
}

void UIApplication::runSplashBackgroundTaskImpl()
{
    SubutaiLauncher::AssetsManager pAssets;

    try
    {
        pAssets.verify();
    }
    catch (std::exception& e)
    {
        Poco::Logger::get("subutai").error("Failed to download assets");
        SubutaiLauncher::Session::instance()->getDownloader()->reset();
    }
    gatherSystemInfoImpl();
    //    _assetsReady = true;
}

void UIApplication::gatherSystemInfoImpl()
{
    SubutaiLauncher::Environment env;
    auto hub = SubutaiLauncher::Session::instance()->getHub();
    // Send launcher information
    if (hub == nullptr) return;
    std::string pCoresNum = Poco::format("%u", env.cpuNum());
    hub->addInfo(SI_LAUNCHER_VERSION, LAUNCHER_VERSION);
    hub->addInfo(SI_OS_NAME, env.versionOS());
    hub->addInfo(SI_CPU_ARCH, env.cpuArch());
    hub->addInfo(SI_CORE_NUM, pCoresNum);
    //hub->addInfo(SI_SYSTEM_INFO, env.getSystemInfo());
    //hub->addInfo(SI_IP, env.getNetworkConfiguration());
    //hub->addInfo(SI_NETSTAT, env.getNetstat());
}

void launcherTerminate()
{
	Poco::Logger::get("subutai").fatal("Unhandled exception");
	//std::printf("Unhandled exception");
	//std::abort();
	std::exit(0);
}

static juce::JUCEApplicationBase* juce_CreateApplication() { return new UIApplication(); }
#if LAUNCHER_WINDOWS
int __stdcall WinMain(struct HINSTANCE__*, struct HINSTANCE__*, char*, int)
#else
int main(int argc, char* argv[])
#endif
{
	std::set_terminate(launcherTerminate);
	juce::JUCEApplicationBase::createInstance = &juce_CreateApplication;
	return juce::JUCEApplicationBase::main(JUCE_MAIN_FUNCTION_ARGS);
}

//START_JUCE_APPLICATION (UIApplication)
