#ifndef LOGO_H_INCLUDED
#define LOGO_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

#include "Poco/Logger.h"

class Logo : public juce::Component {
    public:
        Logo();
        void paint(juce::Graphics &g) override;
        void resized() override;
        juce::Path getLogo();
        juce::Image* getImage();

    private:
        Poco::Logger* _logger;
        juce::Image _logo;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Logo)
};

#endif  // LOGO_H_INCLUDED
