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
        (*it)->setBounds(pWidth, 0, (*it)->getWidth(), 60);
        addAndMakeVisible((*it));
        pWidth += (*it)->getWidth() + 20;
    }

    addChildComponent(_installButton);
    _installButton.addListener(this);
    //_installButton.toFront(true);
    _installButton.setBounds(0, 0, 1, 1);
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

void LibraryComponent::paint(Graphics& g) 
{
    g.setColour (Colours::white);
    g.fillRoundedRectangle(0, 0, getWidth(), getHeight(), 4.0);
}

void LibraryComponent::resized() 
{
    _logger->trace("LibraryComponent::resized()");
    _logger->trace("Width: %d, Install Button width: %d height: %d", getWidth(), _installButton.getWidth(), _installButton.getHeight());
    _installButton.resized();
    _installButton.setBounds(getWidth() - _installButton.getWidth() - 30, 60 / 2 - _installButton.getHeight() / 2, _installButton.getWidth(), _installButton.getHeight());
    _installButton.setVisible(true);
}

#define JUCE_MODAL_LOOPS_PERMITTED
void LibraryComponent::buttonClicked(Button* button) 
{
    if (button == &_installButton) {
        _logger->trace("Install button clicked");
        // Starting Wizard
        DialogWindow::LaunchOptions options;
        options.dialogTitle = "Installation Wizard";
        options.dialogBackgroundColour = juce::Colours::lightgrey;
        options.escapeKeyTriggersCloseButton = true;
        options.useNativeTitleBar = true;
        options.useBottomRightCornerResizer = true;
        juce::OptionalScopedPointer<Component> content(new Wizard, true);
        options.content = content;
        options.runModal();
        SubutaiLauncher::Session::instance()->terminate();
//        WizardWindow *wizard = new WizardWindow();
//        wizard->setVisible(true);
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
