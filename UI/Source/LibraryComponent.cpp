/*
   ==============================================================================

LibraryComponent.cpp
Created: 5 Aug 2016 5:58:23pm
Author:  crioto

==============================================================================
*/

#include "LibraryComponent.h"
// ============================================================================


bool instEnabled = true;
long minCores = 2;
long minRAM = 3600;

std::map <std::string, std::string> mapInstalled;
std::map <std::string, std::string> map2Install;
std::string vboxVersion;
std::vector <std::string> vComponents = {"Browser Plugin", "VBox", "Subutai Client", "Peer"};
std::map <int, std::string> mapInstallConfig = { {RH_ONLY, "RH only"}, {RH_CLIENT, "RH+Client"}, {MH_FULL, "RH+MH+Client"}, {CLIENT_ONLY, "Client only"} };
std::map <int, std::string> mapInstallVersion = { {PROD,"Current release version"}, {STAGE, "Stage snapshot"}, {DEV, "Dev snapshot"} };


struct {
    int instConfig = MH_FULL;
    int instVersion = DEV;
    std::string instDir = "/opt/subutai";
    std::string instTmpDir = "/tmp/subutai";
    long instCores = 2;
    long instRAM = 2048;

} instData;

LibraryItem::LibraryItem(const  std::string& title, const std::string& desc, const std::string& is, const std::string& us, const std::string& rs) : 
    _title(title),
    _desc(desc),
    _installScript(is),
    _updateScript(us),
    _removeScript(rs)
{
    auto l = SubutaiLauncher::Log::instance()->logger();

    if (title != "") 
    {
        auto font = Font(24);
        _titleLabel.setText(_title, dontSendNotification);
        _titleLabel.setColour(Label::textColourId, Colours::white);
        //_titleLabel.setBounds(0, 15, WIDTH, 40);
	_titleLabel.setBounds(0, 10, WIDTH, 40);
        _titleLabel.setFont(font);
        _titleLabel.setJustificationType(Justification::centred);
        _titleLabel.setInterceptsMouseClicks(false, true);
        addAndMakeVisible(_titleLabel);
    }

    auto fontPlus = Font(72);
    _plusLabel.setText("+", dontSendNotification);
    _plusLabel.setColour(Label::textColourId, Colours::white);
    if (title != "") 
    {
        _plusLabel.setBounds(0, 50, WIDTH, 100);
    } 
    else 
    {
        _plusLabel.setBounds(0, 0, WIDTH, 100);
    }
    _plusLabel.setInterceptsMouseClicks(false, true);
    _plusLabel.setFont(fontPlus);
    _plusLabel.setJustificationType(Justification::centred);
    addAndMakeVisible(_plusLabel);
    bool displayVersion = true;

    auto verFont = Font(12);
    _version.setInterceptsMouseClicks(false, true);
    _version.setColour(Label::textColourId, Colours::white);
    _version.setBounds(0, HEIGHT-40, WIDTH, 40);
    _version.setFont(verFont);
    _version.setJustificationType(Justification::bottomLeft);
    addAndMakeVisible(_version);

    drawVersion();

}

LibraryItem::~LibraryItem()
{

}

void LibraryItem::drawVersion()
{
    auto verFont = Font(12);
    _version.setInterceptsMouseClicks(false, true);
    _version.setColour(Label::textColourId, Colours::white);
    _version.setBounds(0, HEIGHT-40, WIDTH, 40);
    _version.setFont(verFont);
    _version.setJustificationType(Justification::bottomLeft);
    addAndMakeVisible(_version);
    std::string vText = findVersion(_title);
    if (vText != "") {
	_version.setText(vText, dontSendNotification);
	//_version.setText(mapInstalled[_title], dontSendNotification);
        _version.setVisible(true);
    } else {
	_version.setVisible(false);
    }
}


std::string LibraryItem::findVersion(std::string cname)
{
    std::string v = "";

    auto l = SubutaiLauncher::Log::instance()->logger();
    l->debug() << "LibraryComponent::findversion " << cname <<std::endl;

    if (cname == "P2P")
    {
        SubutaiLauncher::P2P p2p;
        p2p.findInstallation();
	//l->debug() << "LibraryComponent::constructor p2p is installed: " << p2p.isInstalled() << std::endl;
        //if (p2p.isInstalled()) {
	if (p2p.findInstallation()) {
	    //l->debug() << "LibraryComponent::constructor p2p version: " << p2p.extractVersion() << std::endl;
            v =  p2p.extractVersion();
        }
    } 
    else if (cname == "Tray Client")
    {
        SubutaiLauncher::Tray tray;
        tray.findInstallation();
	//l->debug() << "LibraryComponent::constructor tray is installed: " << tray.findInstallation() << std::endl;
        //if (tray.isInstalled()) {
	if (tray.findInstallation()) {
	    //l->debug() << "LibraryComponent::constructor tray version: " << tray.extractVersion() << std::endl;
            v = tray.extractVersion();
        }
    }
    else if (cname == "Browser Plugin")
    {
	SubutaiLauncher::Browser brw;
        brw.findInstallation();
	l->debug() << "LibraryComponent::LibraryItem browser is installed: " << brw.findInstallation() << " is installed: " << brw.isInstalled() << std::endl;
        if (brw.isInstalled()) {
	//if (tray.findInstallation()) {
	    //l->debug() << "LibraryComponent::constructor tray version: " << tray.extractVersion() << std::endl;
            v = brw.extractVersion();
	    v += "\nE2E";
        }
    }
    else if (cname == "VBox")
    {
        SubutaiLauncher::VirtualBox vbox;
        vbox.findInstallation();

	//l->debug() << "LibraryComponent::constructor vbox is installed: " << vbox.findInstallation() << std::endl;
        //if (vbox.isInstalled()) {
	if (vbox.findInstallation()) {
	    //l->debug() << "LibraryComponent::constructor vbox version: " << vbox.extractVersion() << std::endl;
            v = vbox.extractVersion();
        }
    }
    else if (cname == "Subutai Client")
    {
        SubutaiLauncher::P2P p2p;
        p2p.findInstallation();
	//l->debug() << "LibraryComponent::constructor p2p is installed: " << p2p.isInstalled() << std::endl;
        //if (p2p.isInstalled()) {
	if (p2p.findInstallation()) {
	    //l->debug() << "LibraryComponent::constructor p2p version: " << p2p.extractVersion() << std::endl;
            v = " P2P: " +  p2p.extractVersion();
        }
        SubutaiLauncher::Tray tray;
        tray.findInstallation();
	//l->debug() << "LibraryComponent::constructor tray is installed: " << tray.findInstallation() << std::endl;
        //if (tray.isInstalled()) {
	if (tray.findInstallation()) {
	    //l->debug() << "LibraryComponent::constructor tray version: " << tray.extractVersion() << std::endl;
            v = v + " Tray: " + tray.extractVersion();
        }
    } 
    else //we have peer
    {
        SubutaiLauncher::VirtualBox vbox;
	auto peerList = vbox.getPeers();
        if (peerList.size() == 0) {
	    v = "";
	}
	int i = 0;
	for (auto pt = peerList.begin(); pt != peerList.end(); pt++) {
	    i = cname.find((*pt).name, 0);
	    if (i != std::string::npos) {
		v = (*pt).id;
		break;
	    }
        }
    }
    return v;
}

