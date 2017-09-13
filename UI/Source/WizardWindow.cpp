#include "WizardWindow.h"

#if LIGHT_MODE
WizardWindow::WizardWindow(juce::String name) : juce::DocumentWindow(
        name,
        juce::Colours::lightgrey,
        juce::DocumentWindow::allButtons
        )
#else
WizardWindow::WizardWindow(juce::String name) : juce::DialogWindow(
        "Installation Wizard", 
        juce::Colours::lightgrey,
        false, true)
#endif
{
    _logger = &Poco::Logger::get("subutai");
    _logger->trace("Creating Installation Wizard Window");
    setSize(800, 600);
    centreWithSize(getWidth(), getHeight());
    setDraggable(false);
    setResizable(false, false);
    setUsingNativeTitleBar(true);
    _wizard = new Wizard();
    setContentOwned(_wizard, true);

#if LIGHT_MODE
    centreWithSize(getWidth(), getHeight());
#endif
    setVisible(true);
    toFront(true);
    setSize(800, 600);
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
#if LIGHT_MODE
    juce::JUCEApplication::getInstance()->systemRequestedQuit();
#else
    delete this;
#endif
}

bool WizardWindow::escapeKeyPressed()
{
	return false;
}

void WizardWindow::crash()
{
    Crash* crash = new Crash();
    setContentOwned(crash, true);
}
