/*
  ==============================================================================

    CommunityComponent.cpp
    Created: 8 Aug 2016 2:15:00pm
    Author:  crioto

  ==============================================================================
*/

#include "CommunityComponent.h"

CommunityComponent::CommunityComponent() {

}

CommunityComponent::~CommunityComponent() {

}

void CommunityComponent::paint(Graphics& g) {
    g.fillAll (Colour (0xff222222));

    //g.setColour(Colour(0xff222222));
    //g.fillRect(Rectangle<int>(0, 0, 1024, 30));

    g.setFont (Font (16.0f));
    g.setColour (Colours::white);
    g.drawText ("Community", getLocalBounds(), Justification::centred, true);
    // Banner rotator

}

void CommunityComponent::resized() {

}
