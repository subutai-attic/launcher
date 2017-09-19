#include "ComponentChooser.h"

ComponentChooser::ComponentChooser() :
    _ptp(true),
    _tray(true),
    _ete(true),
    _peer(true),
    _rh(false),
    _cpu(2),
    _mem(2),
    _disk(100),
    _triggered(false),
    _ptpNo(nullptr),
    _ptpYes(nullptr),
    _trayNo(nullptr),
    _trayYes(nullptr),
    _eteNo(nullptr),
    _eteYes(nullptr),
    _peerNo(nullptr),
    _peerYes(nullptr),
    _rhNo(nullptr),
    _rhYes(nullptr),
    _cpuNum(nullptr),
    _cpuPlus(nullptr),
    _cpuMinus(nullptr),
    _memPlus(nullptr),
    _memSize(nullptr),
    _memMinus(nullptr),
    _diskPlus(nullptr),
    _diskSize(nullptr),
    _diskMinus(nullptr)
{
    SubutaiLauncher::Environment pEnv;
    _vtxStatus = false;
    if (pEnv.vtxEnabled()) _vtxStatus = true;
    _maxCpu = pEnv.cpuNum();
    _maxMem = 8;
    SubutaiLauncher::Session::instance()->getSettings()->setInstallationP2P(true);
    SubutaiLauncher::Session::instance()->getSettings()->setInstallationTray(true);
    SubutaiLauncher::Session::instance()->getSettings()->setInstallationE2E(true);
    if (_vtxStatus)
    {
        SubutaiLauncher::Session::instance()->getSettings()->setInstallationPeer(true);
        SubutaiLauncher::Session::instance()->getSettings()->setInstallationRH(false);
    }
    else
    {
        SubutaiLauncher::Session::instance()->getSettings()->setInstallationPeer(false);
        SubutaiLauncher::Session::instance()->getSettings()->setInstallationRH(false);
    }
    SubutaiLauncher::Session::instance()->getSettings()->setCoreNum(2);
    SubutaiLauncher::Session::instance()->getSettings()->setMemSize(2);
    _logger = &Poco::Logger::get("subutai");
    _logger->trace("Creating Component Chooser UI Component");

    auto font = juce::Font("Encode Sans", 15, 0);
    auto font2 = juce::Font("Encode Sans", 13, 0);

    struct RadioBtnComponentItem {
        juce::TextButton **btnDisable;
        juce::TextButton **btnEnable;

        uint32_t radioGroupId;
        bool btnEnableToggleState;

        juce::Label *lbl;
        std::string lblText;
        juce::Label *lblInfo;
        std::string lblInfoText;
    } ;

    std::vector<RadioBtnComponentItem> lstBtnItemsCont = {

        RadioBtnComponentItem{&_ptpNo, &_ptpYes, 20001, true, &_ptpLabel, "P2P", &_ptpInfo,
            "P2P Service to build mesh network with your environments"},

        RadioBtnComponentItem{&_trayNo, &_trayYes, 20002, true, &_trayLabel, "Tray Subutai Client",
            &_trayInfo, "Manages your environments. Requires P2P"},

        RadioBtnComponentItem{&_eteNo, &_eteYes, 20003, true, &_eteLabel, "E2E Browser Plugin",
            &_eteInfo, "E2E description"},

        RadioBtnComponentItem{&_peerNo, &_peerYes, 20004, true, &_peerLabel, "Create Subutai Peer",
            &_peerInfo, "Create new virtual machine with Subutai"},

        RadioBtnComponentItem{&_rhNo, &_rhYes, 20005, false, &_rhLabel, "Create Subutai RH",
            &_rhInfo, "Create new virtual machine with RH only"},
    };


    uint32_t lineY = 15;
    for (RadioBtnComponentItem item : lstBtnItemsCont) {
        static const uint32_t btnWidth = 55;
        static const uint32_t btnHeight = 30;
        static const uint32_t btnIndent1 = 20;
        static const uint32_t btnIndent2 = btnIndent1 + btnWidth;

        *item.btnDisable = new juce::TextButton("-");
        (*item.btnDisable)->setClickingTogglesState(true);
        (*item.btnDisable)->setRadioGroupId(item.radioGroupId);
        (*item.btnDisable)->setColour(TextButton::buttonColourId, Colour(247, 249, 252));
        (*item.btnDisable)->setColour(TextButton::buttonOnColourId, Colour(89, 183, 255));
        (*item.btnDisable)->setBounds(btnIndent1, lineY, btnWidth, btnHeight);
        (*item.btnDisable)->setConnectedEdges(Button::ConnectedOnRight);
        (*item.btnDisable)->setToggleState(!item.btnEnableToggleState, dontSendNotification);
        (*item.btnDisable)->addListener(this);

        *item.btnEnable = new juce::TextButton("+");
        (*item.btnEnable)->setClickingTogglesState(true);
        (*item.btnEnable)->setRadioGroupId(item.radioGroupId);
        (*item.btnEnable)->setColour(TextButton::buttonColourId, Colour(247, 249, 252));
        (*item.btnEnable)->setColour(TextButton::buttonOnColourId, Colour(89, 183, 255));
        (*item.btnEnable)->setBounds(btnIndent2, lineY, btnWidth, btnHeight);
        (*item.btnEnable)->setConnectedEdges(Button::ConnectedOnLeft);
        (*item.btnEnable)->setToggleState(item.btnEnableToggleState, dontSendNotification);
        (*item.btnEnable)->addListener(this);

        item.lbl->setText(item.lblText, dontSendNotification);
        item.lbl->setColour(Label::textColourId, Colour(105, 116, 144));
        item.lbl->setBounds(150, lineY, 500, 40);
        item.lbl->setFont(font);
        item.lbl->setJustificationType(Justification::top);

        item.lblInfo->setText(item.lblInfoText, dontSendNotification);
        item.lblInfo->setColour(Label::textColourId, Colours::grey);
        item.lblInfo->setBounds(150, lineY + 20, 500, 40);
        item.lblInfo->setFont(font2);
        item.lblInfo->setJustificationType(Justification::top);

        addAndMakeVisible(*item.btnDisable);
        addAndMakeVisible(*item.btnEnable);
        addAndMakeVisible(*item.lbl);
        addAndMakeVisible(*item.lblInfo);
        lineY += 40; //todo check this
    }


    struct PlusMinusBtn {
        juce::TextButton **minus;
        juce::TextButton **plus;
        juce::TextButton **btnText;
        juce::Label *lbl;
        juce::Label *lblInfo;
        std::string btnTextText;
        std::string toolTipMinus;
        std::string toolTipPlus;
        std::string lblText;
        std::string lblInfoText;
    };

    std::vector<PlusMinusBtn> lst_pm = {
        PlusMinusBtn {
            (&_cpuMinus), (&_cpuPlus), (&_cpuNum),
            &_cpuLabel, &_cpuInfo, "2",
            "Remove one core", "Add one core",
            "Number of CPUs", "Choose how many CPUs you would like to share with peer"
        },

        PlusMinusBtn {
            (&_memMinus), (&_memPlus), (&_memSize),
            &_memLabel, &_memInfo, "2",
            "Remove one GB", "Add one GB",
            "Memory Limit (GB)", "How many of RAM you would like to share with peer in GB"
        },

        PlusMinusBtn {
            (&_diskMinus), (&_diskPlus), (&_diskSize),
            &_diskLabel, &_diskInfo, "100",
            "Remove one GB", "Add one GB",
            "Disk size (GB)", "How much of disk size would you like to share with peer in GB"
        },
    };

    lineY = 15 + lstBtnItemsCont.size() * 40;

    for (PlusMinusBtn item : lst_pm) {

        (*item.minus) = new juce::TextButton("-");
        (*item.minus)->setTooltip(item.toolTipMinus);
        (*item.minus)->setColour(TextButton::buttonColourId, Colour(247, 249, 252));
        (*item.minus)->setBounds(20, lineY, 36, 30);
        (*item.minus)->setConnectedEdges(Button::ConnectedOnRight);
        (*item.minus)->addListener(this);

        (*item.btnText) = new juce::TextButton(item.btnTextText);
        (*item.btnText)->setColour(TextButton::buttonColourId, Colour(255, 255, 255));
        (*item.btnText)->setColour(TextButton::textColourOffId, Colour(105, 116, 144));
        (*item.btnText)->setColour(TextButton::textColourOnId, Colour(105, 116, 144));
        (*item.btnText)->setBounds(56, lineY, 36, 30);
        (*item.btnText)->setConnectedEdges(Button::ConnectedOnRight | Button::ConnectedOnLeft);

        (*item.plus) = new juce::TextButton("+");
        (*item.plus)->setColour(TextButton::buttonColourId, Colour(247, 249, 252));
        (*item.plus)->setBounds(92, lineY, 36, 30);
        (*item.plus)->setConnectedEdges(Button::ConnectedOnLeft);
        (*item.plus)->addListener(this);

        item.lbl->setText(item.lblText, dontSendNotification);
        item.lbl->setColour(Label::textColourId, Colour(105, 116, 144));
        item.lbl->setBounds(150, lineY, 500, 40);
        item.lbl->setFont(font);
        item.lbl->setJustificationType(Justification::top);

        item.lblInfo->setText(item.lblInfoText, dontSendNotification);
        item.lblInfo->setColour(Label::textColourId, Colours::grey);
        item.lblInfo->setBounds(150, lineY + 20, 500, 40);
        item.lblInfo->setFont(font2);
        item.lblInfo->setJustificationType(Justification::top);

        addAndMakeVisible(*item.minus);
        addAndMakeVisible(*item.btnText);
        addAndMakeVisible(*item.plus);
        addAndMakeVisible(item.lbl);
        addAndMakeVisible(item.lblInfo);
        lineY += 60;
    }

    _peerNo->setEnabled(_vtxStatus);
    _peerYes->setEnabled(_vtxStatus);
    _rhNo->setEnabled(_vtxStatus);
    _rhYes->setEnabled(_vtxStatus);
    _cpuMinus->setEnabled(_vtxStatus);
    _cpuPlus->setEnabled(_vtxStatus);
    _memPlus->setEnabled(_vtxStatus);
    _memMinus->setEnabled(_vtxStatus);
    _diskMinus->setEnabled(_vtxStatus);
    _diskPlus->setEnabled(_vtxStatus);
    _logger->trace("Component Chooser UI Component created");
}

