#ifndef __WIZARD_INSTALL_H__
#define __WIZARD_INSTALL_H__

#include "../JuceLibraryCode/JuceHeader.h"
#include "../JuceLibraryCode/modules/juce_core/threads/juce_Thread.h"
#include "Poco/Logger.h"
#include "SubutaiLauncher.h"
#include "ThreadWrapper.h"

class WizardInstallThreadWorker;


class WizardInstall : public juce::Component 
{
  friend class WizardInstallThreadWorker;
public:
  static const std::string P2P_INSTALL;
  static const std::string TRAY_INSTALL;
  static const std::string E2E_INSTALL;
  static const std::string PEER_INSTALL;
  WizardInstall();
  ~WizardInstall();
  void paint (juce::Graphics&) override;
  void resized() override;
  void start(const std::string& name);
  int wait();
  void run();
  void abort();
  bool isRunning();
  bool isActive();
  void activate();
  void deactivate();
  bool succeed();
private:
//  std::thread runThread();
  CThreadWrapper<WizardInstallThreadWorker> *runThread();
  void runImpl();
  void addLine(const std::string& text, bool error = false);
private:
  bool _succeed;
  Poco::Logger* _logger;
  bool _running;
  bool _active;
  juce::ProgressBar* _pb;
  juce::Label* _title;
  double _progress;
  std::vector<juce::Label*> _lines; // Lines contains report strings
  std::string _script;
  std::string _name;
//  std::thread _installThread;
  CThreadWrapper<WizardInstallThreadWorker>* _installThread;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WizardInstall)
};

class WizardInstallThreadWorker {
private:
  WizardInstall* _wi_instance;

public:
  WizardInstallThreadWorker(WizardInstall* instance) : _wi_instance(instance)  {
  }
  ~WizardInstallThreadWorker() {/*do nothing*/}

  void Run() {
    _wi_instance->runImpl();
  }
};

#endif
