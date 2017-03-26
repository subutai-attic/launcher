#ifndef __WIZARD_INSTALL_H__
#define __WIZARD_INSTALL_H__

#include "../JuceLibraryCode/JuceHeader.h"

class WizardInstall : public juce::Component 
{
    public:
        WizardInstall();
        ~WizardInstall();
        void paint (juce::Graphics&) override;
        void resized() override;
        void start(const std::string& name);
        void wait();
    private:
        void addLine(const std::string& text);
        juce::ProgressBar* _pb;
        juce::Label* _title;
        double _progress;
        std::vector<juce::Label> _lines; // Lines contains report strings

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WizardInstall)
};

#endif
