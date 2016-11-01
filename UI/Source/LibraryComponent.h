/*
   ==============================================================================

   LibraryComponent.h
Created: 5 Aug 2016 5:58:23pm
Author:  crioto

==============================================================================
*/

#ifndef LIBRARYCOMPONENT_H_INCLUDED
#define LIBRARYCOMPONENT_H_INCLUDED

#include "Vars.h"

#if LAUNCHER_LINUX
#include <unistd.h>
#endif
#include <thread>

#include "../JuceLibraryCode/JuceHeader.h"

#include "InstallationDialog.h"
#include "ConfigurationManager.h"
#include "Vars.h"
#include "Session.h"

typedef enum {
    INTRO,
    SYSTEM_CHECK,
    COMPONENT_CHOOSER,
    SYSTEM_CONFIGURE,
    DOWNLOAD,
    PREINSTALL,
    INSTALL,
    POSTINSTALL,
    FINISHED
} InstallStep;

class LibrarySystemCheck;
class LibrarySystemConfigure;
class LibraryDownload;

//==============================================================================
class DemoBackgroundThread  : public juce::ThreadWithProgressWindow
{
    private:
        std::string _process;
        std::string _component;
        std::string _title;
        bool _error;
public:
    DemoBackgroundThread(const std::string& process, const std::string& component, const std::string& title) :
        _process(process),
        _component(component),
        _title(title),
        ThreadWithProgressWindow (title, true, true)
    {
        setStatusMessage ("Getting ready...");
    }

    void run() override
    {
        setProgress (-1.0); // setting a value beyond the range 0 -> 1 will show a spinning bar..
        setStatusMessage ("Download installation script");

        auto conf = SubutaiLauncher::Session::instance()->getConfManager();

        auto configs = conf->getConfigs();
        SubutaiLauncher::InstallConfig config;
        bool found = false;
        for (auto it = configs.begin(); it != configs.end(); it++)
        {
            if ((*it).title == _component)
            {
                config = (*it);
                found = true;
                break;
            }
        }

        if (!found)
        {
            _error = true;
            return;
        }

        while (1)
        {
            if (threadShouldExit())
                return;
        }

/* 
        auto d = SubutaiLauncher::Session::instance()->getDownloader();
        auto file = std::string(config.file);
        file.append(".py");
        d->setFilename(file);
        while (!d->isDone())
        {
            if (threadShouldExit())
                return;
        }

        SL sl(d->getOutputDirectory());
        sl.open(config.file);
        sl.execute();

        setStatusMessage("Downloading necessary files");

        while (!d->isDone())
        {

        }
        */

        return;
        
        //wait (1000);

        const int thingsToDo = 10;

        for (int i = 0; i < thingsToDo; ++i)
        {
            // must check this as often as possible, because this is
            // how we know if the user's pressed 'cancel'
            if (threadShouldExit())
                return;

            // this will update the progress bar on the dialog box
            setProgress (i / (double) thingsToDo);

            setStatusMessage (juce::String (thingsToDo - i) + " things left to do...");

            wait (500);
        }

        setProgress (-1.0); // setting a value beyond the range 0 -> 1 will show a spinning bar..
        setStatusMessage ("Finishing off the last few bits and pieces!");
        wait (2000);
    }

    // This method gets called on the message thread once our thread has finished..
    void threadComplete (bool userPressedCancel) override
    {
        if (_error)
        {
            juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::WarningIcon,
                    "Error occured",
                    "Unknown error occured during process");
        } else if (userPressedCancel)
        {
            juce::AlertWindow::showMessageBoxAsync (juce::AlertWindow::WarningIcon,
                                              "Progress window",
                                              "You pressed cancel!");
        }
        else
        {
            // thread finished normally..
            juce::AlertWindow::showMessageBoxAsync (juce::AlertWindow::WarningIcon,
                                              "Progress window",
                                              "Thread finished ok!");
        }

        // ..and clean up by deleting our thread object..
        delete this;
    }
};

class LibraryItem : public juce::Component
                    //, public juce::MouseListener
{
    public:
        static const int WIDTH = 180;
        static const int HEIGHT = 200;
        LibraryItem(
                const std::string& title, 
                const std::string& desc, 
                const std::string& installScript = "", 
                const std::string& updateScript = "", 
                const std::string& removeScript = "");
        ~LibraryItem();
        void paint(juce::Graphics& g) override;
        void resized() override;
        void mouseUp(const juce::MouseEvent& e) override;
    private:
        std::string _title;
        std::string _desc;
        std::string _installScript;
        std::string _updateScript;
        std::string _removeScript;
        juce::Label _titleLabel;
        juce::Label _plusLabel;
        juce::Label _version;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LibraryItem)
};

