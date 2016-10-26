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

class SocialMediaIcons : public Component, public ButtonListener {
    public:
        SocialMediaIcons();
        ~SocialMediaIcons();
        void paint (Graphics&) override;
        void resized() override;
        void buttonClicked(Button* button) override;

    private:
        Path getFacebook();
        Path getTwitter();
        Path getLinkedin();
        Path getGithub();

        DrawableButton* _facebook;
        DrawableButton* _twitter;
        DrawableButton* _linkedin;
        DrawableButton* _github;

        HyperlinkButton _facebookLink;
        HyperlinkButton _twitterLink;
        HyperlinkButton _linkedinLink;
        HyperlinkButton _githubLink;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SocialMediaIcons)
};



#endif  // SOCIALMEDIAICONS_H_INCLUDED
