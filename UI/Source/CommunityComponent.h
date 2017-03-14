/*
   ==============================================================================

   CommunityComponent.h
Created: 8 Aug 2016 2:15:00pm
Author:  crioto

==============================================================================
*/

#ifndef COMMUNITYCOMPONENT_H_INCLUDED
#define COMMUNITYCOMPONENT_H_INCLUDED

#include <vector>

#include "../JuceLibraryCode/JuceHeader.h"

class FrontBanner : public juce::Component 
{
    public:
        FrontBanner() {

        }

        ~FrontBanner()
        {

        }
        
        void Create(juce::Image img, std::string text) {
            _image.setImage(img);
            auto mainFont = new juce::Font(30.0);
            _label.setText(text, juce::dontSendNotification);
            _label.setColour(juce::Label::backgroundColourId, juce::Colour(0xaa222222));
            _label.setColour(juce::Label::textColourId, juce::Colours::white);
            _label.setJustificationType(juce::Justification::centred);
            _label.setFont(*mainFont);
            _label.setBounds(0, img.getHeight() - 150, img.getWidth(), 150);
        }

        void paint (juce::Graphics&) override
        {

        }

        void resized() override
        {

        }

        juce::Label _label;
        juce::ImageComponent _image;
    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FrontBanner)
};

class CommunityComponent : public juce::Component 
{
    public:
        CommunityComponent();
        ~CommunityComponent();
        void paint (juce::Graphics&) override;
        void resized() override;
    private:
        std::vector<FrontBanner*> _imgs;
        juce::ImageComponent _mainBanner;
        juce::Label _mainLabel;
        juce::ImageComponent _banner1;
        juce::ImageComponent _banner2;
        juce::ImageComponent _banner3;
        juce::Label _label1;
        juce::Label _label2;
        juce::Label _label3;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CommunityComponent)
};

#endif  // COMMUNITYCOMPONENT_H_INCLUDED