class LibraryComponent : public juce::Component, public juce::ButtonListener 
{
    public:
        LibraryComponent();
        ~LibraryComponent();
        void paint (juce::Graphics&) override;
        void resized() override;
        void buttonClicked(juce::Button* button);
        void drawSystemCheck();
        void drawSystemConfigure();
        void drawDownload();
        void drawPreinstall();
        void drawInstall();
        void drawPostInstall();
        void drawFinished();
        void drawIntro();
        void onStepChange();
        void drawProgressButtons(bool next = true, bool back = true, bool cancel = true);
        void nextStep();
        void previousStep();
    private:
        std::thread waitDownloadComplete();
        void waitDownloadCompleteImpl();
    private:
        juce::Label _componentsSectionLabel;
        juce::Label _peersSectionLabel;
        LibrarySystemCheck* _systemCheck;
        LibrarySystemConfigure* _systemConfigure;
        LibraryDownload* _download;
        InstallStep _step;
        juce::TextButton _installButton;
        juce::TextButton _nextButton;
        juce::TextButton _backButton;
        juce::TextButton _cancelButton;
        std::vector<LibraryItem*> _components;
        std::vector<LibraryItem*> _peers;
        LibraryItem* _tb;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LibraryComponent)
};

// ============================================================================

class LibrarySystemCheck : public juce::Component {
    public:
        LibrarySystemCheck();
        ~LibrarySystemCheck();
        void paint(juce::Graphics& g) override;
        void resized();
        void addLine(juce::Label* field, juce::Label* value, juce::Label* hint, std::string text, std::string hintText);
    private:
        juce::Label _numCpuField;
        juce::Label _numCpuValue;
        juce::Label _numCpuHint;

        juce::Label _maxMemoryField;
        juce::Label _maxMemoryValue;
        juce::Label _maxMemoryHint;

        juce::Label _vboxField;
        juce::Label _vboxValue;
        juce::Label _vboxHint;

        juce::Label _vtxField;
        juce::Label _vtxValue;
        juce::Label _vtxHint;

        juce::Label _sshField;
        juce::Label _sshValue;
        juce::Label _sshHint;

        juce::Label _info;

        int _numLines;

        //TextButton _nextButton;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LibrarySystemCheck)
};

// ============================================================================

class LibrarySystemConfigure : public juce::Component {
    public:
        LibrarySystemConfigure();
        ~LibrarySystemConfigure();
        void paint(juce::Graphics& g) override;
        void resized() override;
        bool isPeerInstallChecked();
    private:
        // Install with peer
        juce::Label _installVmField;
        juce::ToggleButton* _installTray;
        juce::ToggleButton* _installVm;

        // Install master or dev
        juce::Label _installTypeField;
        juce::ToggleButton* _installMaster;
        juce::ToggleButton* _installDev;
        std::vector<juce::ToggleButton*> _installTypes;

        // Installation path
        juce::Label _installPathField;
        juce::TextEditor _installPathValue;

        // Tmp directory
        juce::Label _installTmpField;
        juce::TextEditor _installTmpValue;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LibrarySystemConfigure)
};

// ============================================================================

class LibraryDownload : public juce::Component {
    public:
        LibraryDownload(bool downloadPeer = false);
        ~LibraryDownload();
        void paint(juce::Graphics& g) override;
        void resized() override;
        long calculateTotalSize();
        void updateProgress(long p);
        std::thread download();
        void downloadImpl();
        void start();
        bool isComplete();
        bool isCanceled();
        void setWithPeer(bool withPeer);
    private:
        juce::Label _currentAction;
        juce::Label _sizeProgress;
        juce::Slider* _progressBar;
        SubutaiLauncher::Downloader* _downloader;
        bool _withPeer;
        bool _inProgress;
        bool _isDownloadComplete;
        bool _isCanceled;
        long _totalSize;
        long _progress;

        std::vector<std::string> _fileList;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LibraryDownload)
};

// ============================================================================

class LibraryInstall : public juce::Component {

};

// ============================================================================

#endif  // LIBRARYCOMPONENT_H_INCLUDED
