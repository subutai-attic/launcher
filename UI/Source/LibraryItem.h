#ifndef __LIBRARY_ITEM_H__
#define __LIBRARY_ITEM_H__


#include "../JuceLibraryCode/JuceHeader.h"

class LibraryItem : public juce::Component
{
    public:
        LibraryItem(const std::string& title);
        ~LibraryItem();
        void paint(juce::Graphics& g) override;
        void resized() override;
    private:
        std::string _title;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LibraryItem)

};

#endif
