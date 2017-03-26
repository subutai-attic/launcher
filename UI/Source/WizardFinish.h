#ifndef __WIZARD_FINISH_H__
#define __WIZARD_FINISH_H__

#include "../JuceLibraryCode/JuceHeader.h"

class WizardFinish : public juce::Component 
{
    public:
        WizardFinish();
        ~WizardFinish();
        void paint (juce::Graphics&) override;
        void resized() override;
    private:
        juce::Label _thanks;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WizardFinish)
};

#endif
