#include "LibraryItem.h"

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
        if (p2p.findInstallation()) {
            v =  p2p.extractVersion();
        }
    } 
    else if (cname == "Tray Client")
    {
        SubutaiLauncher::Tray tray;
        tray.findInstallation();
        if (tray.findInstallation()) {
            v = tray.extractVersion();
        }
    }
    else if (cname == "Browser Plugin")
    {
        SubutaiLauncher::Browser brw;
        brw.findInstallation();
        l->debug() << "LibraryComponent::LibraryItem browser is installed: " << brw.findInstallation() << " is installed: " << brw.isInstalled() << std::endl;
        if (brw.isInstalled()) {
            v = brw.extractVersion();
            v += "\nE2E";
        }
    }
    else if (cname == "VBox")
    {
        SubutaiLauncher::VirtualBox vbox;
        vbox.findInstallation();

        if (vbox.findInstallation()) {
            v = vbox.extractVersion();
        }
    }
    else if (cname == "Subutai Client")
    {
        SubutaiLauncher::P2P p2p;
        p2p.findInstallation();
        if (p2p.findInstallation()) {
            v = " P2P: " +  p2p.extractVersion();
        }
        SubutaiLauncher::Tray tray;
        tray.findInstallation();
        if (tray.findInstallation()) {
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
        if (!t->runThread()){
        }
    }
    else if (res == 2)
    {
        std::string windowTitle = "Updatiing ";
        windowTitle.append(_title);
        auto t = new LibraryActionThread("update", _title, windowTitle);
        t->launchThread();
        while (t->isRunning()) {
            //sleep(1);
        }

    } 
    else if (res == 3)
    {
        std::string windowTitle = "Removing ";
        windowTitle.append(_title);
        auto t = new LibraryActionThread("uninstall", _title, windowTitle);
        if (!t->runThread()){
            SubutaiLauncher::Log::instance()->logger()->debug() << "LibraryComponent::LibraryItem::mouseUp cancelled " <<std::endl;
        }
    }

    drawVersion();
}
