/*
   ==============================================================================

   Main.h
Created: 18 Aug 2016 6:45:47pm
Author:  crioto

==============================================================================
*/

#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

#include "Core.h"
#include "MainWindow.h"

//==============================================================================
class UIApplication  : public JUCEApplication
{
    public:
        //==============================================================================
        UIApplication() {}

        const String getApplicationName() override       { return ProjectInfo::projectName; }
        const String getApplicationVersion() override    { return ProjectInfo::versionString; }
        bool moreThanOneInstanceAllowed() override       { return true; }

        //==============================================================================
        void initialise (const String& commandLine) override
        {
            // This method is where you should put your application's initialisation code..

            _core = new SubutaiLauncher::Core(std::vector<std::string>());
            _core->run();

            mainWindow = new MainWindow (getApplicationName());
        }

        void shutdown() override
        {
            // Add your application's shutdown code here..

            mainWindow = nullptr; // (deletes our window)
        }

        //==============================================================================
        void systemRequestedQuit() override
        {
            // This is called when the app is being asked to quit: you can ignore this
            // request and let the app carry on running, or call quit() to allow the app to close.
            quit();
        }

        void anotherInstanceStarted (const String& commandLine) override
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
        ScopedPointer<MainWindow> mainWindow;
        SubutaiLauncher::Core* _core;
};




#endif  // MAIN_H_INCLUDED
