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
#include "Core.h"

#include "../JuceLibraryCode/JuceHeader.h"

class CommunityComponent : public Component {
    public:
        CommunityComponent();
        ~CommunityComponent();
        void paint (Graphics&) override;
        void resized() override;
    private:
        ImageComponent _mainBanner;
        Label _mainLabel;
        ImageComponent _banner1;
        ImageComponent _banner2;
        ImageComponent _banner3;
        Label _label1;
        Label _label2;
        Label _label3;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CommunityComponent)
};

#endif  // COMMUNITYCOMPONENT_H_INCLUDED
