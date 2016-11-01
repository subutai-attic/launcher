/*
  ==============================================================================

    SocialMediaIcons.h
    Created: 23 Aug 2016 7:32:40pm
    Author:  crioto

  ==============================================================================
*/

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
        juce::Path getFacebook();
        juce::Path getTwitter();
        juce::Path getLinkedin();
        juce::Path getGithub();

        juce::DrawableButton* _facebook;
        juce::DrawableButton* _twitter;
        juce::DrawableButton* _linkedin;
        juce::DrawableButton* _github;

        juce::HyperlinkButton _facebookLink;
        juce::HyperlinkButton _twitterLink;
        juce::HyperlinkButton _linkedinLink;
        juce::HyperlinkButton _githubLink;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SocialMediaIcons)
};



#endif  // SOCIALMEDIAICONS_H_INCLUDED
