#ifndef SOCIALMEDIAICONS_H_INCLUDED
#define SOCIALMEDIAICONS_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

class SocialMediaIcons : public juce::Component, public juce::ButtonListener {
    public:
        SocialMediaIcons();
        ~SocialMediaIcons();
        void paint (juce::Graphics&) override;
        void resized() override;
        void buttonClicked(juce::Button* button) override;

    private:
        juce::ImageButton _facebookButton;
        juce::ImageButton _twitterButton;
        juce::ImageButton _linkedinButton;
        juce::ImageButton _githubButton;

        juce::HyperlinkButton _facebookLink;
        juce::HyperlinkButton _twitterLink;
        juce::HyperlinkButton _linkedinLink;
        juce::HyperlinkButton _githubLink;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SocialMediaIcons)
};



#endif  // SOCIALMEDIAICONS_H_INCLUDED
