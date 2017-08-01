#ifndef __LAUNCHER_BUTTON_H__
#define __LAUNCHER_BUTTON_H__

#include "../JuceLibraryCode/JuceHeader.h"
#include "Poco/Logger.h"

class LauncherButton : public juce::TextButton
{
    public:
        LauncherButton(const juce::String& buttonName);
        //void paint(juce::Graphics &g) override;
        void resized() override;

    private:
        juce::String _name;
        Poco::Logger* _logger;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LauncherButton)
};

#endif
