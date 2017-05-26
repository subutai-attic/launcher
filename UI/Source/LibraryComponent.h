#ifndef LIBRARYCOMPONENT_H_INCLUDED
#define LIBRARYCOMPONENT_H_INCLUDED

#include "Vars.h"

#include "../JuceLibraryCode/JuceHeader.h"

#include "Poco/Logger.h"
#include "Session.h"
#include "LibraryItem.h"
#include "LibraryItemTab.h"
#include "LibrarySystemCheck.h"
#include "WizardWindow.h"

class LibraryComponent : 
    public juce::Component, 
    public juce::ComponentListener,
    public juce::ButtonListener, 
    public juce::TextEditorListener 
{
    public:
        LibraryComponent();
        ~LibraryComponent();
        void paint (juce::Graphics&) override;
        void resized() override;
        void buttonClicked(juce::Button* button) override;
        void tabClick(const std::string& tabTitle);
    private:
        Poco::Logger* _logger;
        juce::Label _componentsSectionLabel;
        juce::Label _peersSectionLabel;
        juce::TextButton _installButton;
        std::vector<LibraryItem*> _components;
        std::vector<LibraryItemTab*> _componentTabs;
        std::vector<LibraryItem*> _peers;
        LibraryItem* _tb;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LibraryComponent)
};

#endif  // LIBRARYCOMPONENT_H_INCLUDED
