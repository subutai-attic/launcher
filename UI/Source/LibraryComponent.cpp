/*
   ==============================================================================

LibraryComponent.cpp
Created: 5 Aug 2016 5:58:23pm
Author:  crioto

==============================================================================
*/

#include "LibraryComponent.h"

LibraryComponent::LibraryComponent() : 
    _installButton("Install..."),
    _nextButton("Next"),
    _backButton("Back"),
    _cancelButton("Cancel"),
    _step(INTRO)
{
    _installButton.addListener(this);
    _nextButton.addListener(this);
    _backButton.addListener(this);
    _cancelButton.addListener(this);
    _systemCheck = new LibrarySystemCheck();
    _systemConfigure = new LibrarySystemConfigure();
    _download = new LibraryDownload();
    addAndMakeVisible(_systemCheck);
    addAndMakeVisible(_systemConfigure);
    addAndMakeVisible(_download);
    addChildComponent(_nextButton);
    addChildComponent(_backButton);
    addChildComponent(_cancelButton);
}

LibraryComponent::~LibraryComponent() {
    delete(_systemCheck);
    delete(_systemConfigure);
    delete(_download);
}

void LibraryComponent::paint(Graphics& g) {
    g.fillAll (Colour (0xff222222));
    g.setFont (Font (16.0f));
    g.setColour (Colours::white);
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
    };
}

