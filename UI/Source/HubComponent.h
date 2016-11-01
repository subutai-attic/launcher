/*
  ==============================================================================

    HubComponent.h
    Created: 8 Aug 2016 2:15:07pm
    Author:  crioto

  ==============================================================================
*/

#ifndef HUBCOMPONENT_H_INCLUDED
#define HUBCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

class HubComponent : public juce::Component {
    public:
        HubComponent();
        ~HubComponent();
        void paint (juce::Graphics&) override;
        void resized() override;
    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HubComponent)
};




#endif  // HUBCOMPONENT_H_INCLUDED
