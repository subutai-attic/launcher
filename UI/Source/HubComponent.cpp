#include "HubComponent.h"

HubComponent::HubComponent() {

}

HubComponent::~HubComponent() {

}

void HubComponent::paint(Graphics& g) {
    
    //g.setGradientFill(ColourGradient(Colour(0xff16243b), 0, 0, Colour(0xff34557c), 1024-250, 768-20, false));
    g.fillAll (Colour (0xffffffff));

    //g.setColour(Colour(0xff222222));
    //g.fillRect(Rectangle<int>(0, 0, 1024, 30));

    g.setFont (Font (16.0f));
    g.setColour (Colours::white);
    g.drawText ("Coming soon", getLocalBounds(), Justification::centred, true);
    // Banner rotator

}

void HubComponent::resized() {

}
