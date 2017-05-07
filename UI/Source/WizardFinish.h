#ifndef __WIZARD_FINISH_H__
#define __WIZARD_FINISH_H__

#include "../JuceLibraryCode/JuceHeader.h"
#include "../JuceLibraryCode/modules/juce_gui_basics/juce_gui_basics.h"
#include "Poco/Logger.h"

class WizardFinish : 
    public juce::Component,
    public juce::ButtonListener
{
    public:
        WizardFinish();
        ~WizardFinish();
        void paint (juce::Graphics&) override;
        void resized() override;
        void buttonClicked(juce::Button* button) override;
    private:
        Poco::Logger* _logger;
        juce::Label _thanks;
        juce::TextButton _finish;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WizardFinish)
};

#endif
