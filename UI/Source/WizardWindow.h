#ifndef __WIZARD_WINDOW_H__
#define __WIZARD_WINDOW_H__

#include "../JuceLibraryCode/JuceHeader.h"
#include "Wizard.h"

class WizardWindow : public juce::DocumentWindow
{
    public:
        WizardWindow();
        ~WizardWindow();
        void closeButtonPressed();
    private:
        Wizard* _wizard;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WizardWindow)
};

#endif
