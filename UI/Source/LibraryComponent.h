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
#include "Core.h"
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

class LibraryItem : public Component 
{
    public:
        static const int WIDTH = 200;
        static const int HEIGHT = 150;
        LibraryItem(const std::string& title, const std::string& desc);
        ~LibraryItem();
        void paint(Graphics& g) override;
        void resized() override;
    private:
        std::string _title;
        std::string _desc;
        Label _titleLabel;
        Label _plusLabel;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LibraryItem)
};

class LibraryComponent : public Component, public ButtonListener 
{
    public:
        LibraryComponent();
        ~LibraryComponent();
        void paint (Graphics&) override;
        void resized() override;
        void buttonClicked(Button* button);
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
        Label _componentsSectionLabel;
        Label _peersSectionLabel;
        LibrarySystemCheck* _systemCheck;
        LibrarySystemConfigure* _systemConfigure;
        LibraryDownload* _download;
        InstallStep _step;
        TextButton _installButton;
        TextButton _nextButton;
        TextButton _backButton;
        TextButton _cancelButton;
        std::vector<LibraryItem*> _components;
        std::vector<LibraryItem*> _peers;
        LibraryItem* _tb;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LibraryComponent)
};

// ============================================================================

class LibrarySystemCheck : public Component {
    public:
        LibrarySystemCheck();
        ~LibrarySystemCheck();
        void paint(Graphics& g) override;
        void resized();
        void addLine(Label* field, Label* value, Label* hint, std::string text, std::string hintText);
    private:
        Label _numCpuField;
        Label _numCpuValue;
        Label _numCpuHint;

        Label _maxMemoryField;
        Label _maxMemoryValue;
        Label _maxMemoryHint;

        Label _vboxField;
        Label _vboxValue;
        Label _vboxHint;

        Label _vtxField;
        Label _vtxValue;
        Label _vtxHint;

        Label _sshField;
        Label _sshValue;
        Label _sshHint;

        Label _info;

        int _numLines;

        //TextButton _nextButton;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LibrarySystemCheck)
};

// ============================================================================

class LibrarySystemConfigure : public Component {
    public:
        LibrarySystemConfigure();
        ~LibrarySystemConfigure();
        void paint(Graphics& g) override;
        void resized() override;
        bool isPeerInstallChecked();
    private:
        // Install with peer
        Label _installVmField;
        ToggleButton* _installTray;
        ToggleButton* _installVm;

        // Install master or dev
        Label _installTypeField;
        ToggleButton* _installMaster;
        ToggleButton* _installDev;
        std::vector<ToggleButton*> _installTypes;

        // Installation path
        Label _installPathField;
        TextEditor _installPathValue;

        // Tmp directory
        Label _installTmpField;
        TextEditor _installTmpValue;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LibrarySystemConfigure)
};

// ============================================================================

class LibraryDownload : public Component {
    public:
        LibraryDownload(bool downloadPeer = false);
        ~LibraryDownload();
        void paint(Graphics& g) override;
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
        Label _currentAction;
        Label _sizeProgress;
        Slider* _progressBar;
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

class LibraryInstall : public Component {

};

// ============================================================================

#endif  // LIBRARYCOMPONENT_H_INCLUDED