void LibraryComponent::buttonClicked(Button* button) {
    if (button == &_installButton) {
        std::printf("Install button\n");
        _step = SYSTEM_CHECK;
        /*  
            _installButton.setEnabled(false);
            auto dialog = new InstallationDialog("Subutai Installation", Colours::grey, DocumentWindow::allButtons);
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

void LibraryComponent::drawIntro() {
    addAndMakeVisible(_installButton);
    _installButton.setVisible(true);
    _installButton.setBounds(600, 20, 150, 35);
    _nextButton.setVisible(false);
    _backButton.setVisible(false);
    _cancelButton.setVisible(false);
    //sleep(2);
    //exit(10);
}

void LibraryComponent::drawSystemCheck() {
    _systemCheck->setVisible(true);
    _systemCheck->setBounds(20, 20, 1024-250-40, 768 - 60 - 40);
    drawProgressButtons(true, false, true);
}

void LibraryComponent::drawSystemConfigure() {
    _systemConfigure->setVisible(true);
    _systemConfigure->setBounds(20, 20, 1024-250-40, 768 - 60 - 40);
    drawProgressButtons(true, true, true);
}


void LibraryComponent::drawDownload() {
#if LAUNCHER_LINUX
    const std::string& file("launcher-linux-install.py");
#elif LAUNCHER_WINDOWS
#error Not Implemented for this platform
#elif LAUNCHER_MACOS
#error Not Implemented for this platform
#else
#error Unknown Platform
#endif
    _download->setWithPeer(_systemConfigure->isPeerInstallChecked());
    _download->setVisible(true);
    _download->setBounds(20, 20, 1024-250-40, 768 - 60 - 40);
    _download->start();
    drawProgressButtons(false, false, true);
    auto t = waitDownloadComplete();
    t.detach();
}

void LibraryComponent::drawPreinstall() {

}

void LibraryComponent::drawInstall() {

}

void LibraryComponent::drawPostInstall() {

}

void LibraryComponent::drawFinished() {

}

void LibraryComponent::onStepChange() {
    _installButton.setVisible(false);
    _systemCheck->setVisible(false);
    _systemConfigure->setVisible(false);
    _download->setVisible(false);
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
    }
    if (back) {
        _backButton.setBounds(540, 700, 100, 30);
        _backButton.setEnabled(true);
    } else {
        _backButton.setEnabled(false);
    }
    if (cancel) {
        _cancelButton.setBounds(420, 700, 100, 30);
        _cancelButton.setEnabled(true);
    } else {
        _cancelButton.setEnabled(false);
    }
}

void LibraryComponent::nextStep() {
    switch (_step) {
        case SYSTEM_CHECK:
            _step = SYSTEM_CONFIGURE;
            break;
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
    }; 
}

void LibraryComponent::previousStep() {
    switch (_step) {
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
            break;
        case SYSTEM_CONFIGURE:
            _step = SYSTEM_CHECK;
            break;
    };
}

std::thread LibraryComponent::waitDownloadComplete() {
    return std::thread( [=] { waitDownloadCompleteImpl(); } );
}

void LibraryComponent::waitDownloadCompleteImpl() {
    while (!_download->isComplete()) {
        sleep(1);
        if (_download->isCanceled()) {
            return;
        }
    }
    drawProgressButtons(true, false, true);
}

// ============================================================================

LibrarySystemCheck::LibrarySystemCheck() : _numLines(1) 
{
    //Notification note;
    addAndMakeVisible(_numCpuField);
    addAndMakeVisible(_numCpuValue);
    addAndMakeVisible(_numCpuHint);
    addLine(&_numCpuField, &_numCpuValue, &_numCpuHint, "Number of CPU Cores", "Each peer requires at least 2 CPU cores");

    addAndMakeVisible(_maxMemoryField);
    addAndMakeVisible(_maxMemoryValue);
    addAndMakeVisible(_maxMemoryHint);
    addLine(&_maxMemoryField, &_maxMemoryValue, &_maxMemoryHint, "Total System Memory", "4GB will be taken to each peer's virtual machine");

    addAndMakeVisible(_vboxField);
    addAndMakeVisible(_vboxValue);
    addAndMakeVisible(_vboxHint);
    addLine(&_vboxField, &_vboxValue, &_vboxHint, "Oracle VirtualBox version", "We're running our peer on VirtualBox VMs");

    addAndMakeVisible(_vtxField);
    addAndMakeVisible(_vtxValue);
    addAndMakeVisible(_vtxHint);
    addLine(&_vtxField, &_vtxValue, &_vtxHint, "Hardware Virtualization Support", "VTx should be supported by your CPU");

    addAndMakeVisible(_sshField);
    addAndMakeVisible(_sshValue);
    addAndMakeVisible(_sshHint);
    addLine(&_sshField, &_sshValue, &_sshHint, "SSH client version", "SSH client is used to configure peer during installation");
    
    /* 
    _info.setText("ing...", dontSendNotification);
    _info.setColour(Label::textColourId, Colours::white);
    _info.setBounds(00, 0, 800, 440);
    _info.setFont(font);
    _info.setJustificationType(Justification::top);
    */
}

LibrarySystemCheck::~LibrarySystemCheck() {

}

void LibrarySystemCheck::addLine(Label* field, Label* value, Label* hint, std::string text, std::string hintText) {
    auto font = Font(16);
    field->setText(text, dontSendNotification);
    field->setColour(Label::textColourId, Colours::white);
    field->setBounds(0, _numLines * 50, 300, 50);
    field->setFont(font);
    field->setJustificationType(Justification::top);

    value->setText("Checking...", dontSendNotification);
    value->setColour(Label::textColourId, Colours::white);
    value->setBounds(320, _numLines * 50, 800, 50);
    value->setFont(font);
    value->setJustificationType(Justification::top);

    hint->setText(hintText, dontSendNotification);
    hint->setColour(Label::textColourId, Colours::grey);
    hint->setBounds(320, _numLines * 50 + 20, 800, 50);
    hint->setFont(font);
    hint->setJustificationType(Justification::top);

    _numLines++;
}

void LibrarySystemCheck::paint(Graphics& g) {
    g.fillAll (Colour (0xff222222));
    g.setFont (Font (16.0f));
    g.setColour (Colours::white);
}

void LibrarySystemCheck::resized() {
}

// ============================================================================

LibrarySystemConfigure::LibrarySystemConfigure() {

    _installVmField.setText("Choose Installation Configuration", dontSendNotification);
    _installVmField.setColour(Label::textColourId, Colours::white);
    _installVmField.setBounds(0, 5, 500, 440);
    _installVmField.setJustificationType(Justification::top);

    auto conf = SubutaiLauncher::Session::instance()->getConfManager();

    auto configs = conf->getConfigs();
    int i = 0;
    for (auto it = configs.begin(); it != configs.end(); it++) {
        auto b = new ToggleButton((*it).title);
        b->setRadioGroupId(11);
        b->setBounds(300, i*30, 200, 30);
        b->setColour(ToggleButton::textColourId, Colours::white);
        if (i == 0) {
            b->triggerClick();
        }
        addAndMakeVisible(b);
        _installTypes.push_back(b);
        ++i;
    }

    /* 
    _installTray = new ToggleButton("Install only tray application");
    _installTray->setRadioGroupId(11);
    _installTray->setBounds(300, 0, 200, 30);
    _installTray->setColour(ToggleButton::textColourId, Colours::white);
    _installTray->triggerClick();
    _installVm = new ToggleButton("Install tray and setup a new peer");
    _installVm->setRadioGroupId(11);
    _installVm->setBounds(300, 30, 200, 30);
    _installVm->setColour(ToggleButton::textColourId, Colours::white);

    // Version

    _installTypeField.setText("Choose Version", dontSendNotification);
    _installTypeField.setColour(Label::textColourId, Colours::white);
    _installTypeField.setBounds(0, 65, 500, 440);
    _installTypeField.setJustificationType(Justification::top);



    _installMaster = new ToggleButton("Install latest stable release");
    _installMaster->setRadioGroupId(22);
    _installMaster->setBounds(300, 60, 400, 30);
    _installMaster->setColour(ToggleButton::textColourId, Colours::white);
    _installMaster->triggerClick();
    _installDev = new ToggleButton("Install current development snapshot");
    _installDev->setRadioGroupId(22);
    _installDev->setBounds(300, 90, 400, 30);
    _installDev->setColour(ToggleButton::textColourId, Colours::white);

    // Path

    _installPathField.setText("Installation path", dontSendNotification);
    _installPathField.setColour(Label::textColourId, Colours::white);
    _installPathField.setBounds(0, 140, 500, 440);
    _installPathField.setJustificationType(Justification::top);

    _installPathValue.setText("/opt/subutai");
    _installPathValue.setBounds(300, 140, 200, 25);

    // Tmp
    //
    _installTmpField.setText("Temporary directory", dontSendNotification);
    _installTmpField.setColour(Label::textColourId, Colours::white);
    _installTmpField.setBounds(0, 180, 500, 440);
    _installTmpField.setJustificationType(Justification::top);

    _installTmpValue.setText("/tmp");
    _installTmpValue.setBounds(300, 180, 200, 25);

    addAndMakeVisible(_installTypeField);
    addAndMakeVisible(_installMaster);
    addAndMakeVisible(_installDev);
    addAndMakeVisible(_installVmField);
    addAndMakeVisible(_installVm);
    addAndMakeVisible(_installTray);
    addAndMakeVisible(_installPathField);
    addAndMakeVisible(_installPathValue);
    addAndMakeVisible(_installTmpField);
    addAndMakeVisible(_installTmpValue);
    */
}

LibrarySystemConfigure::~LibrarySystemConfigure() {
    delete(_installTray);
    delete(_installVm);
    for (auto it = _installTypes.begin(); it != _installTypes.end(); it++) {
        delete (*it);
    }
}

void LibrarySystemConfigure::paint(Graphics& g) {
    g.fillAll (Colour (0xff222222));
    g.setFont (Font (16.0f));
    g.setColour (Colours::white);
}

void LibrarySystemConfigure::resized() {

}

bool LibrarySystemConfigure::isPeerInstallChecked() {
    return _installVm->getToggleState();
}

// ============================================================================

LibraryDownload::LibraryDownload(bool downloadPeer) {
    _withPeer = downloadPeer;
    _progressBar = new Slider();
    addAndMakeVisible(_progressBar);

    _progressBar->setRange(0.0, 100.0, 0.1);
    _progressBar->setValue(0.0);
    _progressBar->setSliderStyle(Slider::LinearBar);
    _progressBar->setBounds(0, 0, 1024-250-40, 40);

    _currentAction.setText("Calculating download size", dontSendNotification);
    _currentAction.setColour(Label::textColourId, Colours::white);
    _currentAction.setBounds(0, 40, 500, 30);
    _currentAction.setJustificationType(Justification::left);
    addAndMakeVisible(_currentAction);

    _sizeProgress.setText("", dontSendNotification);
    _sizeProgress.setColour(Label::textColourId, Colours::white);
    _sizeProgress.setBounds(500, 40, 1024-250-40-500, 30);
    _sizeProgress.setJustificationType(Justification::right);
    addAndMakeVisible(_sizeProgress);

    _downloader = SubutaiLauncher::Session::instance()->getDownloader();
    _inProgress = false;
    _isDownloadComplete = false;
    _isCanceled = false;

    _fileList.push_back("p2p");
    _fileList.push_back("SubutaiTray");
    _fileList.push_back("SubutaiTray_libs.tar.gz");
    _fileList.push_back("setup-tray.py");
    if (_withPeer) {
        _fileList.push_back("snappy.ova");
        _fileList.push_back("setup-peer.py");
    }
}

LibraryDownload::~LibraryDownload() {
    delete(_progressBar);
}

void LibraryDownload::paint(Graphics& g) {

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
#error Not Implemented for this platform
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
