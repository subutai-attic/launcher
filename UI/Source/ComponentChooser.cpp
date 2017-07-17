#include "ComponentChooser.h"

ComponentChooser::ComponentChooser() :
    _ptp(true),
    _tray(true),
    _ete(true),
    _peer(true),
    _cpu(2),
    _mem(2)
{
    _logger = &Poco::Logger::get("subutai");
    _logger->trace("Creating Component Chooser UI Component");

    auto font = juce::Font(15);
    auto font2 = juce::Font(13);

    // P2P
    
    _ptpNo = new juce::TextButton("-");
	_ptpNo->setTooltip("Exclude P2P");
    _ptpNo->setClickingTogglesState(true);
    _ptpNo->setRadioGroupId(20001);
    _ptpNo->setColour(TextButton::buttonColourId, Colours::white);
    _ptpNo->setColour(TextButton::buttonOnColourId, Colours::blueviolet.brighter());
    _ptpNo->setBounds(20, 15, 55, 30);
    _ptpNo->setConnectedEdges(Button::ConnectedOnRight);
    _ptpNo->addListener(this);
    _ptpYes = new juce::TextButton("+");
	_ptpYes->setTooltip("Include P2P");
    _ptpYes->setClickingTogglesState(true);
    _ptpYes->setRadioGroupId(20001);
    _ptpYes->setColour(TextButton::buttonColourId, Colours::white);
    _ptpYes->setColour(TextButton::buttonOnColourId, Colours::blueviolet.brighter());
    _ptpYes->setBounds(75, 15, 55, 30);
    _ptpYes->setConnectedEdges(Button::ConnectedOnLeft);
    _ptpYes->setToggleState(true, dontSendNotification);
    _ptpYes->addListener(this);

    _ptpLabel.setText("P2P", dontSendNotification);
    _ptpLabel.setColour(Label::textColourId, Colours::white);
    _ptpLabel.setBounds(150, 15, 500, 40);
    _ptpLabel.setFont(font);
    _ptpLabel.setJustificationType(Justification::top);

    _ptpInfo.setText("P2P Service to build mesh network with your environments", dontSendNotification);
    _ptpInfo.setColour(Label::textColourId, Colours::grey);
    _ptpInfo.setBounds(150, 35, 500, 40);
    _ptpInfo.setFont(font2);
    _ptpInfo.setJustificationType(Justification::top);

    addAndMakeVisible(_ptpNo);
    addAndMakeVisible(_ptpYes);
    addAndMakeVisible(_ptpLabel);
    addAndMakeVisible(_ptpInfo);

    // Tray
    
    _trayNo = new juce::TextButton("-");
    _trayNo->setClickingTogglesState(true);
    _trayNo->setRadioGroupId(20002);
    _trayNo->setColour(TextButton::buttonColourId, Colours::white);
    _trayNo->setColour(TextButton::buttonOnColourId, Colours::blueviolet.brighter());
    _trayNo->setBounds(20, 55, 55, 30);
    _trayNo->setConnectedEdges(Button::ConnectedOnRight);
    _trayNo->addListener(this);
    _trayYes = new juce::TextButton("+");
    _trayYes->setClickingTogglesState(true);
    _trayYes->setRadioGroupId(20002);
    _trayYes->setColour(TextButton::buttonColourId, Colours::white);
    _trayYes->setColour(TextButton::buttonOnColourId, Colours::blueviolet.brighter());
    _trayYes->setBounds(75, 55, 55, 30);
    _trayYes->setConnectedEdges(Button::ConnectedOnLeft);
    _trayYes->setToggleState(true, dontSendNotification);
    _trayYes->addListener(this);

    _trayLabel.setText("Tray Subutai Client", dontSendNotification);
    _trayLabel.setColour(Label::textColourId, Colours::white);
    _trayLabel.setBounds(150, 55, 500, 40);
    _trayLabel.setFont(font);
    _trayLabel.setJustificationType(Justification::top);
    addAndMakeVisible(_trayLabel);

    _trayInfo.setText("Manages your environments. Requires P2P", dontSendNotification);
    _trayInfo.setColour(Label::textColourId, Colours::grey);
    _trayInfo.setBounds(150, 75, 500, 40);
    _trayInfo.setFont(font2);
    _trayInfo.setJustificationType(Justification::top);

    addAndMakeVisible(_trayNo);
    addAndMakeVisible(_trayYes);
    addAndMakeVisible(_trayLabel);
    addAndMakeVisible(_trayInfo);

    // E2E
    
    _eteNo = new juce::TextButton("-");
    _eteNo->setClickingTogglesState(true);
    _eteNo->setRadioGroupId(20003);
    _eteNo->setColour(TextButton::buttonColourId, Colours::white);
    _eteNo->setColour(TextButton::buttonOnColourId, Colours::blueviolet.brighter());
    _eteNo->setBounds(20, 95, 55, 30);
    _eteNo->setConnectedEdges(Button::ConnectedOnRight);
    _eteNo->addListener(this);
    _eteYes = new juce::TextButton("+");
    _eteYes->setClickingTogglesState(true);
    _eteYes->setRadioGroupId(20003);
    _eteYes->setColour(TextButton::buttonColourId, Colours::white);
    _eteYes->setColour(TextButton::buttonOnColourId, Colours::blueviolet.brighter());
    _eteYes->setBounds(75, 95, 55, 30);
    _eteYes->setConnectedEdges(Button::ConnectedOnLeft);
    _eteYes->setToggleState(true, dontSendNotification);
    _eteYes->addListener(this);

    _eteLabel.setText("E2E Browser Plugin", dontSendNotification);
    _eteLabel.setColour(Label::textColourId, Colours::white);
    _eteLabel.setBounds(150, 95, 500, 40);
    _eteLabel.setFont(font);
    _eteLabel.setJustificationType(Justification::top);

    _eteInfo.setText("E2E description", dontSendNotification);
    _eteInfo.setColour(Label::textColourId, Colours::grey);
    _eteInfo.setBounds(150, 115, 500, 40);
    _eteInfo.setFont(font2);
    _eteInfo.setJustificationType(Justification::top);

    addAndMakeVisible(_eteNo);
    addAndMakeVisible(_eteYes);
    addAndMakeVisible(_eteLabel);
    addAndMakeVisible(_eteInfo);

    // Subutai Peer
    
    _peerNo = new juce::TextButton("-");
    _peerNo->setClickingTogglesState(true);
    _peerNo->setRadioGroupId(20004);
    _peerNo->setColour(TextButton::buttonColourId, Colours::white);
    _peerNo->setColour(TextButton::buttonOnColourId, Colours::blueviolet.brighter());
    _peerNo->setBounds(20, 135, 55, 30);
    _peerNo->setConnectedEdges(Button::ConnectedOnRight);
    _peerNo->addListener(this);
    _peerYes = new juce::TextButton("+");
    _peerYes->setClickingTogglesState(true);
    _peerYes->setRadioGroupId(20004);
    _peerYes->setColour(TextButton::buttonColourId, Colours::white);
    _peerYes->setColour(TextButton::buttonOnColourId, Colours::blueviolet.brighter());
    _peerYes->setBounds(75, 135, 55, 30);
    _peerYes->setConnectedEdges(Button::ConnectedOnLeft);
    _peerYes->setToggleState(true, dontSendNotification);
    _peerYes->addListener(this);

    _peerLabel.setText("Create Subutai Peer", dontSendNotification);
    _peerLabel.setColour(Label::textColourId, Colours::white);
    _peerLabel.setBounds(150, 135, 500, 40);
    _peerLabel.setFont(font);
    _peerLabel.setJustificationType(Justification::top);

    _peerInfo.setText("Create new virtual machine with Subutai", dontSendNotification);
    _peerInfo.setColour(Label::textColourId, Colours::grey);
    _peerInfo.setBounds(150, 155, 500, 40);
    _peerInfo.setFont(font2);
    _peerInfo.setJustificationType(Justification::top);

    addAndMakeVisible(_peerNo);
    addAndMakeVisible(_peerYes);
    addAndMakeVisible(_peerLabel);
    addAndMakeVisible(_peerInfo);

    // CPUs

    _cpuMinus = new juce::TextButton("-");
	_cpuMinus->setTooltip("Remove one core");
    _cpuMinus->setColour(TextButton::buttonColourId, Colours::white);
    _cpuMinus->setBounds(20, 175, 36, 30);
    _cpuMinus->setConnectedEdges(Button::ConnectedOnRight);
    _cpuMinus->addListener(this);

    _cpuNum = new juce::TextButton("2");
    _cpuNum->setColour(TextButton::buttonColourId, Colour::greyLevel(0.2f));
    _cpuNum->setColour(TextButton::textColourOffId, Colours::white);
    _cpuNum->setColour(TextButton::textColourOnId, Colours::white);
    _cpuNum->setBounds(56, 175, 36, 30);
    _cpuNum->setConnectedEdges(Button::ConnectedOnRight | Button::ConnectedOnLeft);

    _cpuPlus = new juce::TextButton("+");
    _cpuPlus->setColour(TextButton::buttonColourId, Colours::white);
    _cpuPlus->setBounds(92, 175, 36, 30);
    _cpuPlus->setConnectedEdges(Button::ConnectedOnLeft);
    _cpuPlus->addListener(this);

    _cpuLabel.setText("Number of CPUs", dontSendNotification);
    _cpuLabel.setColour(Label::textColourId, Colours::white);
    _cpuLabel.setBounds(150, 175, 500, 40);
    _cpuLabel.setFont(font);
    _cpuLabel.setJustificationType(Justification::top);

    _cpuInfo.setText("Choose how many CPUs you would like to share with peer", dontSendNotification);
    _cpuInfo.setColour(Label::textColourId, Colours::grey);
    _cpuInfo.setBounds(150, 195, 500, 40);
    _cpuInfo.setFont(font2);
    _cpuInfo.setJustificationType(Justification::top);

    addAndMakeVisible(_cpuMinus);
    addAndMakeVisible(_cpuNum);
    addAndMakeVisible(_cpuPlus);
    addAndMakeVisible(_cpuLabel);
    addAndMakeVisible(_cpuInfo);

    // Memory
    
    _memMinus = new juce::TextButton("-");
    _memMinus->setColour(TextButton::buttonColourId, Colours::white);
    _memMinus->setBounds(20, 215, 36, 30);
    _memMinus->setConnectedEdges(Button::ConnectedOnRight);
    _memMinus->addListener(this);

    _memSize = new juce::TextButton("2");
    _memSize->setColour(TextButton::buttonColourId, Colour::greyLevel(0.2f));
    _memSize->setColour(TextButton::textColourOffId, Colours::white);
    _memSize->setColour(TextButton::textColourOnId, Colours::white);
    _memSize->setBounds(56, 215, 36, 30);
    _memSize->setConnectedEdges(Button::ConnectedOnRight | Button::ConnectedOnLeft);

    _memPlus = new juce::TextButton("+");
    _memPlus->setColour(TextButton::buttonColourId, Colours::white);
    _memPlus->setBounds(92, 215, 36, 30);
    _memPlus->setConnectedEdges(Button::ConnectedOnLeft);
    _memPlus->addListener(this);

    _memLabel.setText("Memory Limit (GB)", dontSendNotification);
    _memLabel.setColour(Label::textColourId, Colours::white);
    _memLabel.setBounds(150, 215, 500, 40);
    _memLabel.setFont(font);
    _memLabel.setJustificationType(Justification::top);

    _memInfo.setText("How many of RAM you would like to share with peer in GB", dontSendNotification);
    _memInfo.setColour(Label::textColourId, Colours::grey);
    _memInfo.setBounds(150, 235, 500, 40);
    _memInfo.setFont(font2);
    _memInfo.setJustificationType(Justification::top);

    addAndMakeVisible(_memPlus);
    addAndMakeVisible(_memSize);
    addAndMakeVisible(_memMinus);
    addAndMakeVisible(_memLabel);
    addAndMakeVisible(_memInfo);

    _logger->trace("Component Chooser UI Component created");
}

