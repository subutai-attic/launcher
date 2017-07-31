#ifndef SIDEBAR_H_INCLUDED
#define SIDEBAR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

#include "SocialMediaIcons.h"
#include "Poco/Logger.h"

class SidebarComponent : public juce::Component {
    public:
        SidebarComponent();
        ~SidebarComponent();
        void paint (juce::Graphics&) override;
        void resized() override;
        void activateNews();
        void activateLibrary();
        void activateTemplates();
        void activateCommunity();
    private:
        Poco::Logger *_logger;

        juce::Label _news;
        juce::Label _library;
        juce::Label _templates;
        juce::Label _community;

        juce::ImageComponent _newsActive;
        juce::ImageComponent _newsInactive;
        juce::ImageComponent _libraryActive;
        juce::ImageComponent _libraryInactive;
        juce::ImageComponent _templatesActive;
        juce::ImageComponent _templatesInactive;
        juce::ImageComponent _communityActive;
        juce::ImageComponent _communityInactive;
        SocialMediaIcons* _social;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SidebarComponent)
};



#endif  // SIDEBAR_H_INCLUDED
