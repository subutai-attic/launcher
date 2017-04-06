#ifndef BANNERCOMPONENT_H_INCLUDED
#define BANNERCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

class BannerComponent : public juce::Component {
    public:
        BannerComponent();
        ~BannerComponent();
    private:

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BannerComponent)
};


#endif  // BANNERCOMPONENT_H_INCLUDED