void LibraryItem::paint(Graphics& g)
{
    g.setColour(Colours::white);
    g.drawRoundedRectangle(0, 0, WIDTH, HEIGHT, 4, 1);
}

void LibraryItem::resized()
{

}

void LibraryItem::mouseUp(const juce::MouseEvent& e)
{
    juce::PopupMenu menu;

    menu.addItem(1, "Install");
//    menu.addItem(2, "Update");
    menu.addItem(3, "Remove");    

    const int res = menu.show();
    if (res == 0) {

    } 
    else if (res == 1)
    {
        std::string windowTitle = "Installing ";
        windowTitle.append(_title);
        auto t = new LibraryActionThread("install", _title, windowTitle);
	
	SubutaiLauncher::Log::instance()->logger()->debug() << "LibraryComponent::LibraryItem::mouseUp before Launch thread " <<std::endl;

        //t->launchThread();
	if (!t->runThread()){
	SubutaiLauncher::Log::instance()->logger()->debug() << "LibraryComponent::LibraryItem::mouseUp cancelled " <<std::endl;
	    //return;
	}

	SubutaiLauncher::Log::instance()->logger()->debug() << "LibraryComponent::LibraryItem::mouseUp after launch thread " <<std::endl;

	/*
	while (t->isRunning()) {
            sleep(1);
	//Log::instance()->logger()->debug() << "LibraryComponent::LibraryItem::mouseUp   thread running " <<std::endl;
        }
	*/
	SubutaiLauncher::Log::instance()->logger()->debug() << "LibraryComponent::LibraryItem::mouseUp thread finished " <<std::endl;
    }
    else if (res == 2)
    {
        std::string windowTitle = "Updatiing ";
        windowTitle.append(_title);
        auto t = new LibraryActionThread("update", _title, windowTitle);
        t->launchThread();
        while (t->isRunning()) {
            sleep(1);
        }
	
    } 
    else if (res == 3)
    {
	std::string windowTitle = "Removing ";
        windowTitle.append(_title);
        auto t = new LibraryActionThread("uninstall", _title, windowTitle);
        /*
	t->launchThread();
        while (t->isRunning()) {
            sleep(1);
        }*/
	if (!t->runThread()){
	SubutaiLauncher::Log::instance()->logger()->debug() << "LibraryComponent::LibraryItem::mouseUp cancelled " <<std::endl;
	    //return;
	}
    }
    
    drawVersion();
    LibrarySystemCheck::appsInstalled();
}

// ============================================================================

LibraryComponent::LibraryComponent() : 
    _installButton("Installation Wizard"),
    _nextButton("Next"),
    _backButton("Back"),
    _cancelButton("Cancel"),
    _step(INTRO)
{

    instEnabled = true;

    _installButton.addListener(this);
    _nextButton.addListener(this);
    _backButton.addListener(this);
    _cancelButton.addListener(this);

    _systemCheck = new LibrarySystemCheck();
    _systemCheck->setComponentID("SystemCheck");
    _systemConfigure = new LibrarySystemConfigure();
    _systemConfigure->setComponentID("SystemConfigure");
    //_download = new LibraryDownload(false);
    _install = new LibraryInstall();
    _install->setComponentID("SystemInstall");
    //_install->addListener(this);
    //_preinstall = new LibraryPreinstall();
    addAndMakeVisible(_systemCheck);
    //_systemCheck->addListener(this);
    addAndMakeVisible(_systemConfigure);
    //_systemConfigure->addListener(this);
    //addAndMakeVisible(_download);
    addAndMakeVisible(_install);
    //_install->addListener(this);    

    addChildComponent(_nextButton);
    addChildComponent(_backButton);
    addChildComponent(_cancelButton);
    auto l = SubutaiLauncher::Log::instance()->logger();

}

LibraryComponent::~LibraryComponent() {
    delete(_systemCheck);
    delete(_systemConfigure);
    //delete(_download);
    delete(_install);
}



void LibraryComponent::paint(Graphics& g) {
    g.fillAll (Colour (0xff222222));
    //g.fillAll (Colour (0xff333333));
    g.setFont (Font (16.0f));
    g.setColour (Colours::white);

//    g.drawLine(10, 80, 800, 80, 1);
//    g.drawLine(10, 340, 800, 340, 1);
     g.drawLine(20, 80, 750, 80, 1);
     g.drawLine(20, 380, 750, 380, 1);


    //g.drawText ("Library", getLocalBounds(), Justification::centred, true);
}

void LibraryComponent::resized() {
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
    };
}

void LibraryComponent::buttonClicked(Button* button) {
    if (button == &_installButton) {
        std::printf("Install button\n");
        _step = SYSTEM_CHECK;
	LibrarySystemCheck::appsInstalled();
        /*  
            _installButton.setEnabled(false);
            auto dialog = new InstallationDialog("Subutai Installation", Colours::blue, DocumentWindow::allButtons);
            dialog->setBounds(0, 0, 800, 640);
            dialog->centreWithSize(dialog->getWidth(), dialog->getHeight());
            dialog->setTitleBarHeight(0);
            dialog->setVisible(true);
            */
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
}


void LibraryComponent::componentVisibilityChanged(Component* component){


    NativeMessageBox::showMessageBox(AlertWindow::AlertIconType::InfoIcon, "LibraryInstall::componentVisibilityChanged", 
	mapInstallConfig[instData.instConfig], nullptr);
}

/*
void LibrarySystemConfigure::textEditorTextChanged(TextEditor &teditor) {

    auto l = SubutaiLauncher::Log::instance()->logger();

    //teditor->setColour(TextEditor::backgroundColourId, Colours::yellow);

    //juce::String tname = teditor->getName();
    //l->debug() << "LibrarySystemConfigure::textEditorTextChanged " << tname  << std::endl;

}

*/
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
    }; 
}

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
    };
}

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


// ============================================================================

