#ifndef __WIZARD_FINISH_H__
#define __WIZARD_FINISH_H__

#include "../JuceLibraryCode/JuceHeader.h"
#include "../JuceLibraryCode/modules/juce_gui_basics/juce_gui_basics.h"
#include "Poco/Logger.h"

class WizardFinish : 
    public juce::Component,
    public juce::ButtonListener
{
    public:
        WizardFinish();
        ~WizardFinish();
        void paint (juce::Graphics&) override;
        void resized() override;
        void buttonClicked(juce::Button* button) override;
		void addPTPResult(bool installed, bool succeed);
		void addTrayResult(bool installed, bool succeed);
		void addETEResult(bool installed, bool succeed);
		void addPeerResult(bool installed, bool succeed);
    private:
        Poco::Logger* _logger;
        juce::Label _thanks;
		
		juce::Label _ptp;
		juce::Label _tray;
		juce::Label _ete;
		juce::Label _peer;

		juce::Label _ptpResult;
		juce::Label _trayResult;
		juce::Label _eteResult;
		juce::Label _peerResult;
        juce::TextButton _finish;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WizardFinish)
};

#endif
