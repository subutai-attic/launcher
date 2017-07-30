#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

#if LAUNCHER_LINUX || LAUNCHER_MACOS
#include <unistd.h>
#include <sys/types.h>
#endif

#include "MainWindow.h"
#include "Splash.h"
#include "Poco/Logger.h"
#include <Python.h>

class UIApplication : public juce::JUCEApplication
{
    public:
        UIApplication();
        const juce::String getApplicationName() override;
        const juce::String getApplicationVersion() override;
        bool moreThanOneInstanceAllowed() override;
        void initialise (const juce::String& commandLine) override;
        void shutdown() override;
        void systemRequestedQuit() override;
        void anotherInstanceStarted (const juce::String& commandLine) override;
    private:
        void startMainWindow(const std::string& name);
        bool _assetsReady;
        std::thread getAssets(const std::string& title);
        void getAssetsImpl(const std::string& title);
        Poco::Logger* _logger;
        juce::ScopedPointer<Splash> splash;
        juce::ScopedPointer<MainWindow> mainWindow;
        SubutaiLauncher::Core* _core;
};

#endif  // MAIN_H_INCLUDED
