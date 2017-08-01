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
        juce::ImageComponent _robot;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HubComponent)
};

#endif  // HUBCOMPONENT_H_INCLUDED
