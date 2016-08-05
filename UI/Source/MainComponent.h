/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#ifndef MAINCOMPONENT_H_INCLUDED
#define MAINCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "LoginScreen.h"
#include "MainMenuModel.h"
#include "SubutaiLauncher.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainContentComponent   : public Component, public ButtonListener
{
public:
    //==============================================================================
    MainContentComponent();
    ~MainContentComponent();

    void paint (Graphics&) override;
    void resized() override;

    void buttonClicked(Button* button) override;

    Path getLogo();

private:
    void showLoginScreen();
    LoginScreen* _login;
    ListBox _mainMenu;
    MainMenuModel _mainMenuModel;
    Toolbar _header;
    ImageComponent _banner;
    ImageComponent _logo;

    ImageButton *_closeButton;

    // Launcher
    SubutaiLauncher::Core* _core;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};


#endif  // MAINCOMPONENT_H_INCLUDED
