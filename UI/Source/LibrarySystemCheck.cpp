#include "LibrarySystemCheck.h"

bool instEnabled = true;
long minCores = 2;
long minRAM = 3600;

std::map <std::string, std::string> mapInstalled;
std::map <std::string, std::string> map2Install;
std::string vboxVersion;
std::vector <std::string> vComponents = {"Browser Plugin", "VBox", "Subutai Client", "Peer"};
std::map <int, std::string> mapInstallConfig = { {RH_ONLY, "RH only"}, {RH_CLIENT, "RH+Client"}, {MH_FULL, "RH+MH+Client"}, {CLIENT_ONLY, "Client only"} };
std::map <int, std::string> mapInstallVersion = { {PROD,"Current release version"}, {STAGE, "Stage snapshot"}, {DEV, "Dev snapshot"} };


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
