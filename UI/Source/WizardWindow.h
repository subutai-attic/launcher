#ifndef __WIZARD_WINDOW_H__
#define __WIZARD_WINDOW_H__

#include "../JuceLibraryCode/JuceHeader.h"
#include "Wizard.h"
#include "Poco/Logger.h"

#if LIGHT_MODE
class WizardWindow : public juce::DocumentWindow
#else
class WizardWindow : public juce::DialogWindow
#endif
{
    public:
        WizardWindow(juce::String name);
        ~WizardWindow();
        void closeButtonPressed();
        bool escapeKeyPressed();
    private:
        Poco::Logger* _logger;
        Wizard* _wizard;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WizardWindow)
};

#endif
