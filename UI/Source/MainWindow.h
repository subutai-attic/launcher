#ifndef MAINWINDOW_H_INCLUDED
#define MAINWINDOW_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "MainComponent.h"

class MainWindow : public juce::DocumentWindow
{
    public:
        MainWindow(juce::String name);
        void closeButtonPressed() override;

    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainWindow)
};

#endif  // MAINWINDOW_H_INCLUDED
