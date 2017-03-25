#ifndef __LIBRARY_SYSTEM_CHECK_H__
#define __LIBRARY_SYSTEM_CHECK_H__

#include "../JuceLibraryCode/JuceHeader.h"

typedef enum {
    RH_ONLY,
    RH_CLIENT,
    MH_FULL,
    CLIENT_ONLY,
} InstallConfig;

typedef enum {
    PROD,
    STAGE,
    DEV,
} InstallVersion;


class LibrarySystemCheck : public juce::Component, public juce::ComponentListener {
    public:
        LibrarySystemCheck();
        ~LibrarySystemCheck();
        void paint(juce::Graphics& g) override;
        void resized();
        void addLine(juce::Label* field, juce::Label* value, juce::Label* hint, std::string text,
                std::string hintText, bool inst);
    private:

        juce::Label _osField;
        juce::Label _osValue;
        juce::Label _osHint;

        juce::Label _if64Field;
        juce::Label _if64Value;
        juce::Label _if64Hint;

        juce::Label _numCpuField;
        juce::Label _numCpuValue;
        juce::Label _numCpuHint;

        juce::Label _maxMemoryField;
        juce::Label _maxMemoryValue;
        juce::Label _maxMemoryHint;

        juce::Label _vboxField;
        juce::Label _vboxValue;
        juce::Label _vboxHint;

        juce::Label _vtxField;
        juce::Label _vtxValue;
        juce::Label _vtxHint;

        juce::Label _sshField;
        juce::Label _sshValue;
        juce::Label _sshHint;

        juce::Label _info;

        int _numLines;

        //TextButton _nextButton;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LibrarySystemCheck)
};

#endif
