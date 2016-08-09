/*
  ==============================================================================

    CommunityComponent.h
    Created: 8 Aug 2016 2:15:00pm
    Author:  crioto

  ==============================================================================
*/

#ifndef COMMUNITYCOMPONENT_H_INCLUDED
#define COMMUNITYCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

class CommunityComponent : public Component {
    public:
        CommunityComponent();
        ~CommunityComponent();
        void paint (Graphics&) override;
        void resized() override;
    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CommunityComponent)
};




#endif  // COMMUNITYCOMPONENT_H_INCLUDED
