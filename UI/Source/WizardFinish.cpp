#include "WizardFinish.h"

WizardFinish::WizardFinish()
{
    _logger = &Poco::Logger::get("subutai");
    _logger->trace("Creating Wizard Finish UI Component");
    auto font = juce::Font(18);
    _thanks.setText("Thank you for using Subutai", dontSendNotification);
    _thanks.setColour(Label::textColourId, Colours::white);
    _thanks.setBounds(15, 15, 150, 40);
    _thanks.setFont(font);
    _thanks.setJustificationType(Justification::top);
    addAndMakeVisible(_thanks);
}

WizardFinish::~WizardFinish()
{
    _logger->trace("Destroying Wizard Finish UI Component");
}

void WizardFinish::paint(juce::Graphics& g)
{
    g.fillAll (Colour::greyLevel (0.2f));
}

void WizardFinish::resized()
{

}
