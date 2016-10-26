/*
  ==============================================================================

    Logo.h
    Created: 18 Aug 2016 7:13:25pm
    Author:  crioto

  ==============================================================================
*/

#ifndef LOGO_H_INCLUDED
#define LOGO_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

class Logo : public Component {
    public:
        Logo();
        void paint(Graphics &g) override;
        void resized() override;
        Path getLogo();

    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Logo)
};

#endif  // LOGO_H_INCLUDED
