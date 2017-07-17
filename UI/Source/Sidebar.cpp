#include "Sidebar.h"
#include "MainComponent.h"

SidebarComponent::SidebarComponent()
{
    _social = new SocialMediaIcons();
    _social->setBounds(0, getParentHeight()-20-40, getParentWidth(), 40);
    _social->toFront(true);
    addAndMakeVisible(_social);
    resized();
}

SidebarComponent::~SidebarComponent() {
    delete _social;
}

void SidebarComponent::paint(Graphics& g) {
    //g.setGradientFill(ColourGradient(Colour(0xff16243b), 0, 0, Colour(0xff34557c), 250, 768-20, false));
    //g.fillAll(Colour(0xff222222));
    //g.fillAll (Colour::greyLevel (0.2f));
    g.fillAll(Colour::fromRGB(19, 31, 51));
}

void SidebarComponent::resized() {
    _social->setBounds(0, getParentHeight()-20-40, getParentWidth(), 40);
}

