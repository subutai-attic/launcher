#include "WizardInstall.h"
#include "Wizard.h"

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
////////////////////////////////////////////////////////////////////////////

class DownloadThreadWorker {
private:
  SubutaiLauncher::Downloader* _downloader;
public:
  DownloadThreadWorker(SubutaiLauncher::Downloader* downloader) : _downloader(downloader) {
  }
  ~DownloadThreadWorker() {/*do nothing*/}

  void Run() {
    _downloader->downloadImpl();
  }
};
////////////////////////////////////////////////////////////////////////////

class ScriptThreadWorker {
private:
  SubutaiLauncher::SL* _sl;
public:
  ScriptThreadWorker(SubutaiLauncher::SL* sl) : _sl(sl){}
  ~ScriptThreadWorker() {/*do nothing*/}
  void Run() {
    _sl->execute();
  }
};
////////////////////////////////////////////////////////////////////////////

#if LAUNCHER_LINUX
const std::string WizardInstall::P2P_INSTALL = "launcher-p2p-install-linux";
const std::string WizardInstall::TRAY_INSTALL = "launcher-tray-install-linux";
const std::string WizardInstall::E2E_INSTALL = "launcher-chrome-e2e-install-linux";
const std::string WizardInstall::PEER_INSTALL = "launcher-peer-install-linux";
#elif LAUNCHER_WINDOWS
const std::string WizardInstall::P2P_INSTALL = "launcher-p2p-install-windows";
const std::string WizardInstall::TRAY_INSTALL = "launcher-tray-install-windows";
const std::string WizardInstall::E2E_INSTALL = "launcher-chrome-e2e-install-windows";
const std::string WizardInstall::PEER_INSTALL = "launcher-peer-install-windows";
#else
const std::string WizardInstall::P2P_INSTALL = "launcher-p2p-install-darwin";
const std::string WizardInstall::TRAY_INSTALL = "launcher-tray-install-darwin";
const std::string WizardInstall::E2E_INSTALL = "launcher-chrome-e2e-install-darwin";
const std::string WizardInstall::PEER_INSTALL = "launcher-peer-install-darwin";
#endif

WizardInstall::WizardInstall() : 
    _succeed(false),
    _running(false),
    _active(false),
    _installThread(nullptr),
    _downloadThread(nullptr),
    _scriptThread(nullptr)
{
    _pb = nullptr;
    _logger = &Poco::Logger::get("subutai");
    _logger->trace("Creating Wizard Install UI Component");
    auto font = juce::Font("Encode Sans", 20, 1);
    _title = new juce::Label();
    _title->setText("Initializing", juce::dontSendNotification);
    _title->setColour(juce::Label::textColourId, juce::Colour(105, 116, 144));
    _title->setBounds(20, 20, 480, 40);
    _title->setFont(font);
    _title->setJustificationType(juce::Justification::top);
    addAndMakeVisible(_title);
}

WizardInstall::~WizardInstall()
{
    _logger->trace("Waiting for installation thread to complete");
//    int rc = wait();
//    if (rc == 0)
//    {
//        _logger->trace("Thread finished");
//    }
//    else
//    {
//        _logger->trace("Thread was not running");
//    }
    _logger->trace("Destroying Wizard Install UI Component");
    for (auto it = _lines.begin(); it != _lines.end(); it++)
    {
        if ((*it) != nullptr) delete (*it);
    }
    _lines.clear();
    if (_title != nullptr) delete _title;
    if (_pb != nullptr) delete _pb;
    _logger->trace("Wizard Install UI Component has been destroyed");
    if (_installThread) delete _installThread;
}

void WizardInstall::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(255, 255, 255));
}

void WizardInstall::resized()
{

}

void WizardInstall::start(const std::string& name)
{
    _logger->information("Starting %s installation", name);
    _name = name;
    if (_pb != nullptr) 
    {
        _logger->trace("Destroying progress bar");
        delete _pb;
        _logger->trace("Progress bar deleted");
    }
    _logger->trace("Recreating progress bar");
    std::string nt("Installing ");
    nt.append(name);
    _title->setText(nt, juce::dontSendNotification);
    _progress = 0.0;
    _pb = new juce::ProgressBar(_progress);
    _pb->setBounds(20, 60, 460, 25);
    addAndMakeVisible(_pb);

    // Converting component name to a script
    if (name == "P2P") _script = P2P_INSTALL;
    else if (name == "Tray") _script = TRAY_INSTALL;
    else if (name == "Browser Plugin") _script = E2E_INSTALL;
    else if (name == "Peer") _script = PEER_INSTALL;

    _logger->debug("Installation initializator has been finished");
}

void WizardInstall::run()
{
  _logger->debug("********WizardInstall::run()0");
  _installThread = runThread();
  _running = true;
  _logger->debug("********WizardInstall::run()1");
  _installThread->Start();
  _logger->debug("********WizardInstall::run()2");
}

