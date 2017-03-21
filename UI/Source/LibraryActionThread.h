#ifndef __LIBRARY_ACTION_THREAD_H__
#define __LIBRARY_ACTION_THREAD_H__

#include "../JuceLibraryCode/JuceHeader.h"

class LibraryActionThread  : public juce::ThreadWithProgressWindow
{
    private:
        std::string _process;
        std::string _component;
        std::string _title;
        bool _running;
        bool _error;
    public:
        LibraryActionThread(const std::string& process, const std::string& component, std::string& title);
	unsigned process_no();
        bool isRunning();
        void run() override;
        void threadComplete (bool userPressedCancel) override;
};

#endif