LibrarySystemCheck::LibrarySystemCheck() : _numLines(1) {

    auto l = SubutaiLauncher::Log::instance()->logger();

    int fieldFont = 20;
    SubutaiLauncher::Environment env;

    envCurrent.s_os = env.versionOS();
    envCurrent.b_os = true;
    envCurrent.s_arch = env.cpuArch();
    envCurrent.b_arch = true;
    envCurrent.i_cores = env.cpuNum();
    envCurrent.b_cores = true;
    envCurrent.l_ram =  env.ramSize();
    envCurrent.b_ram = true;
    envCurrent.s_vtx = env.vtxEnabled();
    envCurrent.b_vtx = true;

    //envCurrent.s_vbox = LibraryItem::findVersion("VBox");
    envCurrent.s_vbox = mapInstalled["VBox"];
   
/*
    SubutaiLauncher::VirtualBox vbox;
    vbox.findInstallation();
    std::string v;
    //l->debug() << "LibraryComponent::constructor vbox is installed: " << vbox.findInstallation() << std::endl;
    //if (vbox.isInstalled()) {
    if (vbox.findInstallation()) {
        //l->debug() << "LibraryComponent::constructor vbox version: " << vbox.extractVersion() << std::endl;
        v = vbox.extractVersion();
    }
    l->debug() << "LibrarySystemCheck() v=" << v << std::endl;
    envCurrent.s_vbox = v;
*/
    l->debug() << "LibrarySystemCheck()  envCurrent.s_vbox=" <<  envCurrent.s_vbox << std::endl;
    envCurrent.b_vbox = true;

    checkSystem();

    //Notification note;
    _osField.setFont(fieldFont);
    addAndMakeVisible(_osField);
    addAndMakeVisible(_osValue);
    _osValue.setText(envCurrent.s_os, dontSendNotification);
    addAndMakeVisible(_osHint);
    addLine(&_osField, &_osValue, &_osHint, "OS vesion", "We need version > 16", envCurrent.b_os);

    _if64Field.setFont(fieldFont);
    addAndMakeVisible(_if64Field);
    addAndMakeVisible(_if64Value);
    _if64Value.setText(envCurrent.s_arch, dontSendNotification);
    addAndMakeVisible(_if64Hint);
    addLine(&_if64Field, &_if64Value, &_if64Hint, "Processor architecture", 
	"We need x64 architecture", envCurrent.b_arch);

    addAndMakeVisible(_numCpuField);
    addAndMakeVisible(_numCpuValue);
    _numCpuValue.setText(std::to_string(envCurrent.i_cores), dontSendNotification);
    addAndMakeVisible(_numCpuHint);
    addLine(&_numCpuField, &_numCpuValue, &_numCpuHint, "Number of CPU Cores", 
	"Each peer requires at least 2 CPU cores", envCurrent.b_cores);

    addAndMakeVisible(_maxMemoryField);
    addAndMakeVisible(_maxMemoryValue);
    addAndMakeVisible(_maxMemoryHint);
    _maxMemoryValue.setText(std::to_string(envCurrent.l_ram), dontSendNotification);
    addLine(&_maxMemoryField, &_maxMemoryValue, &_maxMemoryHint, "Total System Memory", 
	"2GB will be taken to each peer's virtual machine", envCurrent.b_ram);
//	"4GB will be taken to each peer's virtual machine", envCurrent.b_ram);

    addAndMakeVisible(_vtxField);
    addAndMakeVisible(_vtxValue);
    addAndMakeVisible(_vtxHint);
    _vtxValue.setText(envCurrent.s_vtx, dontSendNotification);
    addLine(&_vtxField, &_vtxValue, &_vtxHint, "Hardware Virtualization Support", 
	"VTx should be supported by your CPU", envCurrent.b_vtx);

    addAndMakeVisible(_vboxField);
    addAndMakeVisible(_vboxValue);
    addAndMakeVisible(_vboxHint);


    //_vboxValue.setText(envCurrent.s_vbox, dontSendNotification);
    _vboxValue.setText(mapInstalled["VBox"], dontSendNotification);
    //_vboxValue.setText(v, dontSendNotification);
    addLine(&_vboxField, &_vboxValue, &_vboxHint, "Oracle VirtualBox version",
	"We're running our peer on VirtualBox VMs", envCurrent.b_vbox);

/*
    addAndMakeVisible(_sshField);
    addAndMakeVisible(_sshValue);
    addAndMakeVisible(_sshHint);
    _sshValue.setText( env.versionSSH(), dontSendNotification);
    addLine(&_sshField, &_sshValue, &_sshHint, "SSH client version", "SSH client is used to configure peer during installation");
*/
 
    auto font = Font(18);

    if (instEnabled){
	_info.setColour(Label::textColourId, Colours::green);
	_info.setText("Subutai Social can be installed on Your machine", dontSendNotification);
    } else {
	_info.setColour(Label::textColourId, Colours::blue);
	_info.setText("Subutai Social can not installed on Your machine, but You can install Subutai Client", dontSendNotification);
    }

    
    _info.setBounds(0, 500, 800, 440);
    _info.setFont(font);
    _info.setJustificationType(Justification::top);
    addAndMakeVisible(_info);


}

LibrarySystemCheck::~LibrarySystemCheck() {

}

void LibrarySystemCheck::appsInstalled(){
    std::string v = "";
    std::string v1 = "";
    mapInstalled.clear();
    auto l = SubutaiLauncher::Log::instance()->logger();
    //P2P 
    SubutaiLauncher::P2P p2p;
    SubutaiLauncher::Tray tray;

    //if (p2p.findInstallation()){
    if (p2p.findInstallation() && tray.findInstallation()) {
        v = p2p.extractVersion();
	SubutaiLauncher::SubutaiString st(v);
	v = st.remove("\n", "");
        l->debug() << "LibrarySystemCheck::appsInstalled p2p version v: " << v << "/ " << std::endl;
	//if (tray.findInstallation()){
    	v1 = tray.extractVersion();
    	l->debug() << "LibrarySystemCheck::appsInstalled tray version v1: " << v1 << "/ extract: " << tray.extractVersion()  << std::endl;
	//}
	if (v != "") {
	    v.append(";");
	    if (v1 != "")
		v.append(v1);
	    mapInstalled["Subutai Client"] =  v;
	    l->debug() << "LibrarySystemCheck::appsInstalled p2p and tray map: " << mapInstalled["Subutai Client"] << std::endl;
	}
    }

    SubutaiLauncher::Browser brw;
    if (brw.findInstallation()) {
        v = brw.extractVersion();

	v += "\nE2E: Plugin";
	//mapInstalled["E2E"] = v;
	if (v != "")
	    mapInstalled["Browser Plugin"] = v;
	l->debug() << "LibrarySystemCheck::appsInstalled version: " << v << std::endl;
	l->debug() << "LibrarySystemCheck::appsInstalled browser map: " << mapInstalled["Browser Plugin"] << std::endl;
    }

    SubutaiLauncher::VirtualBox vbox;
    //l->debug() << "LibraryComponent::constructor vbox is installed: " << vbox.findInstallation() << std::endl;
    //if (vbox.isInstalled()) {
    if (vbox.findInstallation()) {
	v = vbox.extractVersion();
	l->debug() << "LibrarySystemCheck::appsInstalled vbox map: " << mapInstalled.size() << std::endl;
	if (v != "")
	    mapInstalled["VBox"] = v;
	l->debug() << "LibrarySystemCheck::appsInstalled vbox map: " << mapInstalled["VBox"] << std::endl;
    }

    auto peerList = vbox.getPeers();
    if (peerList.size() > 0) {
	mapInstalled["Peer"] = "Peer installed";
    }
}


bool LibrarySystemCheck::checkSystem(){

    auto l = SubutaiLauncher::Log::instance()->logger();

    instEnabled = true;
    int found = 0;
    found = envCurrent.s_os.find("xenial", 0);
    if (found == std::string::npos){
	envCurrent.b_os = false;
	instEnabled = false;
    }

    found = envCurrent.s_arch.find("64", 0);
    if (found == std::string::npos){
	envCurrent.b_arch = false;
	instEnabled = false;
    }

    if (envCurrent.i_cores < minCores){
	envCurrent.b_cores = false;
	instEnabled = false;
    }

    if (envCurrent.l_ram < minRAM){
	envCurrent.b_ram = false;
	instEnabled = false;
    }

    found = envCurrent.s_vtx.find("VT-x", 0);
    if (found == std::string::npos){
	envCurrent.b_vtx = false;
	instEnabled = false;
    }


    envCurrent.s_vbox = mapInstalled["VBox"];
    found = envCurrent.s_vbox.find("5.1", 0);

    if (found == std::string::npos){
	envCurrent.b_vbox = false;
	instEnabled = false;
    }
    l->debug() << "LibrarySystemCheck::checkSystem() instEnabled=" << instEnabled << std::endl;
    return instEnabled;
}

