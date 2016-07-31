/*
   ==============================================================================

   This file was auto-generated!

   ==============================================================================
   */

#include "MainComponent.h"


//==============================================================================
MainContentComponent::MainContentComponent()
{
    // TODO: Add screen detection size and relative value at startup
    setSize (1024, 768);
}

MainContentComponent::~MainContentComponent()
{
    //_login->deleteAndZero();
    _login->removeFromDesktop();
    delete(_login);
}

void MainContentComponent::paint (Graphics& g)
{
    g.fillAll (Colour (0xff222222));

    g.setFont (Font (16.0f));
    g.setColour (Colours::white);
    g.drawText ("Hello World!", getLocalBounds(), Justification::centred, true);
    showLoginScreen();
}

void MainContentComponent::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
}

void MainContentComponent::showLoginScreen() {
    _login = new LoginScreen();
    _login->addToDesktop(ComponentPeer::windowIsTemporary);
    Rectangle<int> area (0, 0, 200, 300);

    RectanglePlacement placement (RectanglePlacement::xMid
            | RectanglePlacement::yMid
            | RectanglePlacement::doNotResize);

    Rectangle<int> result (placement.appliedTo (area, Desktop::getInstance().getDisplays() 
                .getMainDisplay().userArea.reduced (20)));
    _login->setBounds (result);

    _login->setVisible(true);
}
