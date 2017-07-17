#include "MarketplaceComponent.h"

MarketplaceComponent::MarketplaceComponent() {

}

MarketplaceComponent::~MarketplaceComponent() {

}

void MarketplaceComponent::paint(Graphics& g) {
    g.fillAll (Colour (0xffffffff));

    g.setFont (Font (16.0f));
    g.setColour (Colours::white);
    g.drawText ("Coming soon", getLocalBounds(), Justification::centred, true);
}

void MarketplaceComponent::resized() {

}