ComponentChooser::~ComponentChooser()
{
    _logger->trace("Destroying Component Chooser UI Component");
    if (_ptpNo) delete _ptpNo;
    if (_ptpYes) delete _ptpYes;
    if (_trayNo) delete _trayNo;
    if (_trayYes) delete _trayYes;
    if (_eteNo) delete _eteNo;
    if (_eteYes) delete _eteYes;
    if (_peerNo) delete _peerNo;
    if (_peerYes) delete _peerYes;
    if (_rhNo) delete _rhNo;
    if (_rhYes) delete _rhYes;
    if (_cpuNum) delete _cpuNum;
    if (_cpuPlus) delete _cpuPlus;
    if (_cpuMinus) delete _cpuMinus;
    if (_memPlus) delete _memPlus;
    if (_memSize) delete _memSize;
    if (_memMinus) delete _memMinus;
    if (_diskPlus) delete _diskPlus;
    if (_diskSize) delete _diskSize;
    if (_diskMinus) delete _diskMinus;
}

void ComponentChooser::paint(juce::Graphics& g)
{
    g.fillAll(Colour(255, 255, 255));
}

void ComponentChooser::resized()
{

}

void ComponentChooser::buttonClicked(juce::Button* button)
{
    struct btnClickedHandler {
        juce::TextButton* btn;
        void (ComponentChooser::*handler)();
    };

    std::vector<btnClickedHandler> lstHandlers = {
        {_ptpYes,   &ComponentChooser::btnPtpYes_Clicked},
        {_ptpNo,    &ComponentChooser::btnPtpNo_Clicked},
        {_trayYes,  &ComponentChooser::btnTrayYes_Clicked},
        {_trayNo,   &ComponentChooser::btnTrayNo_Clicked},
        {_eteYes,   &ComponentChooser::btnETEYes_Clicked},
        {_eteNo,    &ComponentChooser::btnETENo_Clicked},
        {_peerYes,  &ComponentChooser::btnPeerYes_Clicked},
        {_peerNo,   &ComponentChooser::btnPeerNo_Clicked},
        {_rhYes,    &ComponentChooser::btnRHYes_Clicked},
        {_rhNo,     &ComponentChooser::btnRHNo_Clicked},

        {_cpuPlus,  &ComponentChooser::btnCPUPlus_Clicked},
        {_cpuMinus, &ComponentChooser::btnCPUMinus_Clicked},
        {_memPlus,  &ComponentChooser::btnMemPlus_Clicked},
        {_memMinus, &ComponentChooser::btnMemMinus_Clicked},
        {_diskPlus,  &ComponentChooser::btnDiskSizePlus_Clicked},
        {_diskMinus, &ComponentChooser::btnDiskSizeMinus_Clicked},
    };

    for (btnClickedHandler item : lstHandlers) {
        if (item.btn != button) continue;
        (this->*item.handler)();
        return;
    }

    _logger->error("Couldn't find handler in ComponentChooser::buttonClicked.");
}

