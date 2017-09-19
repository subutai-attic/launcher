#ifndef __CRASH_H__
#define __CRASH_H__

#include "../JuceLibraryCode/JuceHeader.h"
#include "Poco/Logger.h"

class Crash : 
    public juce::Component,
    public juce::ButtonListener
{
    public:
        Crash();
        ~Crash();
        void paint (juce::Graphics&) override;
        void resized() override;
        void buttonClicked(juce::Button* button) override;
    private:
        Poco::Logger* _logger;
        juce::ImageComponent _crashBot;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Crash);
};

#endif
