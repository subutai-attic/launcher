#include "Wizard.h"

Wizard::Wizard() :
    _step(1),
    _next("Next"),
    _cancel("Cancel"),
    _back("Back"),
    _shutdown(false)
{
    _logger = &Poco::Logger::get("subutai");
	SubutaiLauncher::Session::instance()->start();
#if LAUNCHER_LINUX
    SubutaiLauncher::RootProcess* rp = new SubutaiLauncher::RootProcess();
    rp->addCommand("mkdir -p /opt/subutai");
    rp->addCommand("mkdir -p /opt/subutai/bin");
    rp->addCommand("mkdir -p /opt/subutai/etc");
    rp->addCommand("mkdir -p /opt/subutai/resources");
    std::string chown("chown -R ");
    chown.append(Poco::Environment::get("USER"));
    chown.append(" /opt/subutai");
    rp->addCommand(chown);
    rp->execute("Creating installation directories requires root privileges");
    delete rp;
#elif LAUNCHER_MACOS
    Poco::File bin("/usr/local/share/subutai/bin");
    bin.createDirectories();
    bin = Poco::File("/usr/local/share/subutai/etc");
    bin.createDirectories();
#elif LAUNCHER_WINDOWS
	Poco::File bin(SubutaiLauncher::Session::instance()->getSettings()->getInstallationPath()+"/bin");
	if (!bin.exists())
	{
		bin.createDirectories();
	}
#endif

    setSize(800, 600);
    auto font = juce::Font("Encode Sans", 17, 1);

    _stepIntro.setText("Welcome", dontSendNotification);
    _stepIntro.setColour(Label::textColourId, juce::Colour(105, 116, 144));
    _stepIntro.setBounds(15, 50, 300, 40);
    _stepIntro.setFont(font);
    _stepIntro.setJustificationType(Justification::top);

    _stepSystemCheck.setText("System Check", dontSendNotification);
    _stepSystemCheck.setColour(Label::textColourId, juce::Colour(105, 116, 144));
    _stepSystemCheck.setBounds(15, 80, 300, 40);
    _stepSystemCheck.setFont(font);
    _stepSystemCheck.setJustificationType(Justification::top);

    _stepComponentChooser.setText("Choose Components", dontSendNotification);
    _stepComponentChooser.setColour(Label::textColourId, juce::Colour(105, 116, 144));
    _stepComponentChooser.setBounds(15, 110, 300, 40);
    _stepComponentChooser.setFont(font);
    _stepComponentChooser.setJustificationType(Justification::top);

    _stepInstall.setText("Install", dontSendNotification);
    _stepInstall.setColour(Label::textColourId, juce::Colour(105, 116, 144));
    _stepInstall.setBounds(15, 140, 300, 40);
    _stepInstall.setFont(font);
    _stepInstall.setJustificationType(Justification::top);

    _stepFinal.setText("Finish", dontSendNotification);
    _stepFinal.setColour(Label::textColourId, juce::Colour(105, 116, 144));
    _stepFinal.setBounds(15, 170, 300, 40);
    _stepFinal.setFont(font);
    _stepFinal.setJustificationType(Justification::top);

    addAndMakeVisible(_stepIntro);
    addAndMakeVisible(_stepSystemCheck);
    addAndMakeVisible(_stepComponentChooser);
    addAndMakeVisible(_stepInstall);
    addAndMakeVisible(_stepFinal);

    // Installation step pages

    _introPage = new WizardIntro();
    _introPage->setBounds(300, 0, 500, 600);
    addAndMakeVisible(_introPage);

    _systemCheckPage = new SystemCheck();
    _systemCheckPage->setBounds(300, 0, 500, 600);
    addChildComponent(_systemCheckPage);

    _logger->trace("ComponentChooser::Pre");
    _componentChooserPage = new ComponentChooser();
    _componentChooserPage->setBounds(300, 0, 500, 600);
    addChildComponent(_componentChooserPage);

    _ptpInstall = new WizardInstall();
    _ptpInstall->setBounds(300, 0, 500, 600);
    addChildComponent(_ptpInstall);

    _trayInstall = new WizardInstall();
    _trayInstall->setBounds(300, 0, 500, 600);
    addChildComponent(_trayInstall);

    _eteInstall = new WizardInstall();
    _eteInstall->setBounds(300, 0, 500, 600);
    addChildComponent(_eteInstall);

    _peerInstall = new WizardInstall();
    _peerInstall->setBounds(300, 0, 500, 600);
    addChildComponent(_peerInstall);

    _finishPage = new WizardFinish();
    _finishPage->setBounds(300, 0, 500, 600);
    addChildComponent(_finishPage);

    // Progression buttons

    _next.setBounds(202, 560, 86, 25);
    _next.addListener(this);
    _next.setColour(TextButton::buttonColourId, Colour(7,141,208));
    addAndMakeVisible(_next);

    _back.setBounds(106, 560, 86, 25);
    _back.addListener(this);
    _back.setEnabled(false);
    addAndMakeVisible(_back);

    _cancel.setBounds(10, 560, 86, 25);
    _cancel.addListener(this);
    addAndMakeVisible(_cancel);
}

