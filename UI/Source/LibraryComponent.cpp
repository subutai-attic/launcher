/*
   ==============================================================================

   LibraryComponent.cpp
Created: 5 Aug 2016 5:58:23pm
Author:  crioto

==============================================================================
*/

#include "LibraryComponent.h"
// ============================================================================

struct {
    int instConfig = MH_FULL;
    int instVersion = DEV;
    std::string instDir = "/opt/subutai";
    std::string instTmpDir = "/tmp/subutai";
    long instCores = 2;
    long instRAM = 2048;

} instData;


// ============================================================================

LibraryComponent::LibraryComponent() : _installButton("Install")
                                       /*  
                                           _installButton("Installation Wizard"),
                                           _nextButton("Next"),
                                           _backButton("Back"),
                                           _cancelButton("Cancel"),
                                           _step(INTRO)
                                           */
{
    _logger = &Poco::Logger::get("subutai");
    _logger->debug("Starting Library UI Component");
    _componentTabs.push_back(new LibraryItemTab("Peers", 0, true));
    _componentTabs.push_back(new LibraryItemTab("P2P", 1, false));
    _componentTabs.push_back(new LibraryItemTab("Tray", 2, false));
    _componentTabs.push_back(new LibraryItemTab("E2E", 3, false));

    int pWidth = 20;

    for (auto it = _componentTabs.begin(); it != _componentTabs.end(); it++) 
    {
        (*it)->setBounds(pWidth, 0, (*it)->getWidth(), 30);
        addAndMakeVisible((*it));
        pWidth += (*it)->getWidth() + 20;
    }

    _installButton.addListener(this);
    _installButton.setColour(TextButton::buttonColourId, Colour(7,141,208));
    _installButton.toFront(true);
    addAndMakeVisible(_installButton);

    /*  
        instEnabled = true;

        _installButton.addListener(this);
        _nextButton.addListener(this);
        _backButton.addListener(this);
        _cancelButton.addListener(this);

        _systemCheck = new LibrarySystemCheck();
        _systemCheck->setComponentID("SystemCheck");
        _systemConfigure = new LibrarySystemConfigure();
        _systemConfigure->setComponentID("SystemConfigure");
        _install = new LibraryInstall();
        _install->setComponentID("SystemInstall");
        addAndMakeVisible(_systemCheck);
        addAndMakeVisible(_systemConfigure);
        addAndMakeVisible(_install);

        addChildComponent(_nextButton);
        addChildComponent(_backButton);
        addChildComponent(_cancelButton);
        auto l = SubutaiLauncher::Log::instance()->logger();

*/
}

LibraryComponent::~LibraryComponent() {
    //delete(_systemCheck);
    //delete(_systemConfigure);
    //delete(_download);
    //delete(_install);
}



void LibraryComponent::paint(Graphics& g) {
    g.fillAll (Colour (0xff222222));
    //g.fillAll (Colour (0xff333333));
    g.setFont (Font (16.0f));
    g.setColour (Colours::white);

    //    g.drawLine(10, 80, 800, 80, 1);
    //    g.drawLine(10, 340, 800, 340, 1);
    /*  
        g.drawLine(20, 80, 750, 80, 1);
        g.drawLine(20, 380, 750, 380, 1);
        */

    //g.drawText ("Library", getLocalBounds(), Justification::centred, true);
}

void LibraryComponent::resized() {
    _installButton.setBounds(getParentWidth() - 110 - 250, 10, 100, 35);

    /*  
        onStepChange();
        switch(_step) {
        case SYSTEM_CHECK:
        drawSystemCheck();
        break;
        case SYSTEM_CONFIGURE:
        drawSystemConfigure();
        break;
        case INSTALL:
        collectInstallData();
    //showChangedInfo();
    drawInstall();
    break;
    case FINISHED:
    drawFinished();
    break;
    default:
    drawIntro();
    break;
    */
    /*
       case DOWNLOAD:
       drawDownload();
       break;
       case PREINSTALL:
       drawPreinstall();
       break;
       case INSTALL:
       drawInstall();
       break;
       case POSTINSTALL:
       drawPostInstall();
       break;
       case FINISHED:
       drawFinished();
       break;
       default:
       drawIntro();
       break;
       */
    /*  
        };
        */
}

void LibraryComponent::buttonClicked(Button* button) 
{
    if (button == &_installButton) {
        _logger->trace("Install button clicked");
        // Start Wizard here
        WizardWindow *wizard = new WizardWindow();
        wizard->setVisible(true);
    }
    /*  
        if (button == &_installButton) {
        std::printf("Install button\n");
        _step = SYSTEM_CHECK;
        } else if (button == &_nextButton) {
        nextStep();
        } else if (button == &_backButton) {
        previousStep();
        } else if (button == &_cancelButton) {
    // TODO: Show cancelation dialog
    std::printf("Canceling installation\n");
    _step = INTRO;
    }
    resized();
    */
}

