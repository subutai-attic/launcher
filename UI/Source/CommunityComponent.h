/*
   ==============================================================================

   CommunityComponent.h
Created: 8 Aug 2016 2:15:00pm
Author:  crioto

==============================================================================
*/

#ifndef COMMUNITYCOMPONENT_H_INCLUDED
#define COMMUNITYCOMPONENT_H_INCLUDED

#include <vector>

#include "../JuceLibraryCode/JuceHeader.h"

class CommunityComponent : public juce::Component {
    public:
        CommunityComponent();
        ~CommunityComponent();
        void paint (juce::Graphics&) override;
        void resized() override;
    private:
        juce::ImageComponent _mainBanner;
        juce::Label _mainLabel;
        juce::ImageComponent _banner1;
        juce::ImageComponent _banner2;
        juce::ImageComponent _banner3;
        juce::Label _label1;
        juce::Label _label2;
        juce::Label _label3;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CommunityComponent)
};

#endif  // COMMUNITYCOMPONENT_H_INCLUDED
