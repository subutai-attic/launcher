/*
  ==============================================================================

    LoginScreen.h
    Created: 31 Jul 2016 6:41:07pm
    Author:  crioto

  ==============================================================================
*/

#ifndef LOGINSCREEN_H_INCLUDED
#define LOGINSCREEN_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

class LoginScreen : public Component, public ButtonListener {
    public:
        LoginScreen();
        ~LoginScreen();
        void paint(Graphics& g);
        void buttonClicked(Button* button);
    private:
        TextEditor _login;
        TextEditor _password;
        TextButton _button;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LoginScreen)
};



#endif  // LOGINSCREEN_H_INCLUDED