ChosenComponents ComponentChooser::getComponents()
{
    ChosenComponents c;
    c.ptp = _ptp;
    c.tray = _tray;
    c.ete = _ete;
    c.peer = _peer;
    c.rh = _rh;
    c.cpu = _cpu;
    c.mem = _mem;
    return c;
}

void ComponentChooser::btnPtpYes_Clicked()
{
    _logger->trace("Activating P2P");
    _ptpLabel.setColour(Label::textColourId, Colour(105, 116, 144));
    SubutaiLauncher::Session::instance()->getSettings()->setInstallationP2P(true);
}

void ComponentChooser::btnPtpNo_Clicked()
{
    _logger->trace("Deactivating P2P");
    _ptpLabel.setColour(Label::textColourId, Colours::grey);
    SubutaiLauncher::Session::instance()->getSettings()->setInstallationP2P(false);
}

void ComponentChooser::btnTrayYes_Clicked()
{
    _logger->trace("Activating Tray");
    _trayLabel.setColour(Label::textColourId, Colour(105, 116, 144));
    SubutaiLauncher::Session::instance()->getSettings()->setInstallationTray(true);
}

void ComponentChooser::btnTrayNo_Clicked()
{
    _logger->trace("Deactivating Tray");
    _trayLabel.setColour(Label::textColourId, Colours::grey);
    SubutaiLauncher::Session::instance()->getSettings()->setInstallationTray(false);
}

