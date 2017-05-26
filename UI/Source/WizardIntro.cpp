#include "WizardIntro.h"

WizardIntro::WizardIntro()
{
    _logger = &Poco::Logger::get("subutai");
    _logger->trace("Creating Wizard Intro UI Component");
    auto font = juce::Font(17);
    _text.setText("Welcome to Subutai Installation Wizard!\n\nThis application will help you to setup Subutai Peer and all necessary components.\n\n");
    _text.setJustification(juce::Justification::topLeft);
    _text.setBounds(10, 10, getParentWidth(), getParentHeight());
    _text.setColour(Colours::white);
    _text.setFont(font, true);
    _text.setBoundingBox(RelativeParallelogram( Point<float>(20, 20),
                Point<float>(400, 20),
                Point<float>(20, 400)));
    //_text.setBoundingBox(getBounds().toFloat());
    addAndMakeVisible(_text);
}

WizardIntro::~WizardIntro()
{
    _logger->trace("Destroying Wizard Intro UI Component");
}

void WizardIntro::paint(juce::Graphics& g)
{
    g.fillAll (Colour::greyLevel (0.2f));
}

void WizardIntro::resized()
{

}
