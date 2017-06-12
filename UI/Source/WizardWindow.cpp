#include "WizardWindow.h"

WizardWindow::WizardWindow() : juce::DialogWindow(
        "Installation Wizard", 
        juce::Colours::lightgrey,
        false, true)
{
    _logger = &Poco::Logger::get("subutai");
    _logger->trace("Creating Installation Wizard Window");
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
    if (_wizard != nullptr) delete _wizard;
    _logger->trace("Destroying Installation Wizard Window");
}

void WizardWindow::closeButtonPressed()
{
    _logger->trace("WizardWindow: Close Button Pressed");
	SubutaiLauncher::Session::instance()->terminate();
    delete this;
}

bool WizardWindow::escapeKeyPressed()
{
	return true;
}
