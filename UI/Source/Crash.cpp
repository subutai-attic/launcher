#include "Crash.h"

Crash::Crash()
{
    std::string pCrashBot(SubutaiLauncher::Session::instance()->getSettings()->getTmpPath() + "crashbot.png");
    Poco::File pLogo(pCrashBot);
    if (pLogo.exists())
    {
        _crashBot.setImage(juce::ImageCache::getFromFile(juce::File(pCrashBot)));
    }
    else
    {
        _logger->error("launcher-robot.png doesn't exists");
    }
    addAndMakeVisible(_crashBot);
    _crashBot.setBounds(100, 100, 200, 200);
}

Crash::~Crash()
{

}

void Crash::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour::fromRGB(247, 249, 252));
    g.setFont (Font (16.0f));
    g.setColour (juce::Colour::fromRGB(105, 116, 144));
}

void Crash::resized()
{

}

void Crash::buttonClicked(juce::Button* button)
{

}
