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
#include <map>
#include <string>

#include "../JuceLibraryCode/JuceHeader.h"
#include "LibraryActionThread.h"

#include "InstallationDialog.h"
#include "ConfigurationManager.h"
#include "Vars.h"
#include "Session.h"
#include "SubutaiProcess.h"
#include "LibraryItem.h"
#include "LibraryItemTab.h"
#include "LibrarySystemCheck.h"
#include "WizardWindow.h"

/*
   typedef enum {
   INTRO,
   SYSTEM_CHECK,
   COMPONENT_CHOOSER,
   SYSTEM_CONFIGURE,
   DOWNLOAD,
   PREINSTALL,
   INSTALL,
   POSTINSTALL,
   FINISHED,
   UNINSTALL
   } InstallStep;
   */

typedef enum {
    INTRO,
    SYSTEM_CHECK,
    COMPONENT_CHOOSER,
    SYSTEM_CONFIGURE,
    INSTALL,
    FINISHED,
} InstallStep;

//class LibrarySystemCheck;
class LibrarySystemConfigure;
class LibraryDownload;
//class LibraryPreinstall;
class LibraryInstall;

//bool instEnabled;

//==============================================================================

class LibraryComponent : public juce::Component, 
    public juce::ComponentListener,
    public juce::ButtonListener, 
    public juce::TextEditorListener 
{
    public:
        LibraryComponent();
        ~LibraryComponent();
        void paint (juce::Graphics&) override;
        void resized() override;
        void buttonClicked(juce::Button* button);
        void tabClick(const std::string& tabTitle);

        /*  
        void componentVisibilityChanged(juce::Component *component);
        void textEditorFocusLost(juce::TextEditor &teditor);
        //void textEditorTextChanged(juce::TextEditor &teditor);

        void drawSystemCheck();
        void drawSystemConfigure();
        void drawDownload();
        void drawPreinstall();
        void drawInstall();
        void drawPostInstall();
        void drawFinished();
        void drawIntro();
        void hideIntro();
        void onStepChange();
        void drawProgressButtons(bool next = true, bool back = true, bool cancel = true);
        void nextStep();
        void previousStep();
        void collectInstallData();

        LibrarySystemCheck* _systemCheck;
        LibrarySystemConfigure* _systemConfigure;
        LibraryDownload* _download;
        //LibraryPreinstall* _preinstall;
        LibraryInstall* _install;
        */

    private:
        std::thread waitDownloadComplete();
        void waitDownloadCompleteImpl();
    private:
        juce::Label _componentsSectionLabel;
        juce::Label _peersSectionLabel;
        /*
           LibrarySystemCheck* _systemCheck;
           LibrarySystemConfigure* _systemConfigure;
           LibraryDownload* _download;
        //LibraryPreinstall* _preinstall;
        LibraryInstall* _install;
        */
        /*  
        InstallStep _step;
        juce::TextButton _installButton;
        juce::TextButton _nextButton;
        juce::TextButton _backButton;
        juce::TextButton _cancelButton;
        */
        juce::TextButton _installButton;
        std::vector<LibraryItem*> _components;
        std::vector<LibraryItemTab*> _componentTabs;
        std::vector<LibraryItem*> _peers;
        LibraryItem* _tb;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LibraryComponent)
};

// ============================================================================


// ============================================================================

class LibrarySystemConfigure : public juce::Component, public juce::ComponentListener, 
    public juce::ButtonListener, public juce::TextEditorListener
{
    //class LibrarySystemConfigure : public juce::Component    {
    public:
        LibrarySystemConfigure();
        ~LibrarySystemConfigure();
        void buttonClicked(juce::Button *button);
        void paint(juce::Graphics& g) override;
        void resized() override;
        bool isPeerInstallChecked();
        void setConfigVersion(juce::Button *button);
        void textEditorFocusLost(juce::TextEditor &teditor);
        void textEditorReturnKeyPressed(juce::TextEditor &teditor) ;
        void textEditorTextChanged(juce::TextEditor &teditor);
        void fillInstallList();

    private:
        // Install with peer
        juce::Label _installConfField;
        juce::ToggleButton* _installTray;

        // Install master or dev
        juce::Label _installTypeField;
        juce::ToggleButton* _installProd;
        juce::ToggleButton* _installMaster;
        juce::ToggleButton* _installDev;

        std::vector<juce::ToggleButton*> _installTypes;

        // Installation path
        juce::Label _installPathField;
        juce::TextEditor* _installPathValue;

        // Tmp directory
        juce::Label _installTmpField;
        juce::TextEditor* _installTmpValue;

        // vmCores
        juce::Label _installCoresField;
        juce::TextEditor* _installCoresValue;

        // vmRAM
        juce::Label _installRamField;
        juce::TextEditor* _installRamValue;

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

class LibraryPreinstall : public juce::Component {
    public:
        LibraryPreinstall();
        ~LibraryPreinstall();
        void paint(juce::Graphics& g) override;
        //void resized() override;
        //int calculateTotalSize();
        //void updateProgress(long p);
        //std::thread preinstall();
        //void preinstallImpl();
        //void start();
        //bool isComplete();
        //bool isCanceled();
        //void setWithPeer(bool withPeer);

    private:
        juce::Label _lstep;
        juce::Label _currentAction;
        juce::Label _sizeProgress;
        juce::Slider* _progressBar;
        //SubutaiLauncher::Preinstaller* _preinstaller;
        //bool _withPeer;
        //bool _inProgress;
        //bool _isPreinstallComplete;
        //bool _isCanceled;
        //int _totalSize;
        //long _progress;

        std::vector<std::string> _componentList;

        //JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LibraryPreinstall)

};

// ============================================================================

class LibraryInstall : 	public juce::Component, 
    public juce::ComponentListener,
    public juce::ButtonListener,
    public juce::TextEditorListener {
        public:
            LibraryInstall();
            ~LibraryInstall();
            void paint(juce::Graphics& g) override;
            void addLine(std::string fieldText, std::string valueText, std::string hintText, bool inst);
            void installComponents();
            void installPeer();
            void buttonClicked(juce::Button* button);
            void showChangedInfo();
            void componentVisibilityChanged(juce::Component &component);
            void resized() override;

        private:
            // Install with peer
            juce::Label _lstep;
            juce::Label _lField;
            juce::Label _lValue;
            juce::Label _lHint;
            juce::TextButton _confirmButton;

            int _numLines;

            JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LibraryInstall)

    };

// ============================================================================

#endif  // LIBRARYCOMPONENT_H_INCLUDED
