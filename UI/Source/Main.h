#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

#if LAUNCHER_LINUX || LAUNCHER_MACOS
#include <unistd.h>
#include <sys/types.h>
#endif

#include "MainWindow.h"
#include <Python.h>

//==============================================================================
class UIApplication  : public juce::JUCEApplication
{
    public:
        //==============================================================================
        UIApplication() {}

        const juce::String getApplicationName() override       { return ProjectInfo::projectName; }
        const juce::String getApplicationVersion() override    { return ProjectInfo::versionString; }
        bool moreThanOneInstanceAllowed() override       { return true; }

        //==============================================================================
        void initialise (const juce::String& commandLine) override
        {
            std::string pTitle = "SubutaiLauncher";
            pTitle.append(BUILD_SCHEME);
            // This method is where you should put your application's initialisation code..
            //
            //
            //uid_t cuser = getuid();
            //if (cuser !=0) {

            //}
            _core = new SubutaiLauncher::Core(std::vector<std::string>());
            _core->run();

            mainWindow = new MainWindow(pTitle);
        }

        void shutdown() override
        {
            // Add your application's shutdown code here..
            delete _core;
            mainWindow = nullptr; // (deletes our window)
        }

        //==============================================================================
        void systemRequestedQuit() override
        {
            // This is called when the app is being asked to quit: you can ignore this
            // request and let the app carry on running, or call quit() to allow the app to close.
            quit();
        }

        void anotherInstanceStarted (const juce::String& commandLine) override
        {
            // When another instance of the app is launched while this one is running,
            // this method is invoked, and the commandLine parameter tells you what
            // the other instance's command-line arguments were.
        }


        //==============================================================================
        /*
           This class implements the desktop window that contains an instance of
           our MainContentComponent class.
           */

    private:
        juce::ScopedPointer<MainWindow> mainWindow;
        SubutaiLauncher::Core* _core;
};




#endif  // MAIN_H_INCLUDED