void LibrarySystemCheck::addLine(Label* field, Label* value, Label* hint, std::string text, std::string hintText, bool inst) {
    auto font = Font(18);
    auto fieldFont = Font(20);
    int offLeft = 350;

    field->setText(text, dontSendNotification);
    field->setColour(Label::textColourId, Colours::white);
    field->setBounds(0, _numLines * 50, 300, 50);
    field->setFont(fieldFont);
    field->setJustificationType(Justification::top);

    //value->setText("Checking...", dontSendNotification);
    if (inst) 
	value->setColour(Label::textColourId, Colours::white);
    else 
	value->setColour(Label::textColourId, Colours::red);
    //value->setBounds(320, _numLines * 50, 800, 50);
    value->setBounds(offLeft, _numLines * 50, 800, 50);
    value->setFont(font);
    value->setJustificationType(Justification::top);

    hint->setText(hintText, dontSendNotification);
    hint->setColour(Label::textColourId, Colours::grey);
    //hint->setBounds(320, _numLines * 50 + 20, 800, 50);
    hint->setBounds(offLeft, _numLines * 50 + 20, 800, 50);
    hint->setFont(font);
    hint->setJustificationType(Justification::top);

    _numLines++;
}

void LibrarySystemCheck::paint(Graphics& g) {
    g.fillAll (Colour (0xff222222));
    //g.setFont (Font (16.0f));
    g.setFont (Font (18.0f));
    //g.setColour (Colours::white);
    g.setColour(Colour(200,200,200));
}

void LibrarySystemCheck::resized() {
    appsInstalled();
    envCurrent.s_vbox = mapInstalled["VBox"];//LibraryItem::findVersion("VBox");
    envCurrent.b_vbox = true;
    _vboxValue.setText(mapInstalled["VBox"], dontSendNotification);
    checkSystem();

}

// ============================================================================

LibrarySystemConfigure::LibrarySystemConfigure() {

    auto l = SubutaiLauncher::Log::instance()->logger();
    l->debug()<< "LibrarySystemConfigure constructor start"  << std::endl;

    int fieldFont = 20;
    //Installation Configuration section
    int offConf = 50;
    int offLeft = 400;
    _installConfField.setText("Choose Installation Configuration", dontSendNotification);
    _installConfField.setColour(Label::textColourId, Colours::white);
    //_installConfField.setBounds(0, 5, 500, 440);
    _installConfField.setBounds(0, offConf + 5, 290, 30);
    _installConfField.setFont(fieldFont);
    _installConfField.setJustificationType(Justification::top);


    auto conf = SubutaiLauncher::Session::instance()->getConfManager();

    auto configs = conf->getConfigs();
    int i = 0;
    for (auto it = configs.begin(); it != configs.end(); it++) {
        int found = (*it).file.find("wiz", 0);
	if (found == std::string::npos)
	    continue;
	auto b = new ToggleButton((*it).title);
        b->setRadioGroupId(11);
        //b->setBounds(300, offConf+i*30, 200, 30);
	b->setBounds(offLeft, offConf+i*30, 200, 30);
	b->setClickingTogglesState(true);
        b->setColour(ToggleButton::textColourId, Colours::white);
	//b->setColour(ToggleButton::textColourId, Colour(200,200,200));
	b->addListener(this);
	
        if (i == 2) {
	    b->triggerClick();
        }
        addAndMakeVisible(b);
	b->setEnabled(true);
        _installTypes.push_back(b);
        i++;
    }

//Installation Version section

    int offVersion = 200;
    _installTypeField.setText("Choose Version", dontSendNotification);
    _installTypeField.setColour(Label::textColourId, Colours::white);
    _installTypeField.setBounds(0, offVersion + 5, 290, 30);
    _installTypeField.setFont(fieldFont);
    _installTypeField.setJustificationType(Justification::top);


    _installProd = new ToggleButton("Install latest stable release");
    _installProd->setRadioGroupId(22);
    _installProd->setBounds(offLeft, offVersion, 200, 30);
    //_installProd->setColour(ToggleButton::textColourId, Colours::white);
    _installProd->setColour(ToggleButton::textColourId, Colour(230,230,230));
    _installProd->setEnabled(true);
    _installProd->addListener(this);
    _installProd->setClickingTogglesState(true);
    //_installProd->triggerClick();

    _installMaster = new ToggleButton("stage: Install stage snapshot");
    _installMaster->setRadioGroupId(22);
    _installMaster->setBounds(offLeft, offVersion + 30 , 200, 30);
    //_installMaster->setColour(ToggleButton::textColourId, Colours::white);
    _installMaster->setColour(ToggleButton::textColourId, Colour(230,230,230));
    _installMaster->setEnabled(true);
    _installMaster->addListener(this);
    _installMaster->setClickingTogglesState(true);
    //_installMaster->triggerClick();

    _installDev = new ToggleButton("dev: Install current development snapshot");
    _installDev->setRadioGroupId(22);
    _installDev->setBounds(offLeft,  offVersion + 60, 300, 30);
    //_installDev->setColour(ToggleButton::textColourId, Colours::white);
    _installDev->setColour(ToggleButton::textColourId, Colour(230,230,230));
    _installDev->setEnabled(true);
    _installDev->addListener(this);
    _installDev->setClickingTogglesState (true);
    _installDev->triggerClick();


    l->debug()<< "LibrarySystemConfigure text section  start"  << std::endl;
//Installation directories section
// Path
    int offPath = 350;
    _installPathField.setText("Installation path", dontSendNotification);
    _installPathField.setColour(Label::textColourId, Colours::white);
    _installPathField.setColour(TextEditor::backgroundColourId, Colour(200,200,200));
    //_installPathField.setBounds(0,  hoff + 140, 500, 440);
    _installPathField.setBounds(0, offPath + 5, 290, 30);
    _installPathField.setFont(fieldFont);
    _installPathField.setJustificationType(Justification::top);

    l->debug()<< "LibrarySystemConfigure value set text before start"  << std::endl;
    _installPathValue = new TextEditor("");
    _installPathValue->setText("/opt/subutai");
    //_installPathValue.setBounds(300,  hoff + 140, 200, 25);
    _installPathValue->setBounds(offLeft, offPath, 200, 25);
    _installPathValue->setComponentID("Path");
    _installPathValue->addListener(this);

    // Tmp
    //

    _installTmpField.setText("Temporary directory", dontSendNotification);
    _installTmpField.setColour(Label::textColourId, Colours::white);
    _installTmpField.setColour(TextEditor::backgroundColourId, Colour(230,230,230));

    //_installTmpField.setBounds(0, hoff + 180, 500, 440);
    _installTmpField.setBounds(0, offPath + 35, 500, 30);
    _installTmpField.setFont(fieldFont);
    _installTmpField.setJustificationType(Justification::top);

    _installTmpValue = new TextEditor();
    _installTmpValue->setText("/tmp/subutai");
    _installTmpValue->setBounds(offLeft, offPath + 30, 200, 25);
    _installTmpValue->setComponentID("Tmp");
    _installTmpValue->addListener(this);

    //Cores

    int maxTextLength = 2;
    juce::String allowedCharacters = "0123456789";
    
    int offCores = 420;
    _installCoresField.setText("Cores to share", dontSendNotification);
    _installCoresField.setColour(Label::textColourId, Colours::white);
    _installCoresField.setColour(TextEditor::backgroundColourId, Colour(200,200,200));
    //_installPathField.setBounds(0,  hoff + 140, 500, 440);
    _installCoresField.setBounds(0, offCores + 5, 290, 30);
    _installCoresField.setFont(fieldFont);
    _installCoresField.setJustificationType(Justification::top);

    _installCoresValue = new TextEditor();
    _installCoresValue->setText("2");
    //_installPathValue.setBounds(300,  hoff + 140, 200, 25);
    _installCoresValue->setBounds(offLeft, offCores, 200, 25);
    _installCoresValue->setComponentID("Cores");

    maxTextLength = 6;
    _installCoresValue->setInputRestrictions(maxTextLength, "0123456789");
    _installCoresValue->addListener(this);

    // RAM
    _installRamField.setText("RAM to share, MB", dontSendNotification);
    _installRamField.setColour(Label::textColourId, Colours::white);
    _installRamField.setColour(TextEditor::backgroundColourId, Colour(230,230,230));

    //_installTmpField.setBounds(0, hoff + 180, 500, 440);
    _installRamField.setBounds(0, offCores + 35, 500, 30);
    _installRamField.setFont(fieldFont);
    _installRamField.setJustificationType(Justification::top);

    _installRamValue = new TextEditor();
    _installRamValue->setText("2048");
    _installRamValue->setBounds(offLeft, offCores + 30, 200, 25);
    _installRamValue->setComponentID("Ram");
    //_installRamValue->setInputRestrictions(maxTextLength, allowedCharacters);
    _installRamValue->addListener(this);


    addAndMakeVisible(_installTypeField);
    addAndMakeVisible(_installProd);
    addAndMakeVisible(_installMaster);
    addAndMakeVisible(_installDev);
    addAndMakeVisible(_installConfField);
    //addAndMakeVisible(_installVm);
    addAndMakeVisible(_installTray);
    addAndMakeVisible(_installPathField);
    addAndMakeVisible(_installPathValue);
    addAndMakeVisible(_installTmpField);
    addAndMakeVisible(_installTmpValue);
    addAndMakeVisible(_installCoresField);
    addAndMakeVisible(_installCoresValue);
    addAndMakeVisible(_installRamField);
    addAndMakeVisible(_installRamValue);
    
}

