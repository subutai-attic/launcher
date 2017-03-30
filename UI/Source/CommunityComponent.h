#ifndef COMMUNITYCOMPONENT_H_INCLUDED
#define COMMUNITYCOMPONENT_H_INCLUDED

#include <vector>
#include "Poco/Logger.h"

#include "../JuceLibraryCode/JuceHeader.h"

class CommunityComponent : public juce::Component 
{
    public:
        CommunityComponent();
        ~CommunityComponent();
        void paint (juce::Graphics&) override;
        void resized() override;
    private:
        Poco::Logger* _logger;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CommunityComponent)
};

#endif  // COMMUNITYCOMPONENT_H_INCLUDED