Wizard::~Wizard()
{
    _shutdown = true;
    cleanInstallers();
    _logger->trace("Requesting Introduction page destroy");
    if (_introPage != nullptr) delete _introPage;
    _logger->trace("Requesting System Check page destroy");
    if (_systemCheckPage != nullptr) delete _systemCheckPage;
    _logger->trace("Requesting Component Selector page destroy");
    if (_componentChooserPage != nullptr) delete _componentChooserPage;
    _logger->trace("Requesting P2P Installation page destroy");
    if (_ptpInstall != nullptr) delete _ptpInstall;
    _logger->trace("Requesting Tray Installation page destroy");
    if (_trayInstall != nullptr) delete _trayInstall;
    _logger->trace("Requesting ETE Installation page destroy");
    if (_eteInstall != nullptr) delete _eteInstall;
    _logger->trace("Requesting Peer Installation page destroy");
    if (_peerInstall != nullptr) delete _peerInstall;
    _logger->trace("Requesting Finish page destroy");
    if (_finishPage != nullptr) delete _finishPage;
}

void Wizard::paint(juce::Graphics& g)
{
    g.fillAll (Colour(247, 249, 252));
}

void Wizard::resized()
{

}

void Wizard::buttonClicked(juce::Button* button)
{
    if (button == &_next) 
    {
        switch (_step) 
        {
            case 1:
                _introPage->setVisible(false);
                _systemCheckPage->setVisible(true);
                _stepIntro.setColour(Label::textColourId, Colours::white);
                _stepSystemCheck.setColour(Label::textColourId, juce::Colour(7, 141, 208));
                _back.setEnabled(true);
                _step = 2;
                break;
            case 2:
                _systemCheckPage->setVisible(false);
                _componentChooserPage->setVisible(true);
                _stepSystemCheck.setColour(Label::textColourId, Colours::white);
                _stepComponentChooser.setColour(Label::textColourId, juce::Colour(7, 141, 208));
                _step = 3;
                break;
            case 3:
                _componentChooserPage->setVisible(false);
                _ptpInstall->setVisible(true);
                _stepComponentChooser.setColour(Label::textColourId, Colours::white);
                _stepInstall.setColour(Label::textColourId, juce::Colour(7, 141, 208));
                _step = 4;
                _back.setEnabled(false);
                _next.setEnabled(false);
				runInstall();
                break;
            default:
                _introPage->setVisible(true);
                _back.setEnabled(false);
                _step = 1;
                _stepIntro.setColour(Label::textColourId, juce::Colour(7, 141, 208));
                break;
        }
    } 
    else if (button == &_back) 
    {
        switch (_step) 
        {
            case 1:
                break;
            case 2:
                _back.setEnabled(false);
                _systemCheckPage->setVisible(false);
                _introPage->setVisible(true);
                _stepIntro.setColour(Label::textColourId, juce::Colour(7, 141, 208));
                _stepSystemCheck.setColour(Label::textColourId, Colours::grey);
                _step = 1;
                break;
            case 3:
                _componentChooserPage->setVisible(false);
                _systemCheckPage->setVisible(true);
                _stepSystemCheck.setColour(Label::textColourId, juce::Colour(7, 141, 208));
                _stepComponentChooser.setColour(Label::textColourId, Colours::grey);
                _step = 2;
                break;
            default:
                break;
        }
    } 
    else if (button == &_cancel) 
    {
        runCancelConfirmation();
    }
}

