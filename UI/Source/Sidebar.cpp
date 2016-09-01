/*
  ==============================================================================

    Sidebar.cpp
    Created: 23 Aug 2016 4:21:16pm
    Author:  crioto

  ==============================================================================
*/

#include "Sidebar.h"

SidebarComponent::SidebarComponent() {
    _social = new SocialMediaIcons();
    addAndMakeVisible(_social);
    _social->setBounds(0, 768-20-40, 250, 40);
    _social->toFront(false);
}

SidebarComponent::~SidebarComponent() {

}

void SidebarComponent::paint(Graphics& g) {
    //g.setGradientFill(ColourGradient(Colour(0xff16243b), 0, 0, Colour(0xff34557c), 250, 768-20, false));
    //g.fillAll(Colour(0xff222222));
    g.fillAll (Colour::greyLevel (0.2f));
}

void SidebarComponent::resized() {

}
