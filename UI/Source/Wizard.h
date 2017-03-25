#ifndef __WIZARD_H__
#define __WIZARD_H__

#include "../JuceLibraryCode/JuceHeader.h"
#include "WizardIntro.h"
#include "SystemCheck.h"

class Wizard : public juce::Component,
    public juce::ButtonListener
{
    public:
        Wizard();
        ~Wizard();
        void paint (juce::Graphics&) override;
        void resized() override;
        void buttonClicked(juce::Button* button);
    private:
        int _step;
        juce::Label _stepIntro;
        juce::Label _stepSystemCheck;
        juce::Label _stepComponentChooser;
        juce::Label _stepInstall;
        juce::Label _stepFinal;
        juce::TextButton _next;
        juce::TextButton _cancel;
        juce::TextButton _back;
        WizardIntro* _introPage;
        SystemCheck* _systemCheckPage;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Wizard)
};

#endif
