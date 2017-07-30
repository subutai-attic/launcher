#include "MarketplaceComponent.h"

MarketplaceComponent::MarketplaceComponent() 
{
    std::string pDir = SubutaiLauncher::Session::instance()->getSettings()->getTmpPath();
    _robot.setImage(juce::ImageCache::getFromFile(juce::File(pDir + "launcher-robot.png")));
    _robot.setBounds(getWidth() / 2 - 128, getHeight() / 2 - 83, 256, 167);
    addAndMakeVisible(_robot);
}

MarketplaceComponent::~MarketplaceComponent() 
{

}

void MarketplaceComponent::paint(Graphics& g) 
{
    g.setFont(Font("Encode Sans", 23, 1));
    g.setColour(Colour::fromRGB(105, 116, 144));
    g.drawText("We are launching soon!", Rectangle<int>(0, getHeight() / 2 + 115, getWidth(), 40), Justification::centred, true);
}

void MarketplaceComponent::resized() 
{
    _robot.setBounds(getWidth() / 2 - 128, getHeight() / 2 - 83, 256, 167);
}
