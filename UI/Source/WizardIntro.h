#ifndef __WIZARD_INTRO_H__
#define __WIZARD_INTRO_H__

#include "../JuceLibraryCode/JuceHeader.h"
#include "Poco/Logger.h"

class WizardIntro : public juce::Component 
{
    public:
        WizardIntro();
        ~WizardIntro();
        void paint (juce::Graphics&) override;
        void resized() override;
    private:
        Poco::Logger* _logger;
        juce::DrawableText _text;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WizardIntro)
};

#endif
