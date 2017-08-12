#include "SystemCheck.h"

SystemCheck::SystemCheck()
{
    _logger = &Poco::Logger::get("subutai");
    _logger->trace("Starting System Check UI Component");
    auto font = juce::Font(15);
    auto font2 = juce::Font(13);

    SubutaiLauncher::Environment env;

    _osLabel.setText("Operating System", dontSendNotification);
    _osLabel.setColour(Label::textColourId, Colour(105, 116, 144));
    _osLabel.setBounds(15, 15, 150, 40);
    _osLabel.setFont(font);
    _osLabel.setJustificationType(Justification::top);
    addAndMakeVisible(_osLabel);

    bool osSupported = false;
    std::string pOs = env.versionOS();
    std::string pOsVersion = env.versionNumber();
    pOs.append(" ");
    pOs.append(pOsVersion);

#if LAUNCHER_MACOS
    if (pOsVersion.substr(0, 5) == "10.12" || pOsVersion.substr(0, 5) == "10.13")
    {
        osSupported = true;
    }
#elif LAUNCHER_LINUX
    std::string v = pOsVersion.substr(0, 5);
    if (v == "16.04" || v == "16.10" || v == "17.04" || v == "17.10")
    {
        osSupported = true;
    }
#endif

    _osValue.setText(pOs, dontSendNotification);
    if (osSupported) _osValue.setColour(Label::textColourId, Colour(105, 116, 144));
    else _osValue.setColour(Label::textColourId, Colours::red);
    _osValue.setBounds(150, 15, 500, 40);
    _osValue.setFont(font);
    _osValue.setJustificationType(Justification::top);
    addAndMakeVisible(_osValue);

    _osInfo.setText("We support Windows (8+), MacOS and Linux (Ubuntu 16.04+)", dontSendNotification);
    _osInfo.setColour(Label::textColourId, Colours::grey);
    _osInfo.setBounds(150, 35, 500, 40);
    _osInfo.setFont(font2);
    _osInfo.setJustificationType(Justification::top);
    addAndMakeVisible(_osInfo);

    _archLabel.setText("CPU Architecture", dontSendNotification);
    _archLabel.setColour(Label::textColourId, Colour(105, 116, 144));
    _archLabel.setBounds(15, 55, 150, 40);
    _archLabel.setFont(font);
    _archLabel.setJustificationType(Justification::top);
    addAndMakeVisible(_archLabel);

    _archValue.setText(env.cpuArch(), dontSendNotification);
    _archValue.setColour(Label::textColourId, Colour(105, 116, 144));
    _archValue.setBounds(150, 55, 500, 40);
    _archValue.setFont(font);
    _archValue.setJustificationType(Justification::top);
    addAndMakeVisible(_archValue);

    _archInfo.setText("Subutai support x64 processor architecture only", dontSendNotification);
    _archInfo.setColour(Label::textColourId, Colours::grey);
    _archInfo.setBounds(150, 75, 500, 40);
    _archInfo.setFont(font2);
    _archInfo.setJustificationType(Justification::top);
    addAndMakeVisible(_archInfo);

    _cpuLabel.setText("CPU Cores", dontSendNotification);
    _cpuLabel.setColour(Label::textColourId, Colour(105, 116, 144));
    _cpuLabel.setBounds(15, 95, 150, 40);
    _cpuLabel.setFont(font);
    _cpuLabel.setJustificationType(Justification::top);
    addAndMakeVisible(_cpuLabel);

    std::string cores = Poco::format("%u", env.cpuNum());

    _cpuValue.setText(cores, dontSendNotification);
    _cpuValue.setColour(Label::textColourId, Colour(105, 116, 144));
    _cpuValue.setBounds(150, 95, 500, 40);
    _cpuValue.setFont(font);
    _cpuValue.setJustificationType(Justification::top);
    addAndMakeVisible(_cpuValue);

    _cpuInfo.setText("It is recommended to have at least 2 cores per peer", dontSendNotification);
    _cpuInfo.setColour(Label::textColourId, Colours::grey);
    _cpuInfo.setBounds(150, 115, 500, 40);
    _cpuInfo.setFont(font2);
    _cpuInfo.setJustificationType(Justification::top);
    addAndMakeVisible(_cpuInfo);

    _memLabel.setText("Total Memory", dontSendNotification);
    _memLabel.setColour(Label::textColourId, Colour(105, 116, 144));
    _memLabel.setBounds(15, 135, 150, 40);
    _memLabel.setFont(font);
    _memLabel.setJustificationType(Justification::top);
    addAndMakeVisible(_memLabel);

    std::string memValue = Poco::format("%Lu MB", env.ramSize() / 1024 / 1024);

    _memValue.setText(memValue, dontSendNotification);
    _memValue.setColour(Label::textColourId, Colour(105, 116, 144));
    _memValue.setBounds(150, 135, 500, 40);
    _memValue.setFont(font);
    _memValue.setJustificationType(Justification::top);
    addAndMakeVisible(_memValue);

    _memInfo.setText("Each peer require at least 2GB of RAM", dontSendNotification);
    _memInfo.setColour(Label::textColourId, Colours::grey);
    _memInfo.setBounds(150, 155, 500, 40);
    _memInfo.setFont(font2);
    _memInfo.setJustificationType(Justification::top);
    addAndMakeVisible(_memInfo);

    _vtxLabel.setText("Virtualization support", dontSendNotification);
    _vtxLabel.setColour(Label::textColourId, Colour(105, 116, 144));
    _vtxLabel.setBounds(15, 175, 150, 40);
    _vtxLabel.setFont(font);
    _vtxLabel.setJustificationType(Justification::top);
    addAndMakeVisible(_vtxLabel);

    std::string vtxStatus = "Disabled";
    if (env.vtxEnabled()) vtxStatus = "Enabled";

    _vtxValue.setText(vtxStatus, dontSendNotification);
    _vtxValue.setColour(Label::textColourId, Colour(105, 116, 144));
    if (vtxStatus == "Disabled")
    {
        _vtxValue.setColour(Label::textColourId, Colours::red);
        
    }
    _vtxValue.setBounds(150, 175, 500, 40);
    _vtxValue.setFont(font);
    _vtxValue.setJustificationType(Justification::top);
    addAndMakeVisible(_vtxValue);

    _vtxInfo.setText("VT-x should be enabled", dontSendNotification);
    _vtxInfo.setColour(Label::textColourId, Colours::grey);
    _vtxInfo.setBounds(150, 195, 500, 40);
    _vtxInfo.setFont(font2);
    _vtxInfo.setJustificationType(Justification::top);
    addAndMakeVisible(_vtxInfo);

    _vbLabel.setText("VirtualBox version", dontSendNotification);
    _vbLabel.setColour(Label::textColourId, Colour(105, 116, 144));
    _vbLabel.setBounds(15, 215, 150, 40);
    _vbLabel.setFont(font);
    _vbLabel.setJustificationType(Justification::top);
    addAndMakeVisible(_vbLabel);

    SubutaiLauncher::VirtualBox vb;
	try {
		vb.findInstallation();
		if (vb.isInstalled())
		{
			_vbValue.setText(vb.extractVersion(), dontSendNotification);
			_vbValue.setColour(Label::textColourId, Colour(105, 116, 144));
		}
		else
		{
			_vbValue.setText("Not installed", dontSendNotification);
			_vbValue.setColour(Label::textColourId, Colours::red);
		}
	}
	catch (Poco::NotFoundException& e)
	{
		_vbValue.setText("Not installed", dontSendNotification);
		_vbValue.setColour(Label::textColourId, Colours::red);
	}
	catch (Poco::SystemException& e)
	{
		_vbValue.setText("Not installed", dontSendNotification);
		_vbValue.setColour(Label::textColourId, Colours::red);
	}
    
    _vbValue.setBounds(150, 215, 500, 40);
    _vbValue.setFont(font);
    _vbValue.setJustificationType(Justification::top);
    addAndMakeVisible(_vbValue);

    _vbInfo.setText("VirtualBox is used to create virtual machines", dontSendNotification);
    _vbInfo.setColour(Label::textColourId, Colours::grey);
    _vbInfo.setBounds(150, 235, 500, 40);
    _vbInfo.setFont(font2);
    _vbInfo.setJustificationType(Justification::top);
    addAndMakeVisible(_vbInfo);
    _logger->trace("System Check UI Component created");
}

SystemCheck::~SystemCheck()
{
    _logger->trace("Destroying System Check UI Component");
}

void SystemCheck::paint(juce::Graphics& g)
{
    g.fillAll(Colour(255, 255, 255));
    _logger->trace("SystemCheck::paint");
}

void SystemCheck::resized()
{
    _logger->trace("SystemCheck::resized");

}
