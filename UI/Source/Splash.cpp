#include "Splash.h"

SplashComponent::SplashComponent()
{
    std::string pLogoFile(SubutaiLauncher::Session::instance()->getSettings()->getTmpPath() + "launcher-logo.png");
    Poco::File pLogo(pLogoFile);
    if (pLogo.exists())
    {
        _logo.setImage(juce::ImageCache::getFromFile(juce::File(pLogoFile)));
    }
    _logo.setBounds(0, 0, 60, 60);
    addAndMakeVisible(_logo);
}

SplashComponent::~SplashComponent()
{
}

void SplashComponent::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::transparentWhite);
}

void SplashComponent::resized()
{

}

Splash::Splash(juce::String name) : juce::DocumentWindow(name, 
        juce::Colours::transparentWhite, 
        juce::DocumentWindow::allButtons)
{
    _logger = &Poco::Logger::get("subutai");

    _logger->information("Starting splash screen");
    setUsingNativeTitleBar (false);
    setTitleBarHeight(0);

    _splash = new SplashComponent();
    setContentOwned(_splash, true);
    centreWithSize(getWidth(), getHeight());
    setVisible(true);
    setSize(60, 60);
}

Splash::~Splash()
{
    delete _splash;
}

void Splash::closeButtonPressed()
{

}
