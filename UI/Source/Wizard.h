#ifndef __WIZARD_H__
#define __WIZARD_H__

#include "../JuceLibraryCode/JuceHeader.h"
#include "Poco/Process.h"
#include "Poco/File.h"
#include "Poco/Environment.h"
#include "Poco/Logger.h"
#include "WizardIntro.h"
#include "SystemCheck.h"
#include "ComponentChooser.h"
#include "WizardInstall.h"
#include "RootProcess.h"
#include "WizardFinish.h"

class Wizard : 
    public juce::Component,
    public juce::ButtonListener
{
    public:
        Wizard();
        ~Wizard();
        void paint (juce::Graphics&) override;
        void resized() override;
        void buttonClicked(juce::Button* button) override;
        void runInstall();
        void stepCompleted(const std::string& name);
        void runCancelConfirmation();
        void finish();
    private:
        Poco::Logger* _logger;
        int _step;
        juce::Label _stepIntro;
        juce::Label _stepSystemCheck;
        juce::Label _stepComponentChooser;
        juce::Label _stepInstall;
        juce::Label _stepFinal;
        juce::Label _cancelMessage;
        juce::TextButton _next;
        juce::TextButton _cancel;
        juce::TextButton _back;
        WizardIntro* _introPage;
        SystemCheck* _systemCheckPage;
        ComponentChooser* _componentChooserPage;
        WizardInstall* _ptpInstall;
        WizardInstall* _trayInstall;
        WizardInstall* _eteInstall;
        WizardInstall* _peerInstall;
        WizardFinish* _finishPage;

        bool _ptpInstalled;
        bool _eteInstalled;
        bool _peerInstalled;
        bool _trayInstalled;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Wizard)
};

#endif
