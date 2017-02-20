#include "LibraryActionThread.h"

LibraryActionThread::LibraryActionThread(const std::string& process, const std::string& component, const std::string& title) :
    _process(process),
    _component(component),
    _title(title),
    _running(false),
    ThreadWithProgressWindow (title, true, true) //with progress bar and cancel button, timeout can be added
{
    //setStatusMessage ("Getting ready...");
    //setProgress (0.05);
    //run();

}

bool LibraryActionThread::isRunning()
{
    return _running;
}


void LibraryActionThread::run() 
{
    auto l = SubutaiLauncher::Log::instance()->logger();
    l->debug() << "LAT::run  process: " << _process << " process_no:" << process_no() << std::endl;
    //setProgress (-1.0);//  - indeterminate progress bar

    //auto conf = SubutaiLauncher::Session::instance()->getConfManager();

    _running = true;
    _error = false;
    setStatusMessage ("Getting ready...");
    setProgress (0.1);
    if (_component == "P2P")
    {
        SubutaiLauncher::P2P p2p;
        //p2p.findInstallation();
	//l->debug() << "LibraryAT::run  p2p is installed: " << std::to_string(p2p.isInstalled()) << std::endl;
        //if (p2p.isInstalled() && process_no() == 1) {
	if (p2p.findInstallation() && process_no() == 1) {
	    //l->debug() << "LAT::run  p2p installed already " << std::endl;
	    //Show alert and return
	    _error = true;
	    return;
        }
	//if (!p2p.isInstalled() && process_no() == 3) {
	if (!p2p.findInstallation() && process_no() == 3) {
	    //l->debug() << "LibraryAT::run p2p is uninstalled already " << std::endl;
	    //Show alert and return
	    _error = true;
	    return;
        }
    } 
    else if (_component == "Tray Client")
    {
        SubutaiLauncher::Tray tray;
        //tray.findInstallation();
        if (tray.findInstallation() && process_no() == 1) {
	    //l->debug() << "LibraryComponent::constructor tray version: " << tray.extractVersion() << std::endl;
	    _error = true;
	    return;
        }
	if (!tray.findInstallation() && process_no() == 3) {
	    //l->debug() << "LibraryComponent::constructor tray version: " << tray.extractVersion() << std::endl;
	    _error = true;
	    return;
        }
    }
    else if (_component == "Browser Plugin")
    {
	sleep(4000);
    }
    else if (_component == "VBox")
    {
        SubutaiLauncher::VirtualBox vbox;
        //vbox.findInstallation();
        if (vbox.findInstallation() && process_no() == 1) {
	    //l->debug() << "LibraryComponent::constructor tray version: " << vbox.extractVersion() << std::endl;
	    _error = true;
	    return;
        }
	if (!vbox.findInstallation() && process_no() == 3) {
	    //l->debug() << "LibraryComponent::constructor vbox version: " << vbox.extractVersion() << std::endl;
	    _error = true;
	    return;
        }
    }
    
    setProgress (0.2);
    //_running = true;
    //_error = false;
    
    setStatusMessage ("Download installation script");

    auto conf = SubutaiLauncher::Session::instance()->getConfManager();
    auto configs = conf->getConfigs();
    SubutaiLauncher::InstallConfig config;
    bool found = false;
    for (auto it = configs.begin(); it != configs.end(); it++)
    {
        if ((*it).title == _component)
        {
            config = (*it);
            found = true;
            break;
        }
    }

    if (!found)
    {
        l->error() << _component << " configuration was not found" << std::endl;
        _error = true;
        _running = false;
        return;
    }
    
    l->debug() << _component << " configuration found" << std::endl;

    auto d = SubutaiLauncher::Session::instance()->getDownloader();

    l->debug() << "LibraryActionTread::run getDownloader" << std::endl;

    auto file = std::string(config.file);
    size_t ind = 0;
    ind = file.find(_process);
    if (ind == std::string::npos){
	ind = file.find("install");
	file = file.replace(ind, 7, _process);
    }

    l->debug() << "LibraryActionTread::run config.file " << file << std::endl;
    std::string file_short = file;
    file.append(".py");

    d->reset();
    d->setFilename(file);

    l->debug() << "LibraryActionTread::run setFilename " << file << " file short: " << file_short << std::endl;

    auto dt = d->download();
    if (dt.joinable()){
	dt.detach();
    }
    l->debug() << "LibraryActionTread:: d->download detached" << std::endl;

    setProgress (-0.1);
    while (!d->isDone())
    {
	if (threadShouldExit()) {
            _running = false;
            return;
        }
    }

    l->debug() << "LAT::run  download completed" << std::endl;
    setProgress (0.6);
    
    auto nc = SubutaiLauncher::Session::instance()->getNotificationCenter();
    SubutaiLauncher::SL sl(d->getOutputDirectory());
//    sl.open(config.file);
    l->debug() << "LAT::run file_short:" << d->getOutputDirectory() << "/" << file_short  << std::endl;
    sl.open(file_short);
    auto t = sl.executeInThread();

    bool inProgress = true;
    while (inProgress)
    {
        auto event = nc->dispatch();
        switch (event)
        {
            case SubutaiLauncher::DOWNLOAD_STARTED:
                setStatusMessage("Downloading necessary files");
                setProgress(100 / (double)d->getPercent());
                break;
            case SubutaiLauncher::DOWNLOAD_FINISHED:
                setStatusMessage("Download complete");
		//l->debug() << "setStatusMessage (\"Download complete\") " << std::endl;
                break;
            case SubutaiLauncher::INSTALL_STARTED:
		setStatusMessage("Installing");
		//l->debug() << "setStatusMessage (\"Installing\") " << std::endl;
                break;
            case SubutaiLauncher::INSTALL_FINISHED:
                setStatusMessage("Installation finished");
		//l->debug() << "setStatusMessage (\"Installation finished\") " << std::endl;
		break;
            case SubutaiLauncher::SHOW_MESSAGE:
		setStatusMessage("Message?");
		//l->debug() << "setStatusMessage (\"Message?\") " << std::endl;
                break;
            case SubutaiLauncher::SCRIPT_FINISHED:
		setStatusMessage("Script finished");
		//l->debug() << "setStatusMessage (\"Script finished\") " << std::endl;
                inProgress = false;
                break;
            default:
		setStatusMessage("Running");
    		setProgress(100 / (double)d->getPercent());
		//l->debug() << "setStatusMessage (\"Default\") " << std::endl;
                break;
        };
    }

    setProgress (0.8);

    if (t.joinable()){
	t.join();
    }

    /*
    for (int i = 0; i < thingsToDo; ++i)
    {
        // must check this as often as possible, because this is
        // how we know if the user's pressed 'cancel'
        if (threadShouldExit())
            return;

        // this will update the progress bar on the dialog box
        setProgress (i / (double) thingsToDo);

        setStatusMessage (juce::String (thingsToDo - i) + " things left to do...");
	l->debug() << "setStatusMessage (juce::String (thingsToDo - i) + \" things left to do..\") " << std::endl;
        wait (1000);
    }
    */

    setProgress (0.9);
    setStatusMessage ("Finishing !");
    //wait (200);
    sleep(2000);

    setProgress (1.0);
    _running = false;

}

