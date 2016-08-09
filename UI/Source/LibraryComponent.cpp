/*
  ==============================================================================

    LibraryComponent.cpp
    Created: 5 Aug 2016 5:58:23pm
    Author:  crioto

  ==============================================================================
*/

#include "LibraryComponent.h"

LibraryComponent::LibraryComponent() : 
    _installButton("Install...")
{
    addAndMakeVisible(_installButton);
    _installButton.setBounds(600, 20, 150, 35);
    _installButton.addListener(this);

}

LibraryComponent::~LibraryComponent() {

}

void LibraryComponent::paint(Graphics& g) {
    g.fillAll (Colour (0xff222222));
    g.setFont (Font (16.0f));
    g.setColour (Colours::white);
    g.drawText ("Library", getLocalBounds(), Justification::centred, true);
}

void LibraryComponent::resized() {
}

void LibraryComponent::buttonClicked(Button* button) {
    if (button == &_installButton) {
        _installButton.setEnabled(false);
        auto dialog = new InstallationDialog("Subutai Installation", Colours::grey, DocumentWindow::allButtons);
        dialog->setBounds(0, 0, 800, 640);
        dialog->centreWithSize(dialog->getWidth(), dialog->getHeight());
        dialog->setTitleBarHeight(0);
        dialog->setVisible(true);
    }
}
