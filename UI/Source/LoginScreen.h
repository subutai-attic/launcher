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

class LoginScreen : public juce::Component, public juce::ButtonListener {
    public:
        LoginScreen();
        ~LoginScreen();
        void paint(juce::Graphics& g);
        void buttonClicked(juce::Button* button);
    private:
        juce::TextEditor _login;
        juce::TextEditor _password;
        juce::TextButton _button;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LoginScreen)
};



#endif  // LOGINSCREEN_H_INCLUDED
