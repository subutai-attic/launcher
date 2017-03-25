#ifndef __LIBRARY_ITEM_H__
#define __LIBRARY_ITEM_H__

#include "../JuceLibraryCode/JuceHeader.h"

#include "LibraryActionThread.h"
#include "LibrarySystemCheck.h"

class LibraryItem : public juce::Component
{
    public:
        static const int WIDTH = 180;
        static const int HEIGHT = 200;
        LibraryItem(
                const std::string& title, 
                const std::string& desc, 
                const std::string& installScript = "", 
                const std::string& updateScript = "", 
                const std::string& removeScript = "");
        ~LibraryItem();
        void paint(juce::Graphics& g) override;
        void resized() override;
        void mouseUp(const juce::MouseEvent& e) override;
        void drawVersion();
        static std::string findVersion(std::string cname);

    private:
        std::string _title;
        std::string _desc;
        std::string _installScript;
        std::string _updateScript;
        std::string _removeScript;
        juce::Label _titleLabel;
        juce::Label _plusLabel;
        juce::Label _version;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LibraryItem)

};

#endif
