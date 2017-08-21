#ifndef LOGINSCREEN_H_INCLUDED
#define LOGINSCREEN_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "LauncherButton.h"
#include "Poco/Logger.h"

class LoginScreen : 
    public juce::Component, 
    public juce::ButtonListener,
	public juce::TextEditor::Listener
{
    public:
        static const int INPUT_FIELD_WIDTH = 200;
        LoginScreen();
        ~LoginScreen();
        void paint(juce::Graphics& g) override;
        void resized() override;
        void buttonClicked(juce::Button* button) override;
    private:
		juce::Label _passwordText;
        juce::Label _title;
        juce::Label _version;
        juce::Label _error;
        LauncherButton _button;
        LauncherButton _skipButton;
        juce::TextEditor _login;
        juce::TextEditor _password;
		juce::ImageComponent _logo;
		juce::ImageComponent _userIcon;
		juce::ImageComponent _passwordIcon;
        Poco::Logger* _logger;

        juce::HyperlinkButton _resetLink;
        juce::HyperlinkButton _registerLink;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LoginScreen)
};

#endif  // LOGINSCREEN_H_INCLUDED
