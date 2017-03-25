#ifndef __SYSTEM_CHECK_H__
#define __SYSTEM_CHECK_H__

#include "../JuceLibraryCode/JuceHeader.h"

class SystemCheck : public juce::Component 
{
    public:
        SystemCheck();
        ~SystemCheck();
        void paint (juce::Graphics&) override;
        void resized() override;
    private:
        juce::Label _osLabel;
        juce::Label _osValue;
        juce::Label _osInfo;
        juce::Label _archLabel;
        juce::Label _archValue;
        juce::Label _archInfo;
        juce::Label _cpuLabel;
        juce::Label _cpuValue;
        juce::Label _cpuInfo;
        juce::Label _memLabel;
        juce::Label _memValue;
        juce::Label _memInfo;
        juce::Label _vtxLabel;
        juce::Label _vtxValue;
        juce::Label _vtxInfo;
        juce::Label _vbLabel;
        juce::Label _vbValue;
        juce::Label _vbInfo;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SystemCheck)
};

#endif
