/*
  ==============================================================================

    HubComponent.cpp
    Created: 8 Aug 2016 2:15:07pm
    Author:  crioto

  ==============================================================================
*/

#include "HubComponent.h"

HubComponent::HubComponent() {

}

HubComponent::~HubComponent() {

}

void HubComponent::paint(Graphics& g) {
    g.fillAll (Colour (0xff222222));

    //g.setColour(Colour(0xff222222));
    //g.fillRect(Rectangle<int>(0, 0, 1024, 30));

    g.setFont (Font (16.0f));
    g.setColour (Colours::white);
    g.drawText ("Hub", getLocalBounds(), Justification::centred, true);
    // Banner rotator

}

void HubComponent::resized() {

}
