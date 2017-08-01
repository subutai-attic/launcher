#ifndef MARKETPLACECOMPONENT_H_INCLUDED
#define MARKETPLACECOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "SubutaiLauncher.h"

class MarketplaceComponent : public juce::Component 
{
    public:
        MarketplaceComponent();
        ~MarketplaceComponent();
        void paint (juce::Graphics&) override;
        void resized() override;
    private:
        juce::ImageComponent _robot;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MarketplaceComponent)
};

#endif  // MARKETPLACECOMPONENT_H_INCLUDED
