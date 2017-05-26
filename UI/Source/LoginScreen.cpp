#include "LoginScreen.h"

LoginScreen::LoginScreen() : 
    _button("Login"), 
    _password("Password", (juce::juce_wchar) 0x2022) 
{

}

LoginScreen::~LoginScreen() 
{

}

void LoginScreen::paint(juce::Graphics& g) 
{
    g.fillAll(juce::Colour(0xff555555));

    addAndMakeVisible(_login);
    _login.setBounds(10, 25, 180, 24);
    _login.setText("Username");

    addAndMakeVisible(_password);
    _password.setBounds(10, 65, 180, 24);
    _password.setText("Password");

    addAndMakeVisible(_button);
    _button.addListener(this);
    _button.setBounds(10, 105, 180, 24);
}

void LoginScreen::buttonClicked(juce::Button* button) 
{
    if (button == &_button) {
        //_button.setVisible(false);
        removeFromDesktop();
    }
}
