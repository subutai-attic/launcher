/*
  ==============================================================================

    MarketplaceComponent.h
    Created: 8 Aug 2016 2:15:22pm
    Author:  crioto

  ==============================================================================
*/

#ifndef MARKETPLACECOMPONENT_H_INCLUDED
#define MARKETPLACECOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

class MarketplaceComponent : public juce::Component {
    public:
        MarketplaceComponent();
        ~MarketplaceComponent();
        void paint (juce::Graphics&) override;
        void resized() override;
    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MarketplaceComponent)
};




#endif  // MARKETPLACECOMPONENT_H_INCLUDED
