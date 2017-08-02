#ifndef LOGINSCREEN_H_INCLUDED
#define LOGINSCREEN_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

class LoginScreen : 
    public juce::Component, 
    public juce::ButtonListener,
	public juce::TextEditor::Listener
{
    public:
        LoginScreen();
        ~LoginScreen();
        void paint(juce::Graphics& g) override;
        void buttonClicked(juce::Button* button) override;
    private:
		juce::Label _emailText;
		juce::Label _passwordText;
        juce::TextButton _button;
        juce::TextEditor _login;
        juce::TextEditor _password;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LoginScreen)
};

#endif  // LOGINSCREEN_H_INCLUDED
