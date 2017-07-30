#include "HubComponent.h"

HubComponent::HubComponent() 
{
    std::string pDir = SubutaiLauncher::Session::instance()->getSettings()->getTmpPath();
    _robot.setImage(juce::ImageCache::getFromFile(juce::File(pDir + "launcher-robot.png")));
    _robot.setBounds(getWidth() / 2 - 128, getHeight() / 2 - 83, 256, 167);
    addAndMakeVisible(_robot);
}

HubComponent::~HubComponent() 
{

}

void HubComponent::paint(Graphics& g) 
{
    g.setFont(Font("Encode Sans", 23, 1));
    g.setColour (Colour::fromRGB(105, 116, 144));
    g.drawText ("We are launching soon!", Rectangle<int>(0, getHeight() / 2 + 115, getWidth(), 40), Justification::centred, true);
}

void HubComponent::resized() 
{
    _robot.setBounds(getWidth() / 2 - 128, getHeight() / 2 - 83, 256, 167);
}