void LibraryComponent::tabClick(const std::string& tabTitle)
{
    for (auto it = _componentTabs.begin(); it != _componentTabs.end(); it++)
    {
        if ((*it)->title() == tabTitle) {
            (*it)->select();
            continue;
        }
        (*it)->deselect();
    }
}

/*  
    void LibraryComponent::componentVisibilityChanged(Component* component){
    NativeMessageBox::showMessageBox(AlertWindow::AlertIconType::InfoIcon, "LibraryInstall::componentVisibilityChanged", 
    mapInstallConfig[instData.instConfig], nullptr);
    }
    */

/*
   void LibrarySystemConfigure::textEditorTextChanged(TextEditor &teditor) {

   auto l = SubutaiLauncher::Log::instance()->logger();

//teditor->setColour(TextEditor::backgroundColourId, Colours::yellow);

//juce::String tname = teditor->getName();
//l->debug() << "LibrarySystemConfigure::textEditorTextChanged " << tname  << std::endl;

}

*/
/* 
   void LibraryComponent::textEditorFocusLost(TextEditor &teditor) {

   NativeMessageBox::showMessageBox(AlertWindow::AlertIconType::InfoIcon, "LibrarySystemConfigure::textEditorFocusLost", 
   "hhhhhhhhhhhhhhhhh", nullptr);
//teditor->getName()
auto l = SubutaiLauncher::Log::instance()->logger();

//teditor->setColour(TextEditor::backgroundColourId, Colours::green);
//juce::String tname = teditor->getName();
//l->debug() << "LibraryComponent::textEditorFocusLost " << tname  << std::endl;

}




void LibraryComponent::drawIntro() {

auto font = Font(24);
_componentsSectionLabel.setText("Subutai Components", dontSendNotification);
_componentsSectionLabel.setColour(Label::textColourId, Colours::white);
_componentsSectionLabel.setBounds(15, 50, 300, 40);
_componentsSectionLabel.setFont(font);
_componentsSectionLabel.setJustificationType(Justification::top);
addAndMakeVisible(_componentsSectionLabel);

_peersSectionLabel.setText("Subutai Peers", dontSendNotification);
_peersSectionLabel.setColour(Label::textColourId, Colours::white);
_peersSectionLabel.setBounds(15, 350, 300, 40);
_peersSectionLabel.setFont(font);
_peersSectionLabel.setJustificationType(Justification::top);
addAndMakeVisible(_peersSectionLabel);

auto l = SubutaiLauncher::Log::instance()->logger();
auto conf = SubutaiLauncher::Session::instance()->getConfManager();

auto configs = conf->getConfigs();
int i = 0;

int widthMainWindow = 1024;//MainContentComponent.WINDOW_WIDTH
int widthMenuWindow = 250;//MainContentComponent.MENU_WIDTH

//int offH = (widthMainWindow - widthMenuWindow - configs.size()*LibraryItem::WIDTH)/(configs.size() + 1);
int offH = (widthMainWindow - widthMenuWindow - 3*180)/4;
//int offH = 57;
for (auto it = configs.begin(); it != configs.end(); it++) {
int found = (*it).file.find("wiz", 0);
l->debug() << "in str: " << (*it).file << " wiz found : " << found <<std::endl;
if (found != std::string::npos)
continue;
std::string bs = (*it).file;
size_t index = 0;
std::string is = bs;
index = bs.find("install");
bs = bs.replace(index, 7, "update");
std::string us = bs;
index = bs.find("update");
bs = bs.replace(index, 6, "uninstall");
std::string rs = bs;

auto c = new LibraryItem((*it).title, (*it).description, is, us, rs);
//c->setBounds(i*220+20, 100, LibraryItem::WIDTH, LibraryItem::HEIGHT);
c->setBounds(i*(180+offH)+offH, 100, LibraryItem::WIDTH, LibraryItem::HEIGHT);
addAndMakeVisible(c);
_components.push_back(c);
++i;
}

SubutaiLauncher::VirtualBox vbox;
auto peerList = vbox.getPeers();
offH = (widthMainWindow - widthMenuWindow - (peerList.size() + 1)*180)/(peerList.size() + 2);
//if (peerList.size() == 0) {
auto p = new LibraryItem("Peer", "VM", "peer_install_tt", "peer_install_tt.py", "peer_install_tt");
//p->setBounds(20, 400, LibraryItem::WIDTH, LibraryItem::HEIGHT);
p->setBounds(offH, 400, LibraryItem::WIDTH, LibraryItem::HEIGHT);
addAndMakeVisible(p);
_peers.push_back(p);
//}

i = 1;
for (auto pt = peerList.begin(); pt != peerList.end(); pt++) {
    auto p = new LibraryItem((*pt).name, (*pt).id, "peer_install_tt", "peer_install_tt.py", "peer_install_tt");
    //p->setBounds(i*220+20, 400, LibraryItem::WIDTH, LibraryItem::HEIGHT);
    p->setBounds(i*(180+offH)+offH, 400, LibraryItem::WIDTH, LibraryItem::HEIGHT);
    addAndMakeVisible(p);
    _peers.push_back(p);
    i++;
}

addAndMakeVisible(_installButton);
_installButton.setVisible(true);
_installButton.setBounds(600, 20, 150, 35);
_installButton.setColour(TextButton::buttonColourId, Colour(7,141,208));
drawProgressButtons(false, false, false);


}

void LibraryComponent::hideIntro() {

    _componentsSectionLabel.setVisible(false);
    _peersSectionLabel.setVisible(false);

    auto conf = SubutaiLauncher::Session::instance()->getConfManager();

    int i = 0;
    auto l = SubutaiLauncher::Log::instance()->logger();
    for (auto c = _components.begin(); c != _components.end(); c++) 
    {
        (**c).setVisible(false);
        ++i;
    }

    i = 0;
    for (auto p = _peers.begin(); p != _peers.end(); p++) 
    {
        (**p).setVisible(false);
        ++i;
    }

    drawProgressButtons(false, false, false);
    _installButton.setVisible(false);
}


void LibraryComponent::drawSystemCheck() {
    hideIntro();
    drawProgressButtons(true, false, true);
    _systemCheck->setVisible(true);
    _systemCheck->setBounds(20, 20, 1024-250-40, 768 - 60 - 40);
    drawProgressButtons(true, false, true);
    _systemConfigure->setVisible(false);

    //_download->setVisible(false);
}

void LibraryComponent::drawSystemConfigure() {
    drawProgressButtons(true, true, true);
    _systemCheck->setVisible(false);
    //_download->setVisible(false);
    _install->setVisible(false);
    _systemConfigure->setVisible(true);
    _systemConfigure->setBounds(20, 20, 1024-250-40, 768 - 60 - 40);
    drawProgressButtons(true, true, true);
}
*/


