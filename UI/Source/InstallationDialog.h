/*
  ==============================================================================

    InstallationDialog.h
    Created: 8 Aug 2016 1:21:29pm
    Author:  crioto

  ==============================================================================
*/

#ifndef INSTALLATIONDIALOG_H_INCLUDED
#define INSTALLATIONDIALOG_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

class InstallationDialog : public juce::DocumentWindow {
    public:
        InstallationDialog(const juce::String& name, juce::Colour backgroundColour, int buttonsNeeded);
        ~InstallationDialog();
        void closeButtonPressed();
    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (InstallationDialog)
};

#endif  // INSTALLATIONDIALOG_H_INCLUDED
