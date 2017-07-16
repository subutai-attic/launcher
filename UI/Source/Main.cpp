#define DONT_SET_USING_JUCE_NAMESPACE 1

#include "../JuceLibraryCode/JuceHeader.h"
#include "Main.h"

UIApplication::UIApplication()
{
	try 
	{
		_core = new SubutaiLauncher::Core(std::vector<std::string>());
	}
	catch (Poco::OpenFileException& e)
	{
		std::printf("Couldn't open log file: %s", e.displayText().c_str());
	}
	try 
	{
		_core->run();
	}
    catch (Poco::OpenFileException& e)
    {
        Poco::Logger::get("subutai").error(e.displayText());
    }
	catch (Poco::FileException& e)
	{
		Poco::Logger::get("subutai").error(e.displayText());
	}
	catch (Poco::PathSyntaxException& e)
	{
		Poco::Logger::get("subutai").error(e.displayText());
	}
	SubutaiLauncher::Environment env;
	env.updatePath(SubutaiLauncher::Session::instance()->getSettings()->getInstallationPath() + "bin");

    SubutaiLauncher::AssetsManager pAssets;
    pAssets.verify();
}

const juce::String UIApplication::getApplicationName()
{
    return "Subutai Launcher";
}

const juce::String UIApplication::getApplicationVersion()
{
    return "4.0.15";
}

bool UIApplication::moreThanOneInstanceAllowed()
{
    return false;
}

void UIApplication::initialise(const juce::String& commandLine)
{
    std::string pTitle = "SubutaiLauncher ";
    pTitle.append(BUILD_SCHEME);
    _logger = &Poco::Logger::get("subutai");
    _logger->debug("Initializing UI: %s", pTitle);
    // This method is where you should put your application's initialisation code..
    //
    //
    //uid_t cuser = getuid();
    //if (cuser !=0) {

    //}

    mainWindow = new MainWindow(pTitle);
    _logger->debug("UI Initialization completed");
}

void UIApplication::shutdown()
{
    _logger->debug("Started UI shutdown procedure");
    delete _core;
    //delete mainWindow;
    mainWindow = nullptr;
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

START_JUCE_APPLICATION (UIApplication)
