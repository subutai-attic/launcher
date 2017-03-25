#include "LibrarySystemCheck.h"

LibrarySystemCheck::LibrarySystemCheck() : _numLines(1) 
{
}

LibrarySystemCheck::~LibrarySystemCheck() {

}

void LibrarySystemCheck::addLine(Label* field, Label* value, Label* hint, std::string text, std::string hintText, bool inst) {
    auto font = Font(18);
    auto fieldFont = Font(20);
    int offLeft = 350;

    field->setText(text, dontSendNotification);
    field->setColour(Label::textColourId, Colours::white);
    field->setBounds(0, _numLines * 50, 300, 50);
    field->setFont(fieldFont);
    field->setJustificationType(Justification::top);

    //value->setText("Checking...", dontSendNotification);
    if (inst) 
        value->setColour(Label::textColourId, Colours::white);
    else 
        value->setColour(Label::textColourId, Colours::red);
    //value->setBounds(320, _numLines * 50, 800, 50);
    value->setBounds(offLeft, _numLines * 50, 800, 50);
    value->setFont(font);
    value->setJustificationType(Justification::top);

    hint->setText(hintText, dontSendNotification);
    hint->setColour(Label::textColourId, Colours::grey);
    //hint->setBounds(320, _numLines * 50 + 20, 800, 50);
    hint->setBounds(offLeft, _numLines * 50 + 20, 800, 50);
    hint->setFont(font);
    hint->setJustificationType(Justification::top);

    _numLines++;
}

void LibrarySystemCheck::paint(Graphics& g) {
    g.fillAll (Colour (0xff222222));
    //g.setFont (Font (16.0f));
    g.setFont (Font (18.0f));
    //g.setColour (Colours::white);
    g.setColour(Colour(200,200,200));
}

void LibrarySystemCheck::resized() {
}