/*
   void LibraryComponent::drawDownload() {

   auto l = SubutaiLauncher::Log::instance()->logger();
   l->debug()<< "drawDownload started" << std::endl;

#if LAUNCHER_LINUX
//const std::string& file("launcher-linux-install_tt.py");
const std::string& file("wiz_mh_install.py");
#elif LAUNCHER_WINDOWS
const std::string& file("launcher-windows-install.py");
#elif LAUNCHER_MACOS
#error Not Implemented for this platform
#else
#error Unknown Platform
#endif
_systemCheck->setVisible(false);
_systemConfigure->setVisible(false);
_download->setWithPeer(_systemConfigure->isPeerInstallChecked());
_download->setWithPeer(false);
_download->setVisible(true);
_download->setBounds(20, 20, 1024-250-40, 768 - 60 - 40);
_download->start();
drawProgressButtons(false, false, true);
auto t = waitDownloadComplete();
t.detach();
}
*/

/*  
    void LibraryComponent::drawPreinstall() {

    auto l = SubutaiLauncher::Log::instance()->logger();
    l->debug()<< "drawDownload started" << std::endl;

#if LAUNCHER_LINUX
//const std::string& file("launcher-linux-install_tt.py");
const std::string& file("wiz_mh_install.py");
#elif LAUNCHER_WINDOWS
const std::string& file("launcher-windows-install.py");
#elif LAUNCHER_MACOS
#error Not Implemented for this platform
#else
#error Unknown Platform
#endif
_systemCheck->setVisible(false);
_systemConfigure->setVisible(false);
//_download->setWithPeer(_systemConfigure->isPeerInstallChecked());
//_download->setWithPeer(false);
//_download->setVisible(false);
_install->setVisible(false);
//_preinstall->setVisible(true);
//_download->setBounds(20, 20, 1024-250-40, 768 - 60 - 40);
//_download->start();
drawProgressButtons(false, false, true);
//auto t = waitDownloadComplete();
//t.detach();
}

void LibraryComponent::drawInstall() {

drawProgressButtons(true, true, true);
_systemCheck->setVisible(false);
//_download->setVisible(false);
_systemConfigure->setVisible(false);
_install->setBounds(20, 20, 1024-250-40, 768 - 60 - 40);
_install->setVisible(true);
drawProgressButtons(true, true, true);
//LibraryInstall::showChangedInfo();
}

void LibraryComponent::drawPostInstall() {

}

void LibraryComponent::drawFinished() {

}

void LibraryComponent::onStepChange() {

_installButton.setVisible(false);
_systemCheck->setVisible(false);
_systemConfigure->setVisible(false);
//_download->setVisible(false);
_install->setVisible(false);
}

void LibraryComponent::drawProgressButtons(bool next, bool back, bool cancel) {
_cancelButton.setVisible(true);
_nextButton.setVisible(true);
_backButton.setVisible(true);
if (next) {
_nextButton.setBounds(660, 700, 100, 30);
_nextButton.setEnabled(true);
} else {
_nextButton.setEnabled(false);
_nextButton.setVisible(false);
}
if (back) {
_backButton.setBounds(540, 700, 100, 30);
_backButton.setEnabled(true);
} else {
    _backButton.setEnabled(false);
    _backButton.setVisible(false);
}
if (cancel) {
    _cancelButton.setBounds(420, 700, 100, 30);
    _cancelButton.setEnabled(true);
} else {
    _cancelButton.setEnabled(false);
    _cancelButton.setVisible(false);
}
}
*/

