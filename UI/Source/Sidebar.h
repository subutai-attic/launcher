/*
  ==============================================================================

    Sidebar.h
    Created: 23 Aug 2016 4:21:16pm
    Author:  crioto

  ==============================================================================
*/

#ifndef SIDEBAR_H_INCLUDED
#define SIDEBAR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

#include "SocialMediaIcons.h"

class SidebarComponent : public Component {
    public:
        SidebarComponent();
        ~SidebarComponent();
        void paint (Graphics&) override;
        void resized() override;
    private:
        SocialMediaIcons* _social;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SidebarComponent)
};



#endif  // SIDEBAR_H_INCLUDED
