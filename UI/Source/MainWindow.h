/*
  ==============================================================================

    MainWindow.h
    Created: 18 Aug 2016 7:00:48pm
    Author:  crioto

  ==============================================================================
*/

#ifndef MAINWINDOW_H_INCLUDED
#define MAINWINDOW_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "MainComponent.h"

class MainWindow : public juce::DocumentWindow
{
    public:
        MainWindow (juce::String name)  : juce::DocumentWindow (name,
                juce::Colours::lightgrey,
                juce::DocumentWindow::allButtons)
    {
        setUsingNativeTitleBar (false);
        setTitleBarHeight(0);
        MainContentComponent* mainComponent = new MainContentComponent();
        setContentOwned (mainComponent, true);

        centreWithSize (getWidth(), getHeight());
        setVisible (true);
    }

        void closeButtonPressed() override
        {
            // This is called when the user tries to close this window. Here, we'll just
            // ask the app to quit when this happens, but you can change this to do
            // whatever you need.
            juce::JUCEApplication::getInstance()->systemRequestedQuit();
        }

        /* Note: Be careful if you override any DocumentWindow methods - the base
           class uses a lot of them, so by overriding you might break its functionality.
           It's best to do all your work in your content component instead, but if
           you really have to override any DocumentWindow methods, make sure your
           subclass also calls the superclass's method.
           */

    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainWindow)
};

#endif  // MAINWINDOW_H_INCLUDED
