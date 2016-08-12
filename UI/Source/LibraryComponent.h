/*
   ==============================================================================

   LibraryComponent.h
Created: 5 Aug 2016 5:58:23pm
Author:  crioto

==============================================================================
*/

#ifndef LIBRARYCOMPONENT_H_INCLUDED
#define LIBRARYCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

#include "InstallationDialog.h"

class LibraryComponent : public Component, public ButtonListener {
    public:
        LibraryComponent();
        ~LibraryComponent();
        void paint (Graphics&) override;
        void resized() override;
        void buttonClicked(Button* button);
        void step1();
    private:
        TextButton _installButton;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LibraryComponent)
};

#endif  // LIBRARYCOMPONENT_H_INCLUDED
