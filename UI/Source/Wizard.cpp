#include "Wizard.h"

Wizard::Wizard() :
    _next("Next"),
    _back("Back"),
    _cancel("Cancel"),
    _step(1)
{
    setSize(800, 600);
    auto font = juce::Font(15);

    _stepIntro.setText("Welcome", dontSendNotification);
    _stepIntro.setColour(Label::textColourId, juce::Colour(7, 141, 208));
    _stepIntro.setBounds(15, 50, 300, 40);
    _stepIntro.setFont(font);
    _stepIntro.setJustificationType(Justification::top);

    _stepSystemCheck.setText("System Check", dontSendNotification);
    _stepSystemCheck.setColour(Label::textColourId, Colours::grey);
    _stepSystemCheck.setBounds(15, 80, 300, 40);
    _stepSystemCheck.setFont(font);
    _stepSystemCheck.setJustificationType(Justification::top);

    _stepComponentChooser.setText("Choose Components", dontSendNotification);
    _stepComponentChooser.setColour(Label::textColourId, Colours::grey);
    _stepComponentChooser.setBounds(15, 110, 300, 40);
    _stepComponentChooser.setFont(font);
    _stepComponentChooser.setJustificationType(Justification::top);

    _stepInstall.setText("Install", dontSendNotification);
    _stepInstall.setColour(Label::textColourId, Colours::grey);
    _stepInstall.setBounds(15, 140, 300, 40);
    _stepInstall.setFont(font);
    _stepInstall.setJustificationType(Justification::top);

    _stepFinal.setText("Finish", dontSendNotification);
    _stepFinal.setColour(Label::textColourId, Colours::grey);
    _stepFinal.setBounds(15, 170, 300, 40);
    _stepFinal.setFont(font);
    _stepFinal.setJustificationType(Justification::top);

    addAndMakeVisible(_stepIntro);
    addAndMakeVisible(_stepSystemCheck);
    addAndMakeVisible(_stepComponentChooser);
    addAndMakeVisible(_stepInstall);
    addAndMakeVisible(_stepFinal);

    // Installation step pages

    _introPage = new WizardIntro();
    _introPage->setBounds(300, 0, 500, 600);
    addAndMakeVisible(_introPage);

    _systemCheckPage = new SystemCheck();
    _systemCheckPage->setBounds(300, 0, 500, 600);
    addChildComponent(_systemCheckPage);

    _componentChooserPage = new ComponentChooser();
    _componentChooserPage->setBounds(300, 0, 500, 600);
    addChildComponent(_componentChooserPage);

    _ptpInstall = new WizardInstall();
    _ptpInstall->setBounds(300, 0, 500, 600);
    addChildComponent(_ptpInstall);

    _trayInstall = new WizardInstall();
    _trayInstall->setBounds(300, 0, 500, 600);
    addChildComponent(_trayInstall);

    _eteInstall = new WizardInstall();
    _eteInstall->setBounds(300, 0, 500, 600);
    addChildComponent(_eteInstall);

    _peerInstall = new WizardInstall();
    _peerInstall = new WizardInstall();
    addChildComponent(_peerInstall);

    // Progression buttons

    _next.setBounds(202, 560, 86, 25);
    _next.addListener(this);
    _next.setColour(TextButton::buttonColourId, Colour(7,141,208));
    addAndMakeVisible(_next);

    _back.setBounds(106, 560, 86, 25);
    _back.addListener(this);
    _back.setEnabled(false);
    addAndMakeVisible(_back);

    _cancel.setBounds(10, 560, 86, 25);
    _cancel.addListener(this);
    addAndMakeVisible(_cancel);
}

Wizard::~Wizard()
{
    delete _introPage;
    delete _systemCheckPage;
    delete _componentChooserPage;
    delete _ptpInstall;
}

void Wizard::paint(juce::Graphics& g)
{
    g.fillAll (Colour (0xff222222));
    //g.fillAll (Colour::greyLevel (0.2f));
}

void Wizard::resized()
{

}

void Wizard::buttonClicked(juce::Button* button)
{
    if (button == &_next) {
        switch (_step) {
            case 1:
                _introPage->setVisible(false);
                _systemCheckPage->setVisible(true);
                _stepIntro.setColour(Label::textColourId, Colours::white);
                _stepSystemCheck.setColour(Label::textColourId, juce::Colour(7, 141, 208));
                _back.setEnabled(true);
                _step = 2;
                break;
            case 2:
                _systemCheckPage->setVisible(false);
                _componentChooserPage->setVisible(true);
                _stepSystemCheck.setColour(Label::textColourId, Colours::white);
                _stepComponentChooser.setColour(Label::textColourId, juce::Colour(7, 141, 208));
                _step = 3;
                break;
            case 3:
                _componentChooserPage->setVisible(false);
                _ptpInstall->setVisible(true);
                _stepComponentChooser.setColour(Label::textColourId, Colours::white);
                _stepInstall.setColour(Label::textColourId, juce::Colour(7, 141, 208));
                _step = 4;
                _back.setEnabled(false);
                runInstall();
                break;
            default:
                _introPage->setVisible(true);
                _back.setEnabled(false);
                _step = 1;
                _stepIntro.setColour(Label::textColourId, juce::Colour(7, 141, 208));
                break;
        }
    } else if (button == &_back) {
        switch (_step) {
            case 1:
                break;
            case 2:
                _back.setEnabled(false);
                _systemCheckPage->setVisible(false);
                _introPage->setVisible(true);
                _stepIntro.setColour(Label::textColourId, juce::Colour(7, 141, 208));
                _stepSystemCheck.setColour(Label::textColourId, Colours::grey);
                _step = 1;
                break;
            case 3:
                _componentChooserPage->setVisible(false);
                _systemCheckPage->setVisible(true);
                _stepSystemCheck.setColour(Label::textColourId, juce::Colour(7, 141, 208));
                _stepComponentChooser.setColour(Label::textColourId, Colours::grey);
                _step = 2;
                break;
            default:
                break;
        }
    } else if (button == &_cancel) {

    }
}

void Wizard::runInstall()
{
    auto c = _componentChooserPage->getComponents();
    if (c.ptp) {
        _ptpInstall->start();
    }
}
