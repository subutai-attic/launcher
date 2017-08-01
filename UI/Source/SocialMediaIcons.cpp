#include "SocialMediaIcons.h"

SocialMediaIcons::SocialMediaIcons() 
{
    std::string pDir = SubutaiLauncher::Session::instance()->getSettings()->getTmpPath();
    _facebookButton.setImages(false, false, true, 
           juce::ImageCache::getFromFile(juce::File(pDir + "launcher-facebook-inactive.png")),
           1.0f, juce::Colours::transparentWhite,
           juce::ImageCache::getFromFile(juce::File(pDir + "launcher-facebook-active.png")),
           1.0f, juce::Colours::transparentWhite,
           juce::ImageCache::getFromFile(juce::File(pDir + "launcher-facebook-active.png")),
           1.0f, juce::Colours::transparentWhite);
    _facebookButton.setBounds(50, 0, 30, 30);
    _facebookButton.addListener(this);

    addAndMakeVisible(_facebookButton);

    _twitterButton.setImages(false, false, true, 
           juce::ImageCache::getFromFile(juce::File(pDir + "launcher-twitter-inactive.png")),
           1.0f, juce::Colours::transparentWhite,
           juce::ImageCache::getFromFile(juce::File(pDir + "launcher-twitter-active.png")),
           1.0f, juce::Colours::transparentWhite,
           juce::ImageCache::getFromFile(juce::File(pDir + "launcher-twitter-active.png")),
           1.0f, juce::Colours::transparentWhite);
    _twitterButton.setBounds(90, 0, 30, 30);
    _twitterButton.addListener(this);

    addAndMakeVisible(_twitterButton);

    _githubButton.setImages(false, false, true, 
           juce::ImageCache::getFromFile(juce::File(pDir + "launcher-github-inactive.png")),
           1.0f, juce::Colours::transparentWhite,
           juce::ImageCache::getFromFile(juce::File(pDir + "launcher-github-active.png")),
           1.0f, juce::Colours::transparentWhite,
           juce::ImageCache::getFromFile(juce::File(pDir + "launcher-github-active.png")),
           1.0f, juce::Colours::transparentWhite);
    _githubButton.setBounds(130, 0, 30, 30);
    _githubButton.addListener(this);

    addAndMakeVisible(_githubButton);

    _linkedinButton.setImages(false, false, true, 
           juce::ImageCache::getFromFile(juce::File(pDir + "launcher-linkedin-inactive.png")),
           1.0f, juce::Colours::transparentWhite,
           juce::ImageCache::getFromFile(juce::File(pDir + "launcher-linkedin-active.png")),
           1.0f, juce::Colours::transparentWhite,
           juce::ImageCache::getFromFile(juce::File(pDir + "launcher-linkedin-active.png")),
           1.0f, juce::Colours::transparentWhite);
    _linkedinButton.setBounds(170, 0, 30, 30);
    _linkedinButton.addListener(this);

    addAndMakeVisible(_linkedinButton);


    addAndMakeVisible(_facebookLink);
    addAndMakeVisible(_twitterLink);
    addAndMakeVisible(_linkedinLink);
    addAndMakeVisible(_githubLink);

    _facebookLink.setBounds(-1, -1, 1, 1);
    _facebookLink.setURL(URL("https://www.facebook.com"));
    _twitterLink.setBounds(-1, -1, 1, 1);
    _twitterLink.setURL(URL("https://www.twitter.com"));
    _linkedinLink.setBounds(-1, -1, 1, 1);
    _linkedinLink.setURL(URL("https://www.linkedin.com"));
    _githubLink.setBounds(-1, -1, 1, 1);
    _githubLink.setURL(URL("https://www.github.com"));
}

SocialMediaIcons::~SocialMediaIcons() 
{
}

void SocialMediaIcons::buttonClicked(Button* button) 
{
    if (button == &_facebookButton) {
        _facebookLink.triggerClick();
    } else if (button == &_twitterButton) {
        _twitterLink.triggerClick();
    } else if (button == &_linkedinButton) {
        _linkedinLink.triggerClick();
    } else if (button == &_githubButton) {
        _githubLink.triggerClick();
    }
}

void SocialMediaIcons::paint(Graphics& g) {
}

void SocialMediaIcons::resized() {
    _facebookButton.setBounds(50, 0, 30, 30);
    _twitterButton.setBounds(90, 0, 30, 30);
    _linkedinButton.setBounds(130, 0, 30, 30);
    _githubButton.setBounds(170, 0, 30, 30);
}

