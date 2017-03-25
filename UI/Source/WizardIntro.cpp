#include "WizardIntro.h"

WizardIntro::WizardIntro()
{
    auto font = juce::Font(15);
    _text.setText("Welcommmmmeeee");
    _text.setBounds(10, 10, 490, 400);
    _text.setColour(Colours::white);
    _text.setFont(font, true);
    /*  
    _text.setBoundingBox(RelativeParallelogram( Point<float>(0, 0),
                Point<float>(100, 0),
                Point<float>(0, 0)));
                */
    //_text.setBoundingBox(getBounds().toFloat());
    addAndMakeVisible(_text);
}

WizardIntro::~WizardIntro()
{

}

void WizardIntro::paint(juce::Graphics& g)
{
    g.fillAll (Colour::greyLevel (0.2f));
}

void WizardIntro::resized()
{

}
