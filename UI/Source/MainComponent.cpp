#include "MainComponent.h"

MainContentComponent::MainContentComponent() 
{
    _logger = &Poco::Logger::get("subutai");
    auto font = juce::Font(15);

    _menuItems.push_back("Subutai");
    _menuItems.push_back("Library");
    _menuItems.push_back("Templates");
    _menuItems.push_back("Community");

    _sidebar.setBounds(0, 0, 250, getParentHeight());
    _sidebar.toFront(false);
    addAndMakeVisible(_sidebar);

    // TODO: Add screen detection size and relative value at startup
    setSize(getParentWidth(), getParentHeight());

    //_logoImage = new Logo();
    //_logo.setImage(*_logoImage->getImage());
    std::string pLogoFile(SubutaiLauncher::Session::instance()->getSettings()->getTmpPath() + "launcher-logo.png");
    Poco::File pLogo(pLogoFile);
    if (pLogo.exists())
    {
        _logo.setImage(juce::ImageCache::getFromFile(juce::File(pLogoFile)));
    }
    else
    {
        _logger->error("launcher-logo.png doesn't exists");
    }
    addAndMakeVisible(_logo);

    _mainMenu.setModel(this);
    _mainMenu.setColour(ListBox::backgroundColourId, Colour(0x00000000));
    _mainMenu.selectRow(0);
    _mainMenu.setRowHeight(40);
    _mainMenu.setBounds (0, 120, MENU_WIDTH, 500);
    addAndMakeVisible(_mainMenu);

    _header.setColour(Toolbar::backgroundColourId, Colour::greyLevel (0.2f));
    addChildComponent(_library);
    addChildComponent(_hub);
    addChildComponent(_marketplace);
    addChildComponent(_community);

    // Welcome words
    _welcome.setText("Welcome to", dontSendNotification);
    _welcome.setColour(Label::textColourId, juce::Colour(255, 255, 255));
    _welcome.setBounds(0, 90, 250, 25);
    _welcome.setFont(font);
    _welcome.setJustificationType(Justification::centred);
    addAndMakeVisible(_welcome);

    _title.setText("Subutai Launcher", dontSendNotification);
    _title.setColour(Label::textColourId, juce::Colour(255, 255, 255));
    _title.setBounds(0, 110, 250, 25);
    _title.setFont(font);
    _title.setJustificationType(Justification::centred);
    addAndMakeVisible(_title);

    _version.setText("Version 4.0.16", dontSendNotification);
    _version.setColour(Label::textColourId, juce::Colour(255, 255, 255));
    _version.setBounds(0, 130, 250, 25);
    _version.setFont(font);
    _version.setJustificationType(Justification::centred);
    addAndMakeVisible(_version);
}

MainContentComponent::~MainContentComponent()
{
    delete _logoImage;
}

void MainContentComponent::paint (Graphics& g)
{

    //g.fillAll (Colour (0xff222222));

    g.setFont (Font (16.0f));
    g.setColour (Colours::white);

    // showLoginScreen();
}

void MainContentComponent::resized()
{
    int pLogoWidth = 64;
    int pLogoHeight = 64;
    pLogoWidth = _logo.getImage().getWidth();
    pLogoHeight = _logo.getImage().getHeight();
    juce::Rectangle<int> r (getLocalBounds().reduced (0));
    _mainMenu.setBounds (0, 160, MENU_WIDTH, 500);
    _logo.setBounds(int((MENU_WIDTH / 2) - (pLogoWidth / 2)), 20, pLogoWidth, pLogoHeight);
    _sidebar.setBounds(0, 0, 250, getParentHeight());
    //_header.setBounds(r.withSize(1024, HEADER_HEIGHT));
    //_library.setBounds(MENU_WIDTH, 0, getParentWidth() - MENU_WIDTH, getParentHeight());
    _library.setBounds(MENU_WIDTH, 0, getParentWidth() - MENU_WIDTH, getParentHeight());
    _community.setBounds(MENU_WIDTH, 0, getParentWidth() - MENU_WIDTH, getParentHeight());
    _marketplace.setBounds(MENU_WIDTH, 0, getParentWidth() - MENU_WIDTH, getParentHeight());
    _hub.setBounds(MENU_WIDTH, 0, getParentWidth() - MENU_WIDTH, getParentHeight());
    repaint();
}

int MainContentComponent::getNumRows() {
    return 4; // random
}

void MainContentComponent::paintListBoxItem(int rowNumber, Graphics& g, int width, int height, bool rowIsSelected) {
    if ((size_t)rowNumber > _menuItems.size()) {
        return;
    }

    auto item = _menuItems.at(rowNumber);
    AttributedString t;
    t.append(item);
    if (!rowIsSelected) {
        t.setColour(Colour(200, 200, 200));
    } else {
        //t.setColour(Colour(176, 224, 230));
        t.setColour(Colour(7, 141, 208));
    }
    t.setLineSpacing(40);
    t.setJustification(Justification::left);

    Font f;
    f.setHeight(30);
    t.setFont(f);
    t.draw(g, juce::Rectangle<int>(width + 8, 40).reduced(20, 0).toFloat());
}

void MainContentComponent::selectedRowsChanged(int row) {
    auto item = _menuItems.at(row);
    if (item == "Library") {
        _community.setVisible(false);
        _hub.setVisible(false);
        _marketplace.setVisible(false);
        _library.setVisible(true);
        //addAndMakeVisible(_library);
        //_library.setBounds(MENU_WIDTH, 0, getParentWidth() - MENU_WIDTH, getParentHeight());
        _library.setBounds(MENU_WIDTH, 0, getParentWidth() - MENU_WIDTH, getParentHeight());
    } else if (item == "Subutai") {
        _hub.setVisible(false);
        _library.setVisible(false);
        _marketplace.setVisible(false);
        _community.setVisible(true);
        //addAndMakeVisible(_community);
        _community.setBounds(MENU_WIDTH, 0, getParentWidth() - MENU_WIDTH, getParentHeight());
    } else if (item == "Templates") {
        _community.setVisible(false);
        _hub.setVisible(false);
        _library.setVisible(false);
        _marketplace.setVisible(true);
        //addAndMakeVisible(_marketplace);
        _marketplace.setBounds(MENU_WIDTH, 0, getParentWidth() - MENU_WIDTH, getParentHeight());
    } else if (item == "Community") {
        _library.setVisible(false);
        _marketplace.setVisible(false);
        _community.setVisible(false);
        _hub.setVisible(true);
        //addAndMakeVisible(_hub);
        _hub.setBounds(MENU_WIDTH, 0, getParentWidth() - MENU_WIDTH, getParentHeight());
    }
}

void MainContentComponent::showLoginScreen() {
    /* 
       _login = new LoginScreen();
       _login->addToDesktop(ComponentPeer::windowIsTemporary);
       Rectangle<int> area (0, 0, 200, 300);

       RectanglePlacement placement (RectanglePlacement::xMid
       | RectanglePlacement::yMid
       | RectanglePlacement::doNotResize);

       Rectangle<int> result (placement.appliedTo (area, Desktop::getInstance().getDisplays() 
       .getMainDisplay().userArea.reduced (20)));
       _login->setBounds (result);
       _login->setVisible(true);
       */
}

void MainContentComponent::buttonClicked(Button* button) {
}

