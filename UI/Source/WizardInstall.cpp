#include "WizardInstall.h"

WizardInstall::WizardInstall()
{
    auto font = juce::Font(20);
    _title = new juce::Label();
    _title->setText("Initializing", dontSendNotification);
    _title->setColour(Label::textColourId, Colours::white);
    _title->setBounds(20, 20, 480, 40);
    _title->setFont(font);
    _title->setJustificationType(Justification::top);
    addAndMakeVisible(_title);
}

WizardInstall::~WizardInstall()
{
    delete _title;
    delete _pb;
}

void WizardInstall::paint(juce::Graphics& g)
{
    g.fillAll (Colour::greyLevel (0.2f));
}

void WizardInstall::resized()
{

}

void WizardInstall::start(const std::string& name)
{
    if (_pb) {
        delete _pb;
    }
    std::string nt("Installing ");
    nt.append(name);
    _title->setText(nt, dontSendNotification);
    _progress = 0.0;
    _pb = new ProgressBar(_progress);
    _pb->setBounds(20, 60, 460, 25);
    addAndMakeVisible(_pb);

    // Converting component name to a script

}