void Wizard::runInstall()
{
    _logger->debug("Collecting choosen components");
    //auto c = _componentChooserPage->getComponents();
    SubutaiLauncher::InstallationSettings pSettings = SubutaiLauncher::Session::instance()->getSettings()->getInstallationSettings();
    // TODO: Replace this with Installation Settings
	SubutaiLauncher::Session::instance()->getSettings()->setCoreNum(pSettings.cpuNum);
	SubutaiLauncher::Session::instance()->getSettings()->setMemSize(pSettings.memSize);

    cleanInstallers();

    if (pSettings.installP2P && !_ptpInstalled) 
    {
        _ptpInstall->activate();
        _logger->debug("P2P Component has been choosen");
        _ptpInstall->start("P2P");
        _ptpInstall->run();
        return;
    }
    if (pSettings.installTray && !_trayInstalled) 
    {
        _trayInstall->activate();
        _logger->debug("Tray Component has been choosen");
        _trayInstall->start("Tray");
        _trayInstall->run();
        return;
    }
    if (pSettings.installE2E && !_eteInstalled) 
    {
        _eteInstall->activate();
        _logger->debug("Browser Plugin Component has been choosen");
        _eteInstall->start("Browser Plugin");
        _eteInstall->run();
        return;
    }
    if (pSettings.installPeer && !_peerInstalled) 
    {
        _peerInstall->activate();
        _logger->debug("Peer Component has been choosen");
        _peerInstall->start("Peer");
        _peerInstall->run();
        return;
    } 

    _logger->debug("Nothing to install");
    finish();
}

void Wizard::stepCompleted(const std::string& name)
{
    if (_shutdown)
    {
        return;
    }
    if (name == "P2P")
    {
        _ptpInstalled = true;
        _ptpInstall->setVisible(false);
    }
    else if (name == "Tray")
    {
        _trayInstalled = true;
        _trayInstall->setVisible(false);
    }
    else if (name == "Browser Plugin")
    {
        _eteInstalled = true;
        _eteInstall->setVisible(false);
    }
    else if (name == "Peer")
    {
        _peerInstalled = true;
        _peerInstall->setVisible(false);
    }

    // Determine if we need to install something else
    _logger->debug("Looking for next component");
    auto c = _componentChooserPage->getComponents();
    if ((c.ptp && !_ptpInstalled) || (c.tray && !_trayInstalled) || (c.ete && !_eteInstalled) || (c.peer && !_peerInstalled))
    {
        _logger->debug("Next component found");
        runInstall();
        return;
    }  
    else 
    {
        _logger->trace("Nothing else to install. Proceeding to a final page");
    }

    finish();
}

void Wizard::finish()
{
    cleanInstallers();
    // Show final page
    _cancel.setEnabled(false);
    _cancel.setVisible(false);
    _next.setEnabled(false);
    _next.setVisible(false);
    _back.setEnabled(false);
    _back.setVisible(false);

	auto c = _componentChooserPage->getComponents();
	_finishPage->addPTPResult(c.ptp, _ptpInstall->succeed());
	_finishPage->addTrayResult(c.tray, _trayInstall->succeed());
	_finishPage->addETEResult(c.ete, _eteInstall->succeed());
	_finishPage->addPeerResult(c.peer, _peerInstall->succeed());
    _finishPage->setVisible(true);

    _stepFinal.setColour(Label::textColourId, juce::Colour(7, 141, 208));
    _stepInstall.setColour(Label::textColourId, Colours::white);
}

void Wizard::cleanInstallers()
{
    _logger->trace("Cleaning install pages");
    if (_ptpInstall->isActive())
    {
        _logger->trace("Deactivating P2P Installation page");
        _ptpInstall->deactivate();
        if (_ptpInstall->isRunning())
        {
            _logger->trace("Waiting for thread to complete");
            _ptpInstall->wait();
        }
    }
    if (_trayInstall->isActive())
    {
        _logger->trace("Deactivating Tray Installation page");
        _trayInstall->deactivate();
        if (_trayInstall->isRunning())
        {
            _logger->trace("Waiting for thread to complete");
            _trayInstall->wait();
        }
    }
    if (_eteInstall->isActive())
    {
        _logger->trace("Deactivating ETE Installation page");
        _eteInstall->deactivate();
        if (_eteInstall->isRunning())
        {
            _logger->trace("Waiting for thread to complete");
            _eteInstall->wait();
        }
    }
    if (_peerInstall->isActive())
    {
        _logger->trace("Deactivating Peer Installation page");
        _peerInstall->deactivate();
        if (_peerInstall->isRunning())
        {
            _logger->trace("Waiting for thread to complete");
            _peerInstall->wait();
        }
    }
}

void Wizard::runCancelConfirmation()
{
    juce::String message = "Are you sure you want to cancel installation?";

    juce::DialogWindow::LaunchOptions options;
    _cancelMessage.setText(message, juce::dontSendNotification);
}
