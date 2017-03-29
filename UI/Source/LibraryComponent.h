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

#endif  // LIBRARYCOMPONENT_H_INCLUDED
