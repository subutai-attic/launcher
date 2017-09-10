#include "WizardInstall.h"
#include "Wizard.h"

const std::string WizardInstall::P2P_INSTALL = "p2p-install";
const std::string WizardInstall::TRAY_INSTALL = "tray-install";
const std::string WizardInstall::E2E_INSTALL = "e2e-install";
const std::string WizardInstall::PEER_INSTALL = "peer-install";
const std::string WizardInstall::RH_INSTALL = "rh-install";

WizardInstall::WizardInstall() :
    _succeed(false),
    _running(false),
    _active(false),
    _scriptExitCode(-100)
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
    int rc = wait();
    if (rc == 0)
    {
        _logger->trace("Thread finished");
    }
    else
    {
        _logger->trace("Thread was not running");
    }
    _logger->trace("Destroying Wizard Install UI Component");
    for (auto it = _lines.begin(); it != _lines.end(); it++)
    {
        if ((*it) != nullptr) delete (*it);
    }
    _lines.clear();
    if (_title != nullptr) delete _title;
    if (_pb != nullptr) delete _pb;
    _logger->trace("Wizard Install UI Component has been destroyed");
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
    if (name == "P2P") 
    { 
        SubutaiLauncher::Session::instance()->setStep("P2P");
        _script = P2P_INSTALL;
    }
    else if (name == "Tray") 
    {
        SubutaiLauncher::Session::instance()->setStep("TRAY");
        _script = TRAY_INSTALL;
    }
    else if (name == "Browser Plugin") 
    {
        SubutaiLauncher::Session::instance()->setStep("E2E");
        _script = E2E_INSTALL;
    }
    else if (name == "Peer") 
    {
        SubutaiLauncher::Session::instance()->setStep("PEER");
        _script = PEER_INSTALL;
    }
    else if (name == "RH") 
    {
        SubutaiLauncher::Session::instance()->setStep("RH");
        _script = RH_INSTALL;
    }

    _logger->debug("Installation initializator has been finished");
}

int WizardInstall::wait()
{
    _logger->trace("WizardInstall::wait()");
    if (_installThread.joinable())
    {
        _installThread.join();
        _logger->trace("Install thread has been stopped");
        return 0;
    }
    _logger->trace("Install thread is not running");
    return 1;
}

void WizardInstall::run()
{
    _installThread = runThread();
}

std::thread WizardInstall::runThread()
{
    return std::thread([=] { runImpl(); });
}

std::thread WizardInstall::executeScript()
{
    return std::thread([=] { executeScriptImpl(); });
}

void WizardInstall::executeScriptImpl()
{
    SubutaiLauncher::SL sl(SubutaiLauncher::Session::instance()->getDownloader()->getOutputDirectory());
    sl.open(_script);
    try 
    {
        sl.execute();
    }
    catch (SubutaiLauncher::SLException& e)
    {
        _logger->critical("Script execution failed: %s", e.displayText());
    }
    catch (Poco::Exception& e)
    {
        _logger->critical("Script execution failed: %s", e.displayText());
    }
    catch (std::exception& e)
    {
        _logger->critical("Script execution failed due to unknown error");
    }
    _scriptExitCode = sl.exitCode();
}

void WizardInstall::runImpl()
{
    SubutaiLauncher::Session::instance()->getHub()->flushInfo();
    _running = true;
    _logger->information("%s installation started", _name);
    // Download installation script
    auto downloader = SubutaiLauncher::Session::instance()->getDownloader();

    auto script = _script;
    script.append(".py");
    try 
    {
        downloader->reset();
        downloader->setFilename(script);
        if (!downloader->retrieveFileInfo())
        {
            _logger->critical("Failed to download %s installation script", script);
            addLine("Failed to download installation script", true);
        }
        else
        {
            _logger->information("Downloaded %s installation script", script);
            addLine("Installation script downloaded");
        }
        downloader->downloadImpl();
    }
    catch (Poco::Exception& e)
    {
        _logger->critical("Failed to download installation script: %s", e.displayText());
        _running = false;
        auto parent = (Wizard*)getParentComponent();
        parent->stepCompleted(_name);
        _logger->trace("Parent notified");
        return;
    }
    catch (std::exception& e)
    {
        _logger->critical("Failed to download installation script: Unknown error");
        _running = false;
        auto parent = (Wizard*)getParentComponent();
        parent->stepCompleted(_name);
        _logger->trace("Parent notified");
        return;
    }

    std::thread pScriptThread = executeScript();
    auto nc = SubutaiLauncher::Session::instance()->getNotificationCenter();
    //        bool download = false;
    while (_running)
    {
        auto st = SubutaiLauncher::Session::instance()->getStatus();
        if (st != "") addLine(st);
        auto e = nc->dispatch();
        if (e == SubutaiLauncher::SCRIPT_FINISHED || SubutaiLauncher::Session::instance()->isTerminating())
        {
            if (pScriptThread.joinable()) pScriptThread.join();
            addLine("Script execution completed");
            _logger->information("%s script execution completed", script);
            _progress = 100.0;
            _running = false;
            if (_scriptExitCode == 0) _succeed = true;
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
        std::this_thread::sleep_for(std::chrono::microseconds(100));
    }
    //pScriptThread.join();
    if (pScriptThread.joinable()) pScriptThread.join();


    _logger->debug("Stopping installation process and notifying parent");
    _running = false;
    auto parent = (Wizard*)getParentComponent();
    parent->stepCompleted(_name);
    _logger->trace("Parent notified");
}

void WizardInstall::replaceLine(const std::string& text, bool error)
{
    auto it = _lines.back();
    delete it;
    _lines.pop_back();
    addLine(text, error);
}

void WizardInstall::addLine(const std::string& text, bool error)
{
    MessageManagerLock mmLock;
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