/*  
    void LibraryComponent::nextStep() {
    switch (_step) {
    case SYSTEM_CHECK:
    _step = SYSTEM_CONFIGURE;
    break;
    case SYSTEM_CONFIGURE:
    _step = INSTALL;
    break;
    case INSTALL:
    _step = FINISHED;
    break;
    */
/*
   case SYSTEM_CONFIGURE:
   _step = DOWNLOAD;
   break;
   case DOWNLOAD:
   _step = PREINSTALL;
   break;
   case PREINSTALL:
   _step = INSTALL;
   break;
   case INSTALL:
   _step = POSTINSTALL;
   break;
   case POSTINSTALL:
   _step = FINISHED;
   break;
   */
/*  
    }; 
    }
    */

/*
   void LibraryComponent::previousStep() {
   switch (_step) {
   case FINISHED:
   _step = INSTALL;
   break;
   case INSTALL:
   _step = SYSTEM_CONFIGURE;
   break;
   case SYSTEM_CONFIGURE:
   _step = SYSTEM_CHECK;
   break;
   */
/*
   case FINISHED:
   _step = POSTINSTALL;
   break;
   case POSTINSTALL:
   _step = INSTALL;
   break;
   case INSTALL:
   _step = PREINSTALL;
   break;
   case PREINSTALL:
   _step = DOWNLOAD;
   break;
   case DOWNLOAD:
   _step = SYSTEM_CONFIGURE;
   */
/*
   };
   }
   */

std::thread LibraryComponent::waitDownloadComplete() {
    return std::thread( [=] { waitDownloadCompleteImpl(); } );
}

void LibraryComponent::waitDownloadCompleteImpl() {
    //while (!_download->isComplete()) {
#if LAUNCHER_LINUX
    //    sleep(1);
#elif LAUNCHER_WINDOWS
    //	Sleep(1000);
#endif
    //if (_download->isCanceled()) {
    //    return;
    //}
    //}
    //drawProgressButtons(true, false, true);	
}

/*  
    void LibraryComponent::collectInstallData() {

    auto l = SubutaiLauncher::Log::instance()->logger();
    l->debug()<< "LibraryComponent::collectInstallData() " << std::endl;

//    instData.instDir = LibrarySystemConfigure::_installPathValue->getText();
//    instData.instTmpDir =  LibrarySystemConfigure::_installTmpValue.getText();
//    instData.instCores =  LibrarySystemConfigure::_installCoresValue.getText();
//    instData.instRAM =  LibrarySystemConfigure::_installRamValue.getText();

l->debug()<< "LibraryComponent::collectInstallData() instConfig: " << instData.instConfig << std::endl;
l->debug()<< "LibraryComponent::collectInstallData() instVersion: " << instData.instVersion <<  std::endl;
l->debug()<< "LibraryComponent::collectInstallData() instdDir: " << instData.instDir << std::endl;
l->debug()<< "LibraryComponent::collectInstallData() instTmpDir: " << instData.instTmpDir << std::endl;
l->debug()<< "LibraryComponent::collectInstallData() instCores: " << instData.instCores << std::endl;
l->debug()<< "LibraryComponent::collectInstallData() instRAM: " << instData.instRAM << std::endl;

}
*/

