#include "WizardFinish.h"
#include "WizardWindow.h"

WizardFinish::WizardFinish() :
    _finish("Finish"),
    _peerSucceed(false)
{
	
    _logger = &Poco::Logger::get("subutai");
    _logger->trace("Creating Wizard Finish UI Component");
    auto font = juce::Font("Encode Sans", 17, 0);

    _thanks.setText("Thank you for using Subutai", juce::dontSendNotification);
    _thanks.setColour(juce::Label::textColourId, juce::Colour(105, 116, 144));
    _thanks.setBounds(15, 15, 400, 40);
    _thanks.setFont(font);
    _thanks.setJustificationType(juce::Justification::top);
    addAndMakeVisible(_thanks);

	_ptp.setText("P2P Installation", juce::dontSendNotification);
	_ptp.setColour(juce::Label::textColourId, juce::Colour(105, 116, 144));
	_ptp.setBounds(15, 35, 150, 40);
	_ptp.setFont(font);
	_ptp.setJustificationType(juce::Justification::top);
	addAndMakeVisible(_ptp);

	_ptpResult.setText("Undefined", juce::dontSendNotification);
	_ptpResult.setColour(juce::Label::textColourId, juce::Colour(105, 116, 144));
	_ptpResult.setBounds(150, 35, 500, 40);
	_ptpResult.setFont(font);
	_ptpResult.setJustificationType(juce::Justification::top);
	addAndMakeVisible(_ptpResult);

	_tray.setText("Tray Installation", juce::dontSendNotification);
	_tray.setColour(juce::Label::textColourId, juce::Colour(105, 116, 144));
	_tray.setBounds(15, 55, 150, 40);
	_tray.setFont(font);
	_tray.setJustificationType(juce::Justification::top);
	addAndMakeVisible(_tray);

	_trayResult.setText("Undefined", juce::dontSendNotification);
	_trayResult.setColour(juce::Label::textColourId, juce::Colour(105, 116, 144));
	_trayResult.setBounds(150, 55, 500, 40);
	_trayResult.setFont(font);
	_trayResult.setJustificationType(juce::Justification::top);
	addAndMakeVisible(_trayResult);

	_ete.setText("E2E Installation", juce::dontSendNotification);
	_ete.setColour(juce::Label::textColourId, juce::Colour(105, 116, 144));
	_ete.setBounds(15, 75, 150, 40);
	_ete.setFont(font);
	_ete.setJustificationType(juce::Justification::top);
	addAndMakeVisible(_ete);

	_eteResult.setText("Undefined", juce::dontSendNotification);
	_eteResult.setColour(juce::Label::textColourId, juce::Colour(105, 116, 144));
	_eteResult.setBounds(150, 75, 500, 40);
	_eteResult.setFont(font);
	_eteResult.setJustificationType(juce::Justification::top);
	addAndMakeVisible(_eteResult);

	_peer.setText("Peer Installation", juce::dontSendNotification);
	_peer.setColour(juce::Label::textColourId, juce::Colour(105, 116, 144));
	_peer.setBounds(15, 95, 150, 40);
	_peer.setFont(font);
	_peer.setJustificationType(juce::Justification::top);
	addAndMakeVisible(_peer);

	_peerResult.setText("Undefined", juce::dontSendNotification);
	_peerResult.setColour(juce::Label::textColourId, juce::Colour(105, 116, 144));
	_peerResult.setBounds(150, 95, 500, 40);
	_peerResult.setFont(font);
	_peerResult.setJustificationType(juce::Justification::top);
	addAndMakeVisible(_peerResult);

    _finish.setBounds(400, 560, 86, 25);
    _finish.setColour(juce::TextButton::buttonColourId, juce::Colour(7,141,208));
    _finish.addListener(this);
    addAndMakeVisible(_finish);

    _peerLink.setBounds(-1, -1, 1, 1);
    _peerLink.setURL(URL("https://localhost:9999"));
    addAndMakeVisible(_peerLink);
    
    std::string pCrashBot(SubutaiLauncher::Session::instance()->getSettings()->getTmpPath() + "crashbot.png");
    Poco::File pLogo(pCrashBot);
    if (pLogo.exists())
    {
        _crashBot.setImage(juce::ImageCache::getFromFile(juce::File(pCrashBot)));
    }
    else
    {
        _logger->error("crashbot.png doesn't exists");
    }
    addChildComponent(_crashBot);
    _crashBot.setBounds(122, 200, 256, 167);

    auto font2 = juce::Font("Encode Sans", 21, 1);
	_crashWarning.setText("Something went wrong!", juce::dontSendNotification);
	_crashWarning.setColour(juce::Label::textColourId, juce::Colour(105, 116, 144));
	_crashWarning.setBounds(0, 380, 500, 40);
	_crashWarning.setFont(font2);
	_crashWarning.setJustificationType(juce::Justification::centred);
    addChildComponent(_crashWarning);
    
	_crashText.setText("Use the code below if you're going to contact us about these problems", juce::dontSendNotification);
	_crashText.setColour(juce::Label::textColourId, juce::Colour(105, 116, 144));
	_crashText.setBounds(0, 420, 500, 40);
	_crashText.setFont(font);
	_crashText.setJustificationType(juce::Justification::centred);
    addChildComponent(_crashText);

    auto font3 = juce::Font("Encode Sans", 24, 1);
	_installId.setText("", juce::dontSendNotification);
	_installId.setColour(juce::Label::textColourId, juce::Colours::black);
	_installId.setBounds(0, 460, 500, 40);
	_installId.setFont(font3);
	_installId.setJustificationType(juce::Justification::centred);
    addChildComponent(_installId);

}