void LibraryActionThread::threadComplete (bool userPressedCancel)
{
    char m1[255];
    char m2[255];
    int n = 0;
    unsigned pn = process_no();

    auto l = SubutaiLauncher::Log::instance()-> logger();

    l->debug() << "LAT::threadComplete start error: " << _error << std::endl;
    if (_error)
    {
	strcpy(m1, "Process stopped\0");
	strcpy(m2, "Process was not completed!\0");
    } else if (userPressedCancel)
    {
	switch (pn){
	    case 1: 
		strcpy(m1, "Installation process stopped\0");
		strcpy(m2, "Component was not installed!\0");
		break;
	    case 2: 
		strcpy(m1, "Update process stopped\0");
		strcpy(m2, "Component was not updated!\0");
		break;
	    case 3: 
		strcpy(m1, "Uninstall process stopped\0");
		strcpy(m2, "Component was not uninstalled!\0");
		break;
	    default: 
		strcpy(m1, "Process stopped\0");
		strcpy(m2, "Unknown error!\0");
		break;    
	}        
    }
    else
    {
	l->debug() << "LAT::threadComplete normal " << std::endl;
	// thread finished normally..
	n = _process.find("uninstall", 0);
	switch (pn){
	    case 1: 
		strcpy(m1, "Installation has been completed\0");
		strcpy(m2, "Component is installed!\0");
		break;
	    case 2: 
		strcpy(m1, "Update process has been completed\0");
		strcpy(m2, "Component is updated!\0");
		break;
	    case 3: 
		strcpy(m1, "Uninstall has been completed\0");
		strcpy(m2, "Component is uninstalled!\0");
		break;
	    default: 
		strcpy(m1, "Process completed\0");
		strcpy(m2, "Process completed without errors!\0");
		break;    
        }
    }

    l->debug() << "LAT::threadComplete before alert " << m1 << " " << m2 << std::endl;
//    juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::WarningIcon,
//                m1, m2);

    juce::AlertWindow::showMessageBox(juce::AlertWindow::WarningIcon,
                m1, m2);

    // ..and clean up by deleting our thread object..
    delete this;
}

unsigned LibraryActionThread::process_no(){
    int n = _process.find("uninstall", 0);
    if (n !=std::string::npos){
	return 3;
    }
    n = _process.find("install", 0);
    if (n !=std::string::npos){
	return 1;
    } 
    n = _process.find("update", 0);
    if (n !=std::string::npos){
	return 2;
    }
    return 0;
}