void WizardInstall::abort()
{
  _logger->debug("********WizardInstall::abort()1");
  _running = false;
  _logger->debug("********WizardInstall::abort()2");
  _installThread->Terminate(0);
  _scriptThread->Terminate(0);
  _downloadThread->Terminate(0);
  _logger->debug("********WizardInstall::abort()3");
}

CThreadWrapper<WizardInstallThreadWorker>* WizardInstall::runThread()
{
  return new CThreadWrapper<WizardInstallThreadWorker>(new WizardInstallThreadWorker(this), true);
}

void WizardInstall::runImpl() 
{
    _logger->information("%s installation started", _name);
    // Download installation script
    auto downloader = SubutaiLauncher::Session::instance()->getDownloader();
    auto script = _script;

    script.append(".py");
    downloader->reset();
    downloader->setFilename(script);
    if (!downloader->retrieveFileInfo())
    {
        _logger->error("Failed to download %s installation script", script);
        addLine("Failed to download installation script", true);
    }
    else
    {
        _logger->information("Downloaded %s installation script", script);
        addLine("Installation script downloaded");
    }

    _downloadThread = new CThreadWrapper<DownloadThreadWorker>(new DownloadThreadWorker(downloader),
                                                               true);
    _downloadThread->Start();
    _downloadThread->Join();
//    std::thread pDownloadThread = downloader->download();
//    pDownloadThread.join();

    SubutaiLauncher::SL sl(downloader->getOutputDirectory());
    sl.open(_script);

    try
    {
        _scriptThread = new CThreadWrapper<ScriptThreadWorker>(new ScriptThreadWorker(&sl),
                                                               true);
        _scriptThread->Start();

        auto nc = SubutaiLauncher::Session::instance()->getNotificationCenter();
        //        bool download = false;
        while (_running)
        {
            auto st = SubutaiLauncher::Session::instance()->getStatus();
            if (st != "") addLine(st);
            auto e = nc->dispatch();
            if (e == SubutaiLauncher::SCRIPT_FINISHED || SubutaiLauncher::Session::instance()->isTerminating() || !sl.running())
            {
                //pScriptThread.join();
                addLine("Script execution completed");
                _logger->information("%s script execution completed", script);
                _progress = 100.0;
                _running = false;
                if (sl.exitCode() == 0) _succeed = true;
            }
            else if (e == SubutaiLauncher::DOWNLOAD_STARTED)
            {
                //                download = true;
            }
            else if (e == SubutaiLauncher::DOWNLOAD_FINISHED)
            {
                //                download = false;
            }

            if (!nc->notificationEmpty())
            {
                auto pNotification = nc->dispatchNotification();
                if (pNotification.type == SubutaiLauncher::N_DOUBLE_DATA)
                {
                    double val = 0.0;
                    try
                    {
                        pNotification.message.convert(val);
                    }
                    catch (Poco::BadCastException& e)
                    {
                        _logger->error("Failed to convert progress value: %s", e.displayText());
                        _progress = -1.0;
                    }
                    _progress = val;
                }
                else if (pNotification.type == SubutaiLauncher::N_ERROR)
                {
                    std::string msg("");
                    try
                    {
                        pNotification.message.convert(msg);
                    }
                    catch (Poco::BadCastException& e)
                    {
                        _logger->error("Failed to convert error: %s", e.displayText());
                        msg = "Unknown error occured";
                    }
                    addLine(msg, true);
                }
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        _scriptThread->Join();
    }
    catch (SubutaiLauncher::SLException& e)
    {
        _scriptThread->Join();
        _running = false;
        _progress = 100.0;
        _logger->error(e.displayText());
    }

    _logger->debug("Stopping installation process and notifying parent");
    _running = false;
    auto parent = (Wizard*)getParentComponent();
    parent->stepCompleted(_name);
    _logger->trace("Parent notified");
}

void WizardInstall::addLine(const std::string& text, bool error)
{
    auto font = juce::Font("Encode Sans", 14, 0);
    auto line = new juce::Label();
    std::string t = _name;
    t.append(": ");
    t.append(text);
    line->setText(t, dontSendNotification);
    if (error) line->setColour(Label::textColourId, Colours::red);
    else line->setColour(Label::textColourId, Colour(105, 116, 144));
    line->setBounds(20, 100 + (_lines.size() * 30), 480, 30);
    line->setFont(font);
    line->setJustificationType(Justification::top);
    addAndMakeVisible(line);
    _lines.push_back(line);
}

bool WizardInstall::isRunning()
{
    return _running;
}

bool WizardInstall::isActive()
{
    return _active;
}

void WizardInstall::activate()
{
    if (_active)
    {
        _logger->error("Tried to activate installation screen that is already active");
        return;
    }
    _active = true;
    this->setVisible(true);
}

void WizardInstall::deactivate()
{
    if (!_active)
    {
        _logger->error("Tried to deactivate installation screen that is inactive");
        return;
    }
    _active = false;
    this->setVisible(false);
}

bool WizardInstall::succeed()
{
    return _succeed;
}
