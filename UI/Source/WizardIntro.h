#ifndef __WIZARD_INTRO_H__
#define __WIZARD_INTRO_H__

#include "../JuceLibraryCode/JuceHeader.h"

class WizardIntro : public juce::Component 
{
    public:
        WizardIntro();
        ~WizardIntro();
        void paint (juce::Graphics&) override;
        void resized() override;
    private:
        juce::DrawableText _text;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WizardIntro)
};

#endif
