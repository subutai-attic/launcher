#include "WizardInstall.h"
#include "Wizard.h"

WizardInstall::WizardInstall()
{
    auto font = juce::Font(20);
    _title = new juce::Label();
    _title->setText("Initializing", dontSendNotification);
    _title->setColour(Label::textColourId, Colours::white);
    _title->setBounds(20, 20, 480, 40);
    _title->setFont(font);
    _title->setJustificationType(Justification::top);
    addAndMakeVisible(_title);
}

WizardInstall::~WizardInstall()
{
    delete _title;
    delete _pb;
}

void WizardInstall::paint(juce::Graphics& g)
{
    g.fillAll (Colour::greyLevel (0.2f));
}

void WizardInstall::resized()
{

}

void WizardInstall::start(const std::string& name)
{
    SubutaiLauncher::Log::instance()->logger()->debug() << "Starting " << name << " installation" << std::endl;
    _name = name;
    if (_pb) {
        delete _pb;
    }
    std::string nt("Installing ");
    nt.append(name);
    _title->setText(nt, dontSendNotification);
    _progress = 0.0;
    _pb = new ProgressBar(_progress);
    _pb->setBounds(20, 60, 460, 25);
    addAndMakeVisible(_pb);

    // Converting component name to a script
    if (name == "P2P") {
        _script = "launcher-p2p-install";
    } else if (name == "Tray") {
        _script = "launcher-tray-install";
    } else if (name == "Browser Plugin") {
        _script = "launcher-e2e-install";
    } else if (name == "Peer") {
        _script = "launcher-peer-install";
    }
}

void WizardInstall::wait()
{
}

std::thread WizardInstall::run()
{
    return std::thread([=] { runImpl(); });
}

void WizardInstall::runImpl() {
    _running = true;
    SubutaiLauncher::Log::instance()->logger()->debug() << _name << " installation started" << std::endl;
    // Download installation script
    auto downloader = SubutaiLauncher::Session::instance()->getDownloader();
    auto script = _script;
    script.append(".py");
    downloader->reset();
    downloader->setFilename(script);
    if (!downloader->retrieveFileInfo()) {
        addLine("Failed to download installation script", true);
    } else {
        addLine("Installation script downloaded");
    }
    auto td = downloader->download();
    td.join();

    SubutaiLauncher::SL sl(downloader->getOutputDirectory());
    sl.open(_script);
    auto t = sl.executeInThread();
    t.detach();
    auto nc = SubutaiLauncher::Session::instance()->getNotificationCenter();
    bool download = false;
    while (_running) {
        auto st = SubutaiLauncher::Session::instance()->getStatus();
        if (st != "") addLine(st);
        auto e = nc->dispatch();
        if (e == SubutaiLauncher::SCRIPT_FINISHED) {
            addLine("Script execution completed");
            _progress = 100.0;
            _running = false;
        } else if (e == SubutaiLauncher::DOWNLOAD_STARTED) {
            download = true;
        } else if (e == SubutaiLauncher::DOWNLOAD_FINISHED) {
            download = false;
        }

        if (download) {
            _progress = (double)SubutaiLauncher::Session::instance()->getDownloader()->getPercent();
            SubutaiLauncher::Log::instance()->logger()->debug() << _progress << "%" << std::endl;
        }

        repaint();

#if LAUNCHER_LINUX || LAUNCHER_MACOS
        usleep(100);
#else
#error Not implemented on this platform
#endif
    }

    _running = false;
    auto parent = (Wizard*)getParentComponent();
    parent->stepCompleted(_name);
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