LibrarySystemConfigure::~LibrarySystemConfigure() {
    delete(_installTray);
    //delete(_installVm);
    for (auto it = _installTypes.begin(); it != _installTypes.end(); it++) {
        delete (*it);
    }
}

void LibrarySystemConfigure::paint(Graphics& g) {
    g.fillAll (Colour (0xff222222));
    //g.setFont (Font (16.0f));
    g.setFont (Font (20.0f));
    //g.setColour (Colours::white);
    g.setColour(Colour(200,200,200));
}

void LibrarySystemConfigure::buttonClicked(Button *button) {

    if (button->getToggleState()){
	button->setColour(TextButton::buttonColourId, Colour(7,141,208));
	button->setColour(ToggleButton::textColourId, Colour(7,141,208));
	setConfigVersion(button);

    } else {
	button->setColour(TextButton::buttonColourId, Colour(200,200,200));
	button->setColour(ToggleButton::textColourId, Colour(200,200,200));
    }
}

void LibrarySystemConfigure::setConfigVersion(Button *button) {

    auto l = SubutaiLauncher::Log::instance()->logger();
    juce::String btext = button->getButtonText();

    l->debug() << "Library SystemConfigure setConfigVersion" << btext << std::endl;


    if (btext == "RH only")
    {
	instData.instConfig = RH_ONLY;
    }  else if (btext == "RH + Client"){
	instData.instConfig = RH_CLIENT;
    } else if (btext == "MH: RH+MH+Client"){
    	instData.instConfig = MH_FULL;
    } else if (btext == "Subutai Client only"){
    	instData.instConfig = CLIENT_ONLY;
    } else if (btext == "Install latest stable release"){
	instData.instVersion = PROD;
    } else if (btext == "stage: Install stage snapshot"){
	instData.instVersion = STAGE;
    } else if (btext == "dev: Install current development snapshot"){
	instData.instVersion = DEV;
    }

    l->debug() << "Library SystemConfigure setConfigVersion instConfig=" << instData.instConfig << " instVersion=" << instData.instVersion << std::endl;

}


void LibrarySystemConfigure::textEditorTextChanged(TextEditor &teditor) {
/*
    NativeMessageBox::showMessageBox(AlertWindow::AlertIconType::InfoIcon, "LibrarySystemConfigure::textTextChanged", 
	teditor.getText(), nullptr);

    teditor.setColour(TextEditor::backgroundColourId, Colours::yellow);

    juce::String tname = teditor.getName();
*/
}


void LibrarySystemConfigure::textEditorFocusLost(TextEditor &teditor)  {

//    NativeMessageBox::showMessageBox(AlertWindow::AlertIconType::InfoIcon, "LibrarySystemConfigure::textEditorFocusLost", 
//	teditor.getText(), nullptr);


/*    
int instConfig = MH_FULL;
    int instVersion = DEV;
    std::string instDir = "/opt/subutai";
    std::string instTmpDir = "/tmp/subutai";
    long instCores = 2;
    long instRAM = 2048;

} instData;
*/

}

void LibrarySystemConfigure::textEditorReturnKeyPressed(TextEditor &teditor) {

    teditor.setColour(TextEditor::backgroundColourId, Colours::green);
    
    std::string s1, s2;
    juce::String jname = teditor.getComponentID();

    if (jname.compare("Path") == 0)
	instData.instDir = teditor.getText().toStdString();
    else if (jname.compare("Tmp") == 0)
	 instData.instTmpDir = teditor.getText().toStdString();

    else if (jname.compare("Cores") == 0)
	s1 = (std::string)teditor.getText().toStdString();
    else if (jname.compare("Ram") == 0)
	s2 =  (std::string)teditor.getText().toStdString();


    NativeMessageBox::showMessageBox(AlertWindow::AlertIconType::InfoIcon, "LibrarySystemConfigure::textEditorRturnKeyPressed", 
	teditor.getText(), nullptr);

}


void LibrarySystemConfigure::fillInstallList() {

    auto l = SubutaiLauncher::Log::instance()->logger();
    l->debug() << "LibrarySystemConfigure::setInstallList "  << std::endl;

    map2Install.clear();
    switch (instData.instConfig) {
	case RH_ONLY:
		    l->debug() << "LibrarySystemConfigure::setInstallList " << instData.instConfig  << std::endl;
	            map2Install["VBox"] = "Oracle VirtualBox";
    		    map2Install["Peer"] = "Core16)";
		    break;

	case RH_CLIENT:
		    l->debug() << "LibrarySystemConfigure::setInstallList " << instData.instConfig  << std::endl;
	            map2Install["VBox"] = "Oracle VirtualBox";
    		    map2Install["Peer"] = "Core16";
	            map2Install["Subutai Client"] = "Tray, P2P";
	            map2Install["Browser Plugin"] = "Chrome, E2E";
		    break;
	case MH_FULL:
		    l->debug() << "LibrarySystemConfigure::setInstallList " << instData.instConfig  << std::endl;
	            map2Install["VBox"] = "Oracle VirtualBox";
    		    map2Install["Peer"] = "Core16";
		    map2Install["MH"] = "Import management template";
	            map2Install["Subutai Client"] = "Tray, P2P";
	            map2Install["Browser Plugin"] = "Chrome, E2E";
		    break;
	case CLIENT_ONLY:
		    l->debug() << "LibrarySystemConfigure::setInstallList " << instData.instConfig  << std::endl;
	            map2Install["Subutai Client"] = "Tray, P2P";
	            map2Install["Browser Plugin"] = "Chrome, E2E";
		    break;
	default: 
		    l->debug() << "LibrarySystemConfigure::setInstallList default"  << std::endl;
	            map2Install["VBox"] = "Oracle VirtualBox";
    		    map2Install["Peer"] = "Core16";
		    map2Install["MH"] = "Import management template";
	            map2Install["Subutai Client"] = "Tray, P2P";
	            map2Install["Browser Plugin"] = "Chrome, E2E";
		    break;

    }

//    instData.instDir = _installPathValue->getText();
//    l->debug() << "LibrarySystemConfigure::setInstallList instDir: " << instData.instDir  << std::endl;

}

