#include "LauncherButton.h"

LauncherButton::LauncherButton(const juce::String& buttonName) : _name(buttonName)
{
    _logger = &Poco::Logger::get("subutai");
    _logger->trace("Creating button: %s", buttonName.toStdString());
    setColour(juce::TextButton::buttonColourId, juce::Colour(89, 183, 255));
    setColour(juce::TextButton::buttonOnColourId, juce::Colour(74, 148, 225));
    setColour(juce::TextButton::textColourOffId, juce::Colour(255, 255, 255));
    setColour(juce::TextButton::textColourOnId, juce::Colour(255, 255, 255));
    setButtonText(buttonName);
    resized();
}

/*
void LauncherButton::paint(Graphics& g)
{

}
*/

void LauncherButton::resized()
{
    _logger->trace("LauncherButton::resized() ~ %s", _name.toStdString());
    changeWidthToFitText(35);
}