WizardFinish::~WizardFinish()
{
    _logger->trace("Destroying Wizard Finish UI Component");
}

void WizardFinish::paint(juce::Graphics& g)
{
    g.fillAll(Colour(255, 255, 255));
}

void WizardFinish::resized()
{

}

void WizardFinish::buttonClicked(juce::Button* button)
{
    if (button == &_finish)
    {
        _logger->trace("Finish button pressed");
        if (_peerSucceed)
        {
            _peerLink.triggerClick();
        }
        // Wizard -> WizardWindow
        WizardWindow* window = (WizardWindow*)getParentComponent()->getParentComponent();
        window->closeButtonPressed();
    }
}

void WizardFinish::addPTPResult(bool installed, bool succeed)
{
	if (!installed)
	{
		_ptpResult.setText("Not Installed", juce::dontSendNotification);
		return;
	}
	if (succeed)
	{
		_ptpResult.setText("Installed", juce::dontSendNotification);
	}
	else
	{
		_ptpResult.setText("Failed", juce::dontSendNotification);
        activateCrash();
	}
    _installId.setText(SubutaiLauncher::Session::instance()->getHub()->getId(), juce::dontSendNotification);
}

void WizardFinish::addTrayResult(bool installed, bool succeed)
{
	if (!installed)
	{
		_trayResult.setText("Not Installed", juce::dontSendNotification);
		return;
	}
	if (succeed)
	{
		_trayResult.setText("Installed", juce::dontSendNotification);
	}
	else
	{
		_trayResult.setText("Failed", juce::dontSendNotification);
        activateCrash();
	}
    _installId.setText(SubutaiLauncher::Session::instance()->getHub()->getId(), juce::dontSendNotification);
}

void WizardFinish::addETEResult(bool installed, bool succeed)
{
	if (!installed)
	{
		_eteResult.setText("Not Installed", juce::dontSendNotification);
		return;
	}
	if (succeed)
	{
		_eteResult.setText("Installed", juce::dontSendNotification);
	}
	else
	{
		_eteResult.setText("Failed", juce::dontSendNotification);
        activateCrash();
	}
    _installId.setText(SubutaiLauncher::Session::instance()->getHub()->getId(), juce::dontSendNotification);
}

void WizardFinish::addPeerResult(bool installed, bool succeed)
{
	if (!installed)
	{
		_peerResult.setText("Not Installed", juce::dontSendNotification);
		return;
	}
	if (succeed)
	{
        _peerSucceed = true;
		_peerResult.setText("Installed", juce::dontSendNotification);
	}
	else
	{
		_peerResult.setText("Failed", juce::dontSendNotification);
        activateCrash();
	}
}

void WizardFinish::finalize()
{
	WizardWindow* window = (WizardWindow*)getParentComponent()->getParentComponent();
	window->closeButtonPressed();
	return;

}

void WizardFinish::activateCrash()
{
    _installId.setText(SubutaiLauncher::Session::instance()->getHub()->getId(), juce::dontSendNotification);
    _crashBot.setVisible(true);
    _crashWarning.setVisible(true);
    _crashText.setVisible(true);
    _installId.setVisible(true);
}
