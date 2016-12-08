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
    auto file = std::string(config.file);
    file.append(".py");
    d->reset();
    d->setFilename(file);
    auto dt = d->download();
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
    sl.open(config.file);
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
    if (_error)
    {
        juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::WarningIcon,
                "Error occured",
                "Unknown error occured during process");
    } else if (userPressedCancel)
    {
        juce::AlertWindow::showMessageBoxAsync (juce::AlertWindow::WarningIcon,
                "Installation process stopped",
                "Component was not installed!");
    }
    else
    {
        // thread finished normally..
        juce::AlertWindow::showMessageBoxAsync (juce::AlertWindow::WarningIcon,
                "Installation has been completed",
                "Component is now installed!");
    }

    // ..and clean up by deleting our thread object..
    delete this;
}

