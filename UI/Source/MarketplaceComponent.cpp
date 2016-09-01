/*
  ==============================================================================

    MarketplaceComponent.cpp
    Created: 8 Aug 2016 2:15:22pm
    Author:  crioto

  ==============================================================================
*/

#include "MarketplaceComponent.h"

MarketplaceComponent::MarketplaceComponent() {

}

MarketplaceComponent::~MarketplaceComponent() {

}

void MarketplaceComponent::paint(Graphics& g) {
    g.fillAll (Colour (0xff222222));

    //g.setColour(Colour(0xff222222));
    //g.fillRect(Rectangle<int>(0, 0, 1024, 30));

    g.setFont (Font (16.0f));
    g.setColour (Colours::white);
    g.drawText ("Coming soon", getLocalBounds(), Justification::centred, true);
    // Banner rotator

}

void MarketplaceComponent::resized() {

}
