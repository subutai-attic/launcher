#include "WizardFinish.h"
#include "WizardWindow.h"

WizardFinish::WizardFinish() :
    _finish("Finish")
{
    _logger = &Poco::Logger::get("subutai");
    _logger->trace("Creating Wizard Finish UI Component");
    auto font = juce::Font(17);
    _thanks.setText("Thank you for using Subutai", juce::dontSendNotification);
    _thanks.setColour(juce::Label::textColourId, juce::Colours::white);
    _thanks.setBounds(15, 15, 150, 40);
    _thanks.setFont(font);
    _thanks.setJustificationType(juce::Justification::top);
    addAndMakeVisible(_thanks);

    _finish.setBounds(400, 560, 86, 25);
    _finish.setColour(juce::TextButton::buttonColourId, juce::Colour(7,141,208));
    _finish.addListener(this);
    addAndMakeVisible(_finish);
}

WizardFinish::~WizardFinish()
{
    _logger->trace("Destroying Wizard Finish UI Component");
}

void WizardFinish::paint(juce::Graphics& g)
{
    g.fillAll(Colour::greyLevel (0.2f));
}

void WizardFinish::resized()
{

}

void WizardFinish::buttonClicked(juce::Button* button)
{
    if (button == &_finish)
    {
        _logger->trace("Finish button pressed");
        // Wizard -> WizardWindow
        WizardWindow* window = (WizardWindow*)getParentComponent()->getParentComponent();
        window->closeButtonPressed();
    }
}
