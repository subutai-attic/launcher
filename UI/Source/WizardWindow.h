#ifndef __WIZARD_WINDOW_H__
#define __WIZARD_WINDOW_H__

#include "../JuceLibraryCode/JuceHeader.h"
#include "Wizard.h"
#include "Poco/Logger.h"

class WizardWindow : public juce::DialogWindow
{
    public:
        WizardWindow();
        ~WizardWindow();
        void closeButtonPressed();
        bool escapeKeyPressed();
    private:
        Poco::Logger* _logger;
        Wizard* _wizard;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WizardWindow)
};

#endif
