#ifndef __LIBRARY_ITEM_TAB_H__
#define __LIBRARY_ITEM_TAB_H__

#include "../JuceLibraryCode/JuceHeader.h"

class TabListener
{
    public:
        TabListener() 
        {

        }

        ~TabListener()
        {

        }

        void click()
        {

        }
};

class LibraryComponent;

class LibraryItemTab : public juce::Component
{
    public:
        LibraryItemTab(const std::string& title, int order, bool highlight);
        ~LibraryItemTab();
        void paint(juce::Graphics&) override;
        void resized() override;
        void mouseEnter(const juce::MouseEvent& e) override;
        void mouseExit(const juce::MouseEvent& e) override;
        void mouseUp(const juce::MouseEvent& e) override;
        void select();
        void deselect();
        std::string title();
    private: 
        bool _selected;
        bool _hover;
        std::string _title;
        int _order;

        juce::Label _titleLabel;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LibraryItemTab)

};

#endif
