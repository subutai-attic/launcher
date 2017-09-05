#ifndef __COMPONENT_CHOOSER_H__
#define __COMPONENT_CHOOSER_H__

#include "../JuceLibraryCode/JuceHeader.h"
#include "Poco/Logger.h"
#include "LauncherButton.h"

struct ChosenComponents
{
    bool ptp = false;
    bool tray = false;
    bool ete = false;
    bool peer = false;
    bool rh = false;
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
        void btnPtpYes_Clicked();
        void btnPtpNo_Clicked();
        void btnTrayYes_Clicked();
        void btnTrayNo_Clicked();
        void btnETEYes_Clicked();
        void btnETENo_Clicked();
        void btnPeerYes_Clicked();
        void btnPeerNo_Clicked();
        void btnRHYes_Clicked();
        void btnRHNo_Clicked();
        void btnCPUPlus_Clicked();
        void btnCPUMinus_Clicked();
        void btnMemPlus_Clicked();
        void btnMemMinus_Clicked();

        Poco::Logger *_logger;

        juce::Label _ptpLabel;
        juce::Label _ptpInfo;
        juce::Label _trayLabel;
        juce::Label _trayInfo;
        juce::Label _eteLabel;
        juce::Label _eteInfo;
        juce::Label _peerLabel;
        juce::Label _peerInfo;
        juce::Label _rhLabel;
        juce::Label _rhInfo;
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
        juce::TextButton* _rhNo;
        juce::TextButton* _rhYes;

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
        bool _rh;
        int _cpu;
        int _mem;

        bool _vtxStatus;
        int _maxCpu;
        int _maxMem;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ComponentChooser)
};

#endif
