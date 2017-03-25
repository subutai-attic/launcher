#include "WizardWindow.h"

WizardWindow::WizardWindow() : DocumentWindow(
        "Installation Wizard", 
        juce::Colours::lightgrey,
        juce::DocumentWindow::closeButton)
{
    setSize(640, 480);
    centreWithSize(getWidth(), getHeight());
    setDraggable(false);
    setResizable(false, false);
    setUsingNativeTitleBar(true);
    _wizard = new Wizard();
    setContentOwned(_wizard, true);
}

WizardWindow::~WizardWindow()
{

}

void WizardWindow::closeButtonPressed()
{
    delete this;
}