void LibrarySystemConfigure::resized() {

//    NativeMessageBox::showMessageBox(AlertWindow::AlertIconType::InfoIcon, "LibrarySystemConfigure::resized()", 
//	    "resized", nullptr);

    auto l = SubutaiLauncher::Log::instance()->logger();
    l->debug() << "LibrarySystemConfigure::resized start  "  << std::endl;
    fillInstallList();
    //instData.instDir = _installPathValue->getText();
    //instData.instTmpDir =  _installTmpValue->getText();
    //instData.instCores =  _installCoresValue->getText();
    //instData.instRAM =  _installRamValue->getText();


}

bool LibrarySystemConfigure::isPeerInstallChecked() {
    //return _installVm->getToggleState();
}

// ============================================================================

LibraryDownload::LibraryDownload(bool downloadPeer) {

    auto l = SubutaiLauncher::Log::instance()->logger();
    l->debug() << "LibraryDownload start" << std::endl;

    _withPeer = downloadPeer;
    _progressBar = new Slider();
    addAndMakeVisible(_progressBar);
    _progressBar->setRange(0.0, 100.0);
    l->debug() << "LibraryDownload progressbar created" << std::endl;

    _progressBar->setValue(0.0);
    _progressBar->setSliderStyle(Slider::LinearBar);
    _progressBar->setBounds(0, 0, 1024-250-40, 20);
    _progressBar->setColour(Slider::backgroundColourId, Colour(100,100,100));
    _progressBar->setColour(Slider::trackColourId, Colour(7,141,208));
    _progressBar->setColour(Slider::thumbColourId, Colour(7,141,208));
    addAndMakeVisible(_progressBar);

    l->debug() << "LibraryDownload progressbar created 2" << std::endl;

    _currentAction.setText("Calculating download size", dontSendNotification);
    _currentAction.setColour(Label::textColourId, Colours::white);
    _currentAction.setBounds(0, 40, 500, 30);
    _currentAction.setJustificationType(Justification::left);
    addAndMakeVisible(_currentAction);
    l->debug() << "LibraryDownload progressbar created 3" << std::endl;

    _sizeProgress.setText("", dontSendNotification);
    _sizeProgress.setColour(Label::textColourId, Colours::white);
    _sizeProgress.setBounds(500, 40, 1024-250-40-500, 30);
    _sizeProgress.setJustificationType(Justification::right);
    addAndMakeVisible(_sizeProgress);

    l->debug() << "LibraryDownload all elements" << std::endl;


    _downloader = SubutaiLauncher::Session::instance()->getDownloader();
    _inProgress = false;
    _isDownloadComplete = false;
    _isCanceled = false;

    _fileList.push_back("hello_install.py");
    _fileList.push_back("repomd5");
//    _fileList.push_back("SubutaiTray");
//    _fileList.push_back("SubutaiTray_libs.tar.gz");
//    _fileList.push_back("setup-tray.py");
//    if (_withPeer) {
//        _fileList.push_back("snappy.ova");
//        _fileList.push_back("setup-peer.py");
//    }
}

LibraryDownload::~LibraryDownload() {
    delete(_progressBar);
}

void LibraryDownload::paint(Graphics& g) {
    g.fillAll (Colour (0xff222222));
    //g.setFont (Font (16.0f));
    g.setFont (Font (20.0f));
    //g.setColour (Colours::white);
    g.setColour(Colour(200,200,200));
    //_progressBar->setColour(Slider::backgroundColourId, Colour(100,100,100));
    //_progressBar->setColour(Slider::foregroundColourId, Colour(7,141,208));

}

void LibraryDownload::start() {
    _isCanceled = false;
    auto d = download();
    d.detach();
}

std::thread LibraryDownload::download() {
    return std::thread( [=] { downloadImpl(); } );
}

void LibraryDownload::downloadImpl() {
    if (!_inProgress) {
        _inProgress = true;
    } else {
        return;
    }
    auto total = calculateTotalSize();
    std::printf("Total download size: %lu\n", total);

    _progress = 0;
    _isDownloadComplete = false;
    for (auto it = _fileList.begin(); it != _fileList.end(); it++) {
        _currentAction.setText((*it), dontSendNotification);
        _downloader->setFilename((*it));
        auto t = _downloader->download();
        t.detach();
        while (!_downloader->isDone()) {
            updateProgress(_downloader->currentProgress());
#if LAUNCHER_LINUX
            usleep(1000);
#elif LAUNCHER_WINDOWS
	    Sleep(1000);
#elif LAUNCHER_MACOS
#error Not Implemented for this platform
#else
#error Unknown Platform
#endif
        }
        _progress += _downloader->currentProgress();
        _downloader->reset();
    }
    _currentAction.setText("Finalizing...", dontSendNotification);
    _sizeProgress.setText("Done", dontSendNotification);
    _progressBar->setValue(100.0);
    _inProgress = false;
    _isDownloadComplete = true;
}

void LibraryDownload::updateProgress(long p) {
    if (_progress + p > _totalSize) {
        _progressBar->setValue(100.0);
        return;
    }
    char val[500];
    std::sprintf(val, "%ld / %ld", _progress + p, _totalSize);
    _sizeProgress.setText(std::string(val), dontSendNotification);
    _progressBar->setValue((float)((_progress + p)/(_totalSize/100)));
    _progressBar->setVisible(true);
    resized();
}

void LibraryDownload::resized() {
    _currentAction.setBounds(0, 40, 500, 30);
    _sizeProgress.setBounds(500, 40, 1024-250-40-500, 30);
}

long LibraryDownload::calculateTotalSize() {
    _totalSize = 0;
    for (auto it = _fileList.begin(); it != _fileList.end(); it++) {
        _downloader->reset();
        _downloader->setFilename((*it));
        auto s = _downloader->info().size;
        _totalSize += s;
    }
    _downloader->reset();
    return _totalSize;
}

bool LibraryDownload::isComplete() {
    return _isDownloadComplete;
}

bool LibraryDownload::isCanceled() {
    return _isCanceled;
}

void LibraryDownload::setWithPeer(bool withPeer) {
    _withPeer = withPeer;
}

// ============================================================================

