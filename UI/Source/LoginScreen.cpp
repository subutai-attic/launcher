#include "LoginScreen.h"

LoginScreen::LoginScreen() : 
    _button("Login"), 
    _password("Password", (juce::juce_wchar) 0x2022) 
{
	addAndMakeVisible(_login);
	_login.setBounds(10, 50, 180, 24);
	_login.setColour(juce::TextEditor::outlineColourId, juce::Colours::grey);
	_login.setText("Username");

	addAndMakeVisible(_password);
	_password.setBounds(10, 85, 180, 24);
	_password.setColour(juce::TextEditor::outlineColourId, juce::Colours::grey);
	_password.setText("Password");

	addAndMakeVisible(_button);
	_button.addListener(this);
	_button.setBounds(10, 105, 180, 24);

	auto font = Font("Encode Sans", 17, 1);

	_emailText.setText("E-mail", juce::dontSendNotification);
	_emailText.setFont(font);
	_emailText.setBounds(10, 25, 180, 24);
	_emailText.setJustificationType(Justification::centred);
	_emailText.setColour(Label::textColourId, juce::Colours::grey);
	addAndMakeVisible(_emailText);
}

LoginScreen::~LoginScreen() 
{

}

void LoginScreen::paint(juce::Graphics& g) 
{
	g.fillAll(juce::Colour::fromRGB(247, 249, 252));
}

void LoginScreen::buttonClicked(juce::Button* button) 
{
    if (button == &_button) {
        //_button.setVisible(false);
        removeFromDesktop();
    }
}
