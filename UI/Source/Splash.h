#ifndef __SPLASH_H__
#define __SPLASH_H__

#include "../JuceLibraryCode/JuceHeader.h"
#include "Poco/Logger.h"
#include "SubutaiLauncher.h"

class SplashComponent : public juce::Component
{
    public:
        SplashComponent();
        ~SplashComponent();
        void paint (juce::Graphics&) override;
        void resized() override;
    private:
        juce::ImageComponent _logo;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SplashComponent)
};

class Splash : public juce::DocumentWindow
{
    public:
        Splash(juce::String name);
        ~Splash();
        void closeButtonPressed() override;

    private:
        SplashComponent* _splash;
        Poco::Logger* _logger;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Splash)
};

#endif
