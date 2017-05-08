#include "WizardInstall.h"
#include "Wizard.h"

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
    _running(false),
    _active(false)
{
    _pb = nullptr;
    _logger = &Poco::Logger::get("subutai");
    _logger->trace("Creating Wizard Install UI Component");
    auto font = juce::Font(20);
    _title = new juce::Label();
    _title->setText("Initializing", juce::dontSendNotification);
    _title->setColour(juce::Label::textColourId, juce::Colours::white);
    _title->setBounds(20, 20, 480, 40);
    _title->setFont(font);
    _title->setJustificationType(juce::Justification::top);
    addAndMakeVisible(_title);
}

WizardInstall::~WizardInstall()
{
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
    g.fillAll(juce::Colour::greyLevel (0.2f));
}

void WizardInstall::resized()
{

}

void WizardInstall::start(const std::string& name)
{
    _logger->information("Starting %s installation", name);
    _name = name;
    if (_pb != nullptr) {
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

int WizardInstall::wait()
{
    if (_running && _installThread.joinable())
    {
        _installThread.join();
        return 0;
    }
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

void WizardInstall::runImpl() 
{
    _running = true;
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
    std::thread pDownloadThread = downloader->download();
    pDownloadThread.join();

    SubutaiLauncher::SL sl(downloader->getOutputDirectory());
    sl.open(_script);
    std::thread pScriptThread;
    try 
    {
        pScriptThread = sl.executeInThread();
        auto nc = SubutaiLauncher::Session::instance()->getNotificationCenter();
        bool download = false;
        while (_running) 
        {
            auto st = SubutaiLauncher::Session::instance()->getStatus();
            if (st != "") addLine(st);
            auto e = nc->dispatch();
            if (e == SubutaiLauncher::SCRIPT_FINISHED) 
            {
                pScriptThread.join();
                addLine("Script execution completed");
                _logger->information("%s script execution completed", script);
                _progress = 100.0;
                _running = false;
            } 
            else if (e == SubutaiLauncher::DOWNLOAD_STARTED) 
            {
                download = true;
            } 
            else if (e == SubutaiLauncher::DOWNLOAD_FINISHED) 
            {
                download = false;
            }

            if (!nc->notificationEmpty()) 
            {
                auto pNotification = nc->dispatchNotification();
                if (pNotification.type == SubutaiLauncher::N_DOUBLE_DATA) 
                {
                    try 
                    {
                        _progress = pNotification.message.convert<double>();
                    } 
                    catch (Poco::BadCastException e) 
                    {
                        _logger->error("Failed to convert progress value");
                        _progress = 1.0;
                    }
                }
            }
#if LAUNCHER_LINUX || LAUNCHER_MACOS
            usleep(100);
#else
            Sleep(100);
#endif
        }
    } 
    catch (SubutaiLauncher::SLException& e)
    {
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
    auto font = juce::Font(14);
    auto line = new juce::Label();
    std::string t = _name;
    t.append(": ");
    t.append(text);
    line->setText(t, dontSendNotification);
    if (error) line->setColour(Label::textColourId, Colours::red);
    else line->setColour(Label::textColourId, Colours::white);
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