void ComponentChooser::btnETEYes_Clicked()
{
    _logger->trace("Activating E2E");
    _eteLabel.setColour(Label::textColourId, Colour(105, 116, 144));
    SubutaiLauncher::Session::instance()->getSettings()->setInstallationE2E(true);
}

void ComponentChooser::btnETENo_Clicked()
{
    _logger->trace("Deactivating E2E");
    _eteLabel.setColour(Label::textColourId, Colours::grey);
    SubutaiLauncher::Session::instance()->getSettings()->setInstallationE2E(false);
}


static volatile bool btnYesNoInProgress = false;
void ComponentChooser::btnPeerYes_Clicked()
{
    _logger->trace("btnPeerYes_Clicked");
    if (btnYesNoInProgress) {
        _logger->trace("*** in progress ***");
        return;
    }
    btnYesNoInProgress = true;
    _rhNo->setToggleState(true, sendNotificationSync);
    SubutaiLauncher::Session::instance()->getSettings()->setInstallationPeer(true);
    SubutaiLauncher::Session::instance()->getSettings()->setInstallationRH(false);
    _peerLabel.setColour(Label::textColourId, Colour(105, 116, 144));
    _cpuLabel.setColour(Label::textColourId, Colour(105, 116, 144));
    _memLabel.setColour(Label::textColourId, Colour(105, 116, 144));
    auxSetPlusMinusComponentsEnabled(true);
    btnYesNoInProgress = false;
}

void ComponentChooser::btnPeerNo_Clicked()
{
    _logger->trace("btnPeerNo_Clicked");
    SubutaiLauncher::Session::instance()->getSettings()->setInstallationPeer(false);
    _peerLabel.setColour(Label::textColourId, Colours::grey);
    _cpuLabel.setColour(Label::textColourId, Colours::grey);
    _memLabel.setColour(Label::textColourId, Colours::grey);
    auxSetPlusMinusComponentsEnabled(false);
}

