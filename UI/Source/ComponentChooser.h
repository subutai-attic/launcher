#ifndef __COMPONENT_CHOOSER_H__
#define __COMPONENT_CHOOSER_H__

#include "../JuceLibraryCode/JuceHeader.h"
#include "Poco/Logger.h"

struct ChosenComponents
{
    bool ptp = false;
    bool tray = false;
    bool ete = false;
    bool peer = false;
    int cpu = 2;
    int mem = 2;
};

class ComponentChooser : public juce::Component,
    public juce::ButtonListener
{
    public:
        ComponentChooser();
        ~ComponentChooser();
        void paint (juce::Graphics&) override;
        void resized() override;
        void buttonClicked(juce::Button* button) override;
        ChosenComponents getComponents();
    private:
        Poco::Logger *_logger;

        juce::Label _ptpLabel;
        juce::Label _ptpInfo;
        juce::Label _trayLabel;
        juce::Label _trayInfo;
        juce::Label _eteLabel;
        juce::Label _eteInfo;
        juce::Label _peerLabel;
        juce::Label _peerInfo;
        juce::Label _cpuLabel;
        juce::Label _cpuInfo;
        juce::Label _memLabel;
        juce::Label _memInfo;

        juce::TextButton* _ptpNo;
        juce::TextButton* _ptpYes;
        juce::TextButton* _trayNo;
        juce::TextButton* _trayYes;
        juce::TextButton* _eteNo;
        juce::TextButton* _eteYes;
        juce::TextButton* _peerNo;
        juce::TextButton* _peerYes;

        juce::TextButton* _cpuNum;
        juce::TextButton* _cpuPlus;
        juce::TextButton* _cpuMinus;

        juce::TextButton* _memPlus;
        juce::TextButton* _memSize;
        juce::TextButton* _memMinus;

        bool _ptp;
        bool _tray;
        bool _ete;
        bool _peer;
        int _cpu;
        int _mem;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ComponentChooser)
};

#endif
