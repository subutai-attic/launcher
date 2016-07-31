/*
  ==============================================================================

    LoginScreen.cpp
    Created: 31 Jul 2016 6:41:07pm
    Author:  crioto

  ==============================================================================
*/

#include "LoginScreen.h"

LoginScreen::LoginScreen() : _button("Login"), _password("Password", (juce_wchar) 0x2022) {

}

LoginScreen::~LoginScreen() {

}

void LoginScreen::paint(Graphics& g) {
    g.fillAll (Colour (0xff555555));

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

void LoginScreen::buttonClicked(Button* button) {
    if (button == &_button) {
        _button.setVisible(false);
        //this->removeFromDesktop();
    }
}
