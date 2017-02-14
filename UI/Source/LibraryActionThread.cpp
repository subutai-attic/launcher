#include "LibraryActionThread.h"

LibraryActionThread::LibraryActionThread(const std::string& process, const std::string& component, const std::string& title) :
    _process(process),
    _component(component),
    _title(title),
    _running(false),
    ThreadWithProgressWindow (title, true, true)
{
    setStatusMessage ("Getting ready...");

}

bool LibraryActionThread::isRunning()
{
    return _running;
}

void LibraryActionThread::run() 
{
    _running = true;
    _error = false;
    auto l = SubutaiLauncher::Log::instance()->logger();
    l->debug() << "Starting dialog thread" << std::endl;
    setProgress (-1.0); // setting a value beyond the range 0 -> 1 will show a spinning bar..
    //setProgress (0); // setting a value beyond the range 0 -> 1 will show a spinning bar..
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

    l->debug() << "LibraryActionTread::run reset " << file << std::endl;

    d->setFilename(file);

    l->debug() << "LibraryActionTread::run setFilename " << file << std::endl;

    auto dt = d->download();
    
    l->debug() << "LibraryActionTread::run d->download" << std::endl;

    dt.detach();
    while (!d->isDone())
    {
        if (threadShouldExit()) {
            _running = false;
            return;
        }
    }

    l->debug() << d->getOutputDirectory() << "/" << file << " download complete" << std::endl;

    auto nc = SubutaiLauncher::Session::instance()->getNotificationCenter();
    SubutaiLauncher::SL sl(d->getOutputDirectory());
//    sl.open(config.file);
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
                break;
            case SubutaiLauncher::INSTALL_STARTED:
                break;
            case SubutaiLauncher::INSTALL_FINISHED:
                break;
            case SubutaiLauncher::SHOW_MESSAGE:
                break;
            case SubutaiLauncher::SCRIPT_FINISHED:
                inProgress = false;
                break;
            default:
                break;
        };
    }

    t.join();
    _running = false;

    return;

    //wait (1000);

    const int thingsToDo = 10;

    for (int i = 0; i < thingsToDo; ++i)
    {
        // must check this as often as possible, because this is
        // how we know if the user's pressed 'cancel'
        if (threadShouldExit())
            return;

        // this will update the progress bar on the dialog box
        setProgress (i / (double) thingsToDo);

        setStatusMessage (juce::String (thingsToDo - i) + " things left to do...");

        wait (500);
    }

    setProgress (-1.0); // setting a value beyond the range 0 -> 1 will show a spinning bar..
    setStatusMessage ("Finishing off the last few bits and pieces!");
    wait (2000);
}

void LibraryActionThread::threadComplete (bool userPressedCancel)
{
    char m1[255];
    char m2[255];
    int n = 0;
    unsigned pn = process_no();

    auto l = SubutaiLauncher::Log::instance()-> logger();

    if (_error)
    {
	strcpy(m1, "Installation process stopped\0");
	strcpy(m2, "Component was not installed!\0");
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
    juce::AlertWindow::showMessageBoxAsync (juce::AlertWindow::WarningIcon,
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