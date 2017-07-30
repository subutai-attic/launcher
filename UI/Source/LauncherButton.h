#ifndef __LAUNCHER_BUTTON_H__
#define __LAUNCHER_BUTTON_H__

#include "../JuceLibraryCode/JuceHeader.h"

class LauncherButton : public juce::TextButton
{
    public:
        LauncherButton(const juce::String& buttonName);

    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LauncherButton)
};

#endif