ComponentChooser::~ComponentChooser()
{
    _logger->trace("Destroying Component Chooser UI Component");
    delete _ptpNo;
    delete _ptpYes;
    delete _trayNo;
    delete _trayYes;
    delete _eteNo;
    delete _eteYes;
    delete _peerNo;
    delete _peerYes;
    delete _cpuNum;
    delete _cpuPlus;
    delete _cpuMinus;
    delete _memPlus;
    delete _memSize;
    delete _memMinus;
}

void ComponentChooser::paint(juce::Graphics& g)
{
    g.fillAll (Colour::greyLevel (0.2f));
}

void ComponentChooser::resized()
{

}

void ComponentChooser::buttonClicked(juce::Button* button)
{
    if (button == _ptpYes) 
    {
		_logger->trace("Activating P2P");
        _ptpLabel.setColour(Label::textColourId, Colours::white);
        SubutaiLauncher::Session::instance()->getSettings()->setInstallationP2P(true);
		return;
    } 
    else if (button == _ptpNo) 
    {
		_logger->trace("Deactivating P2P");
        _ptpLabel.setColour(Label::textColourId, Colours::grey);
        SubutaiLauncher::Session::instance()->getSettings()->setInstallationP2P(false);
		return;
    } 
    else if (button == _trayYes) 
    {
		_logger->trace("Activating Tray");
        _trayLabel.setColour(Label::textColourId, Colours::white);
        SubutaiLauncher::Session::instance()->getSettings()->setInstallationTray(true);
		return;
    }
    else if (button == _trayNo) 
    {
		_logger->trace("Deactivating Tray");
        _trayLabel.setColour(Label::textColourId, Colours::grey);
        SubutaiLauncher::Session::instance()->getSettings()->setInstallationTray(false);
		return;
    }
    else if (button == _eteYes)
    {
		_logger->trace("Activating E2E");
        _eteLabel.setColour(Label::textColourId, Colours::white);
        SubutaiLauncher::Session::instance()->getSettings()->setInstallationE2E(true);
		return;
    }
    else if (button == _eteNo)
    {
		_logger->trace("Deactivating E2E");
        _eteLabel.setColour(Label::textColourId, Colours::grey);
        SubutaiLauncher::Session::instance()->getSettings()->setInstallationE2E(false);
		return;
    }
    else if (button == _peerYes)
    {
		_logger->trace("Activating Peer");
        SubutaiLauncher::Session::instance()->getSettings()->setInstallationPeer(true);
        _peerLabel.setColour(Label::textColourId, Colours::white);
        _cpuLabel.setColour(Label::textColourId, Colours::white);
        _memLabel.setColour(Label::textColourId, Colours::white);
        _cpuNum->setEnabled(true);
        _cpuPlus->setEnabled(true);
        _cpuMinus->setEnabled(true);
        _memSize->setEnabled(true);
        _memPlus->setEnabled(true);
        _memMinus->setEnabled(true);
		return;
    }
    else if (button == _peerNo)
    {
		_logger->trace("Deactivating Peer");
        SubutaiLauncher::Session::instance()->getSettings()->setInstallationPeer(false);
        _peerLabel.setColour(Label::textColourId, Colours::grey);
        _cpuLabel.setColour(Label::textColourId, Colours::grey);
        _memLabel.setColour(Label::textColourId, Colours::grey);
        _cpuNum->setEnabled(false);
        _cpuPlus->setEnabled(false);
        _cpuMinus->setEnabled(false);
        _memSize->setEnabled(false);
        _memPlus->setEnabled(false);
        _memMinus->setEnabled(false);
		return;
    }
    else if (button == _cpuPlus) 
    {
        _cpu++;
        SubutaiLauncher::Session::instance()->getSettings()->setInstallationCpuNum(_cpu);
        char t[4];
        std::sprintf(t, "%d", _cpu);
        _cpuNum->setButtonText(t);
		return;
    }
    else if (button == _cpuMinus)
    {
        _cpu--;
        if (_cpu <= 1) {
            _cpu = 1;
        }
        SubutaiLauncher::Session::instance()->getSettings()->setInstallationCpuNum(_cpu);
        char t[4];
        std::sprintf(t, "%d", _cpu);
        _cpuNum->setButtonText(t);
		return;
    }
    else if (button == _memPlus) 
    {
        _mem++;
        SubutaiLauncher::Session::instance()->getSettings()->setInstallationMemSize(_mem);
        char t[4];
        std::sprintf(t, "%d", _mem);
        _memSize->setButtonText(t);
		return;
    }
    else if (button == _memMinus)
    {
        _mem--;
        if (_mem <= 1) {
            _mem = 1;
        }
        SubutaiLauncher::Session::instance()->getSettings()->setInstallationMemSize(_mem);
        char t[4];
        std::sprintf(t, "%d", _mem);
        _memSize->setButtonText(t);
		return;
    }
}

ChosenComponents ComponentChooser::getComponents()
{
    ChosenComponents c;
    c.ptp = _ptp;
    c.tray = _tray;
    c.ete = _ete;
    c.peer = _peer;
    c.cpu = _cpu;
    c.mem = _mem;
    return c;
}
