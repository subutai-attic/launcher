#include "LibraryComponent.h"

LibraryComponent::LibraryComponent() : _installButton("Install")
{
    _logger = &Poco::Logger::get("subutai");
    _logger->debug("Starting Library UI Component");
    _componentTabs.push_back(new LibraryItemTab("Peers", 0, true));
    _componentTabs.push_back(new LibraryItemTab("P2P", 1, false));
    _componentTabs.push_back(new LibraryItemTab("Tray", 2, false));
    _componentTabs.push_back(new LibraryItemTab("E2E", 3, false));

    int pWidth = 20;

    for (auto it = _componentTabs.begin(); it != _componentTabs.end(); it++) 
    {
        (*it)->setBounds(pWidth, 0, (*it)->getWidth(), 30);
        addAndMakeVisible((*it));
        pWidth += (*it)->getWidth() + 20;
    }

    _installButton.addListener(this);
    _installButton.setColour(juce::TextButton::buttonColourId, juce::Colour(7,141,208));
    _installButton.toFront(true);
    _installButton.setBounds(getParentWidth() - 150 - 250, 10, 100, 35);
    addAndMakeVisible(_installButton);
    resized();
}

LibraryComponent::~LibraryComponent() 
{
    for (auto it = _componentTabs.begin(); it != _componentTabs.end(); it++)
    {
        delete (*it);
    }
    _logger->trace("Destroying Library UI Component");    
}

void LibraryComponent::paint(Graphics& g) {
    //g.fillAll (Colour (0xffffffff));
    g.setColour (Colours::white);
    g.fillRoundedRectangle(0, 0, getWidth(), getHeight(), 4.0);
    g.setFont (Font (16.0f));
    g.setColour (Colours::white);
}

void LibraryComponent::resized() {
    _installButton.setBounds(getParentWidth() - 110 - 250, 10, 100, 35);
}

void LibraryComponent::buttonClicked(Button* button) 
{
    if (button == &_installButton) {
        _logger->trace("Install button clicked");
        // Starting Wizard
        WizardWindow *wizard = new WizardWindow();
        wizard->setVisible(true);
    }
}

void LibraryComponent::tabClick(const std::string& tabTitle)
{
    for (auto it = _componentTabs.begin(); it != _componentTabs.end(); it++)
    {
        if ((*it)->title() == tabTitle) {
            (*it)->select();
            continue;
        }
        (*it)->deselect();
    }
}