LibraryPreinstall::LibraryPreinstall() {

    auto l = SubutaiLauncher::Log::instance()->logger();
    l->debug() << "LibraryDownload start" << std::endl;
    _lstep.setText("Preinstall", dontSendNotification);
    //_withPeer = downloadPeer;
    _progressBar = new Slider();
    addAndMakeVisible(_progressBar);
    _progressBar->setRange(0.0, 0.9);
    l->debug() << "LibraryDownload progressbar created" << std::endl;

    _progressBar->setValue(0.0);
    _progressBar->setSliderStyle(Slider::LinearBar);
    _progressBar->setBounds(0, 0, 1024-250-40, 20);
    _progressBar->setColour(Slider::backgroundColourId, Colour(100,100,100));
    _progressBar->setColour(Slider::trackColourId, Colour(7,141,208));
    _progressBar->setColour(Slider::thumbColourId, Colour(7,141,208));
    addAndMakeVisible(_progressBar);

    l->debug() << "LibraryPreinstall progressbar created 2" << std::endl;


    _currentAction.setText("Calculating preinstall components", dontSendNotification);
    _currentAction.setColour(Label::textColourId, Colours::white);
    _currentAction.setBounds(0, 40, 500, 30);
    _currentAction.setJustificationType(Justification::left);
    addAndMakeVisible(_currentAction);
    l->debug() << "LibraryPreinstall progressbar created 3" << std::endl;

    _sizeProgress.setText("", dontSendNotification);
    _sizeProgress.setColour(Label::textColourId, Colours::white);
    _sizeProgress.setBounds(500, 40, 1024-250-40-500, 30);
    _sizeProgress.setJustificationType(Justification::right);
    addAndMakeVisible(_sizeProgress);

    l->debug() << "LibraryPreinstall all elements" << std::endl;


    //_preinstaller = SubutaiLauncher::Session::instance()->getPreinstaller();
    //_inProgress = false;
    //_isPreinstallComplete = false;
    //_isCanceled = false;


    //_componentList.push_back("P2P");
    //_componentList.push_back("Tray");


}

LibraryPreinstall::~LibraryPreinstall() {
    //delete(_progressBar);
}

void LibraryPreinstall::paint(Graphics& g) {
    g.fillAll (Colour (0xff222222));
    //g.setFont (Font (16.0f));
    g.setFont (Font (20.0f));
    //g.setColour (Colours::white);
    g.setColour(Colour(200,200,200));
}

/*
void LibraryPreinstall::start() {
    _isCanceled = false;
    auto d = preinstall();
    d.detach();
}

std::thread LibraryPreinstall::preinstall() {
    return std::thread( [=] { preinstallImpl(); } );
}

void LibraryPreinstall::preinstallImpl() {

    auto t = new LibraryActionThread("install", _componentname, _componentname);
    SubutaiLauncher::Log::instance()->logger()->debug() << "LibraryPreinstall::preinstallImpl() before run " <<std::endl;
    _currentAction.setText("Installing components", dontSendNotification);
    //t->launchThread();
    if (!t->runThread()){
	SubutaiLauncher::Log::instance()->logger()->debug() << "LibraryComponent::preinstallImpl() cancelled " <<std::endl;
	return;
    }


    if (!_inProgress) {
        _inProgress = true;
    } else {
        return;
    }
    auto total = calculateTotalSize();
    std::printf("Need to install : %d components\n ", total);

    _progress = 0;
    _isPreinstallComplete = false;
    for (auto it = _componentList.begin(); it != _componentList.end(); it++) {
        _currentAction.setText((*it), dontSendNotification);
        //_preinstaller->setFilename((*it));
        //auto t = _preinstaller->preinstall();
        //t.detach();
        //while (!_preinstaller->isDone()) {
        //    updateProgress(_preinstaller->currentProgress());
#if LAUNCHER_LINUX
            //usleep(1000);
#elif LAUNCHER_WINDOWS
	    //Sleep(1000);
#elif LAUNCHER_MACOS
#error Not Implemented for this platform
#else
#error Unknown Platform
#endif
        //}
        //_progress += _preinstaller->currentProgress();
        //_preinstaller->reset();
    }

    _currentAction.setText("Finalizing...", dontSendNotification);
    _sizeProgress.setText("Done", dontSendNotification);
    _progressBar->setValue(100.0);
    _inProgress = false;
    _isPreinstallComplete = true;
}


void LibraryPreinstall::updateProgress() {
    if (_progress + 1 > _totalSize) {
        _progressBar->setValue(100.0);
        return;
    }
    char val[500];
    std::sprintf(val, "%d / %d", _progress + p, _totalSize);
    _sizeProgress.setText(std::string(val), dontSendNotification);
    _progressBar->setValue((float)((_progress + p)/(_totalSize/100)));
    _progressBar->setVisible(true);
    resized();
}


void LibraryPreinstall::resized() {
    _currentAction.setBounds(0, 40, 500, 30);
    _sizeProgress.setBounds(500, 40, 1024-250-40-500, 30);
}

int LibraryPreinstall::calculateTotalSize() {
    _totalSize = 0;
    //_preinstaller->reset();
    return _componentList.size();
}

bool LibraryPreinstall::isComplete() {
    return _isPreinstallComplete;
}

bool LibraryPreinstall::isCanceled() {
    return _isPreinstallCanceled;
}

*/
//void LibraryPreinstall::setWithPeer(bool withPeer) {
//    _withPeer = withPeer;
//}
// ======================================================================

LibraryInstall::LibraryInstall() :  _numLines(2),  _confirmButton("Confirm")
{
    auto l = SubutaiLauncher::Log::instance()->logger();
    auto fieldFont = Font(24);

    //l->debug() << "LibraryInstall constructor: map2Install.size() " << map2Install.size() << std::endl;
    //for (auto it = map2Install.begin(); it != map2Install.end(); it++) {
//	//l->debug() << "LibrarySystemConfigure::setInstallList element: " << it->first  << std::endl;
//	addLine(it->first, it->second, "Need to install", true);
//    }

    l->debug() << "LibraryInstall start" << std::endl;

    _lstep.setText("Components to be Installed: ", dontSendNotification);
    _lstep.setColour(Label::textColourId, Colours::white);
    _lstep.setBounds(0, 10, 1024-250-40-500, 30);
    _lstep.setJustificationType(Justification::left);
    _lstep.setFont(fieldFont);
    addAndMakeVisible(_lstep);

    //TextButton* _confirmButton = new TextButton("Confirm");
    _confirmButton.addListener(this);
    _confirmButton.setBounds(600, 20, 150, 35);
    _confirmButton.setColour(TextButton::buttonColourId, Colour(7,141,208));
    //drawProgressButtons(false, false, false);
    addAndMakeVisible(_confirmButton);
    //_confirmButton.setVisible(true);


    l->debug() << "LibraryInstall all elements" << std::endl;
}

LibraryInstall::~LibraryInstall() {
    //delete(_progressBar);
}

void LibraryInstall::paint(Graphics& g) {

    g.fillAll (Colour (0xff222222));
    //g.setFont (Font (16.0f));
    g.setFont (Font (20.0f));
    //g.setColour (Colours::white);
    g.setColour(Colour(200,200,200));

    //showChangedInfo();

}