void ComponentChooser::btnRHYes_Clicked()
{
    _logger->trace("btnRHYes_Clicked");
    if (btnYesNoInProgress) {
        _logger->trace("*** in progress ***");
        return;
    }

    btnYesNoInProgress = true;
    _peerNo->setToggleState(true, sendNotificationSync);
    SubutaiLauncher::Session::instance()->getSettings()->setInstallationRH(true);
    SubutaiLauncher::Session::instance()->getSettings()->setInstallationPeer(false);

    _rhLabel.setColour(Label::textColourId, Colour(105, 116, 144));
    _cpuLabel.setColour(Label::textColourId, Colour(105, 116, 144));
    _memLabel.setColour(Label::textColourId, Colour(105, 116, 144));
    auxSetPlusMinusComponentsEnabled(true);
    btnYesNoInProgress = false;
}

void ComponentChooser::btnRHNo_Clicked()
{
    _logger->trace("btnRHNo_Clicked");
    SubutaiLauncher::Session::instance()->getSettings()->setInstallationRH(false);
    _rhLabel.setColour(Label::textColourId, Colours::grey);
    _cpuLabel.setColour(Label::textColourId, Colours::grey);
    _memLabel.setColour(Label::textColourId, Colours::grey);
    auxSetPlusMinusComponentsEnabled(false);
}

void ComponentChooser::btnCPUPlus_Clicked()
{
    _cpu++;
    SubutaiLauncher::Session::instance()->getSettings()->setInstallationCpuNum(_cpu);
    SubutaiLauncher::Session::instance()->getSettings()->setCoreNum(_cpu);
    char t[4];
    std::sprintf(t, "%d", _cpu);
    _cpuNum->setButtonText(t);
}

void ComponentChooser::btnCPUMinus_Clicked()
{
    _cpu--;
    if (_cpu <= 1) {
        _cpu = 1;
    }
    SubutaiLauncher::Session::instance()->getSettings()->setInstallationCpuNum(_cpu);
    SubutaiLauncher::Session::instance()->getSettings()->setCoreNum(_cpu);
    char t[4];
    std::sprintf(t, "%d", _cpu);
    _cpuNum->setButtonText(t);
}

void ComponentChooser::btnMemPlus_Clicked()
{
    _mem++;
    SubutaiLauncher::Session::instance()->getSettings()->setInstallationMemSize(_mem);
    SubutaiLauncher::Session::instance()->getSettings()->setMemSize(_mem);
    char t[4];
    std::sprintf(t, "%d", _mem);
    _memSize->setButtonText(t);
}

void ComponentChooser::btnMemMinus_Clicked()
{
    _mem--;
    if (_mem <= 1) {
        _mem = 1;
    }
    SubutaiLauncher::Session::instance()->getSettings()->setInstallationMemSize(_mem);
    SubutaiLauncher::Session::instance()->getSettings()->setMemSize(_mem);
    char t[4];
    std::sprintf(t, "%d", _mem);
    _memSize->setButtonText(t);
}

void ComponentChooser::btnDiskSizePlus_Clicked()
{
    _disk += 100;
    SubutaiLauncher::Session::instance()->getSettings()->setDiskSize(_disk);
    char t[4];
    std::sprintf(t, "%d", _disk);
    _diskSize->setButtonText(t);
}

void ComponentChooser::btnDiskSizeMinus_Clicked()
{
    _disk -= 100;
    if (_disk < 100) _disk = 100;
    SubutaiLauncher::Session::instance()->getSettings()->setDiskSize(_disk);
    char t[4];
    std::sprintf(t, "%d", _disk);
    _diskSize->setButtonText(t);
}

void ComponentChooser::auxSetPlusMinusComponentsEnabled(bool enabled)
{
    _cpuNum->setEnabled(enabled);
    _cpuPlus->setEnabled(enabled);
    _cpuMinus->setEnabled(enabled);
    _memSize->setEnabled(enabled);
    _memPlus->setEnabled(enabled);
    _memMinus->setEnabled(enabled);
    _diskMinus->setEnabled(enabled);
    _diskPlus->setEnabled(enabled);
    _diskSize->setEnabled(enabled);
}
