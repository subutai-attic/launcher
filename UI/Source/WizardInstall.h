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
        std::thread run();
    private:
        bool _running;
        void runImpl();
        void addLine(const std::string& text, bool error = false);
        juce::ProgressBar* _pb;
        juce::Label* _title;
        double _progress;
        std::vector<juce::Label*> _lines; // Lines contains report strings
        std::string _script;
        std::string _name;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WizardInstall)
};

#endif