void LibraryInstall::addLine(std::string fieldText, std::string valueText, std::string hintText, bool inst) {
    auto font = Font(18);
    auto fieldFont = Font(20);
    int offLeft = 350;

    auto field = new Label();
    field->setText(fieldText, dontSendNotification);
    field->setColour(Label::textColourId, Colours::white);
    field->setBounds(0, _numLines * 50, 300, 50);
    field->setFont(fieldFont);
    field->setJustificationType(Justification::top);

    auto value = new Label();
    value->setText(valueText, dontSendNotification);
    field->setColour(Label::textColourId, Colours::white);
    value->setBounds(offLeft, _numLines * 50, 800, 50);
    value->setFont(font);
    value->setJustificationType(Justification::top);

    auto hint = new Label();

    if (inst) { 
	value->setColour(Label::textColourId, Colours::white);
	hint->setText(hintText, dontSendNotification);
    }  else {
	value->setColour(Label::textColourId, Colours::green);
	hint->setText(hintText, dontSendNotification);
    }

    hint->setColour(Label::textColourId, Colours::grey);
    //hint->setBounds(320, _numLines * 50 + 20, 800, 50);
    hint->setBounds(offLeft, _numLines * 50 + 20, 800, 50);
    hint->setFont(font);
    hint->setJustificationType(Justification::top);

    addAndMakeVisible((*field));
    addAndMakeVisible((*value));
    addAndMakeVisible((*hint));

    _numLines++;
}


void LibraryInstall::installComponents() {
    
    auto l =  SubutaiLauncher::Log::instance()->logger();
    for(auto it = vComponents.begin(); it != vComponents.end(); it++) {
	l->debug() << "LibraryInstall checking: " << (*it)  << std::endl;
	l->debug() << "LibraryInstall map2Install[" << (*it) << "]: " << map2Install[(*it)] << "/"  << std::endl;
	l->debug() << "LibraryInstall mapInstalled[" << (*it) << "]:: " << mapInstalled[(*it)] << "/" << std::endl;
	if (map2Install.find((*it)) != map2Install.end() && 
		(mapInstalled.find((*it)) == mapInstalled.end() || mapInstalled[(*it)] == "")){
	    std::string cName = (*it);
	    auto t = new LibraryActionThread("install", cName, cName);
	    l->debug() << "LibraryInstall::installComponents() before run: " << cName <<std::endl;
	    //_currentAction.setText("Installing components:", dontSendNotification);
	    //t->launchThread();
	    if (!t->runThread()){
	        l->debug() << "LibraryComponent::InstallComponents() cancelled " <<std::endl;
	    }//run and wait thread, log if no
	    if ((*it) == "Peer") {
		installPeer();
	    }
	resized();
	}
    }
}



void LibraryInstall::installPeer() {
    SubutaiLauncher::VirtualBox vbox;
    vbox.cloneVM();
    std::string sarg = "";

    switch (instData.instConfig)	{
	case 0:	sarg = "PROD";
		break;
	case 1:	sarg = "STAGE";
		break;
	case 2:	sarg = "DEV";
		break;    
	default: sarg = "DEV";
		break;    
    }

    NativeMessageBox::showMessageBox(AlertWindow::AlertIconType::InfoIcon, "LibraryInstall::InstallPeer", 
	sarg, nullptr);
    
    //Need to run this in thread with progress
    #std::thread pi = ([=] {execute vbox.runScripts(sarg)}); 

    vbox.runScripts(sarg);



    if (instData.instConfig == MH_FULL){
	vbox.importManagement();
    }
}

void LibraryInstall::buttonClicked(Button* button) {
    if (button == &_confirmButton) {
        std::printf("Confirm button\n");
        installComponents();
        //_step = SYSTEM_CHECK;
        /*  
            _confirmButton.setEnabled(false);
            auto dialog = new InstallationDialog("Subutai Installation", Colours::blue, DocumentWindow::allButtons);
            dialog->setBounds(0, 0, 800, 640);
            dialog->centreWithSize(dialog->getWidth(), dialog->getHeight());
            dialog->setTitleBarHeight(0);
            dialog->setVisible(true);
            */
    }
    //resized();
}

void LibraryInstall::componentVisibilityChanged(Component &component){

    NativeMessageBox::showMessageBox(AlertWindow::AlertIconType::InfoIcon, "LibraryInstall::componentVisibilityChanged", 
	mapInstallConfig[instData.instConfig], nullptr);
}

void LibraryInstall::showChangedInfo() {

    auto l = SubutaiLauncher::Log::instance()->logger();
    for(auto it = map2Install.begin(); it != map2Install.end(); it++) {
	l->debug() << "LibraryInstall::showChangedInfo element: " << it->first  << std::endl;
	bool needInstall = true;
	std::string hintText;
	if (mapInstalled.find(it->first) != mapInstalled.end() && mapInstalled[it->first] != ""){
	    l->debug() << "LibraryInstall showChangedInfo: mapInstall[it->first] " << mapInstalled[it->first] << std::endl;
	    needInstall = false;
	    hintText = "Already installed: " + mapInstalled[it->first];
	} else {
	    needInstall = true;
	    hintText = "Will be installed";
	}
	addLine(it->first, it->second, hintText, needInstall);
    }
    _numLines++;

    std::string tmp = mapInstallConfig[instData.instConfig];
    NativeMessageBox::showMessageBox(AlertWindow::AlertIconType::InfoIcon, "LibrarySystemConfigure::changedInfo()", 
	    tmp, nullptr);
    addLine("Configuration", tmp,  "", false);

    tmp = mapInstallVersion[instData.instVersion];
    NativeMessageBox::showMessageBox(AlertWindow::AlertIconType::InfoIcon, "LibrarySystemConfigure::changedInfo()", 
	    tmp, nullptr);

    addLine("Version", tmp, "", false);
    addLine("Inst Dir", instData.instDir, "", false);
    addLine("Tmp Dir",  instData.instTmpDir, "", false);
    //addLine("Cores", std::to_string(instData.instCores, "", false);
    //addLine("RAM, MB", std::to_string(instData.instRAM), "", false);

}


void LibraryInstall::resized() {

    auto l = SubutaiLauncher::Log::instance()->logger();
    l->debug() << "LibraryInstall::resized start  "  << std::endl;
    l->debug() << "LibraryInstall resized: map2Install.size() " << map2Install.size() << std::endl;

    //typedef std::map<std::string, std::string>::iterator it_type;
/*    
    for(auto it = map2Install.begin(); it != map2Install.end(); it++) {
	l->debug() << "LibraryInstall::resized element: " << it->first  << std::endl;
	bool needInstall = true;
	std::string hintText;
	if (mapInstalled.find(it->first) != mapInstalled.end() && mapInstalled[it->first] != ""){
	    l->debug() << "LibraryInstall resized: mapInstall[it->first] " << mapInstalled[it->first] << std::endl;
	    needInstall = false;
	    hintText = "Already installed: " + mapInstalled[it->first];
	} else {
	    needInstall = true;
	    hintText = "Will be installed";
	}
	addLine(it->first, it->second, hintText, needInstall);
    }
    _numLines++;
*/


    showChangedInfo();
/*
    std::string tmp = mapInstallConfig[instData.instConfig];
    addLine("Configuration", tmp,  "", false);
    tmp = mapInstallVersion[instData.instVersion];
    addLine("Version", tmp, "", false);
    addLine("Inst Dir", instData.instDir, "", false);
    addLine("Tmp Dir",  instData.instTmpDir, "", false);
    //addLine("Cores", std::to_string(instData.instCores, "", false);
    //addLine("RAM, MB", std::to_string(instData.instRAM), "", false);
*/
}
