#include "MainComponent.h"

MainContentComponent::MainContentComponent() : _logoImage(nullptr)
{
    _logger = &Poco::Logger::get("subutai");
    auto font = juce::Font("Encode Sans", 20, 1);
    auto fontVersion = juce::Font("Encode Sans", 15, 1);

    _menuItems.push_back("News");
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
    _mainMenu.setRowHeight(55);
    _mainMenu.setBounds(40, 180, MENU_WIDTH, 500);
    addAndMakeVisible(_mainMenu);

    _header.setColour(Toolbar::backgroundColourId, Colour::greyLevel (0.2f));
    addChildComponent(_library);
    addChildComponent(_hub);
    addChildComponent(_marketplace);
    addChildComponent(_community);

    _title.setText("Subutai Launcher", dontSendNotification);
    _title.setColour(Label::textColourId, juce::Colour(105, 116, 144));
    _title.setBounds(0, 110, 250, 25);
    _title.setFont(font);
    _title.setJustificationType(Justification::centred);
    addAndMakeVisible(_title);

    _version.setText("Version 4.0.16", dontSendNotification);
    _version.setColour(Label::textColourId, juce::Colour(105, 116, 144));
    _version.setBounds(0, 130, 250, 25);
    _version.setFont(fontVersion);
    _version.setJustificationType(Justification::centred);
    addAndMakeVisible(_version);
}

MainContentComponent::~MainContentComponent()
{
    if (_logoImage) delete _logoImage;

}

void MainContentComponent::paint (Graphics& g)
{
    g.fillAll(juce::Colour::fromRGB(247, 249, 252));
    g.setFont (Font (16.0f));
    g.setColour (juce::Colour::fromRGB(105, 116, 144));

    // showLoginScreen();
}

void MainContentComponent::resized()
{
    int pLogoWidth = 64;
    int pLogoHeight = 64;
    pLogoWidth = _logo.getImage().getWidth();
    pLogoHeight = _logo.getImage().getHeight();
    juce::Rectangle<int> r (getLocalBounds().reduced (0));
    _mainMenu.setBounds(40, 180, MENU_WIDTH, 500);
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

    return;

    auto item = _menuItems.at(rowNumber);
    AttributedString t;
    t.append(item);
    //t.append("");
    /*
    if (!rowIsSelected) {
        t.setColour (juce::Colour::fromRGB(105, 116, 144));
    } else {
        //t.setColour(Colour(176, 224, 230));
    }
    */
    t.setLineSpacing(0);
    t.setJustification(Justification::left);

    Font f("Encode Sans", 17, 1);
    t.setFont(f);

    _logger->debug("MA: %s, %d, W: %d, H: %d", item, rowNumber, width, height);

    if (rowNumber == 0)
    {
        g.drawRect(40, 180, 210, 40);
        //t.draw(g, juce::Rectangle<int>(0, 0, 250, 40).toFloat());
    }
    else if (rowNumber == 1)
    {
        g.drawRect(40, 235, 210, 40);
        //t.draw(g, juce::Rectangle<int>(0, 20, 250, 40).toFloat());
    }
    else if (rowNumber == 2)
    {
        g.drawRect(40, 290, 210, 40);
        //t.draw(g, juce::Rectangle<int>(0, 60, 250, 40).toFloat());
    }
    else if (rowNumber == 3)
    {
        g.drawRect(40, 345, 210, 40);
        //t.draw(g, juce::Rectangle<int>(0, 160, 250, 40).toFloat());
    }

    /*
    if (rowNumber == 0)
    {
        t.draw(g, juce::Rectangle<int>(0, 0, 250, 40).toFloat());
    }
    else if (rowNumber == 1)
    {
        t.draw(g, juce::Rectangle<int>(0, 20, 250, 40).toFloat());
    }
    else if (rowNumber == 2)
    {
        t.draw(g, juce::Rectangle<int>(0, 60, 250, 40).toFloat());
    }
    else if (rowNumber == 3)
    {
        t.draw(g, juce::Rectangle<int>(0, 160, 250, 40).toFloat());
    }
    */

    /*
    if (rowNumber == 0)
    {
        t.draw(g, juce::Rectangle<int>(40, 180, 250, 40).toFloat());
    }
    else if (rowNumber == 1)
    {
        t.draw(g, juce::Rectangle<int>(40, 235, 250, 40).toFloat());
    }
    else if (rowNumber == 2)
    {
        t.draw(g, juce::Rectangle<int>(40, 290, 250, 40).toFloat());
    }
    else if (rowNumber == 3)
    {
        t.draw(g, juce::Rectangle<int>(40, 345, 250, 40).toFloat());
    }
    */

    /*
    if (rowNumber == 2)
    {
        t.draw(g, juce::Rectangle<int>(40, 290, 250, 40).toFloat());
    }
    else
    {
        t.draw(g, juce::Rectangle<int>(width + 8, 40).reduced(20, 0).toFloat());
    }
    */
}

void MainContentComponent::selectedRowsChanged(int row) {
    auto item = _menuItems.at(row);
    if (item == "Library") 
    {
        _sidebar.activateLibrary();
        _community.setVisible(false);
        _hub.setVisible(false);
        _marketplace.setVisible(false);
        _library.setVisible(true);
        _library.setBounds(MENU_WIDTH, 30, getParentWidth() - MENU_WIDTH - 40, getParentHeight() - 60);
    } 
    else if (item == "News") 
    {
        _sidebar.activateNews();
        _hub.setVisible(false);
        _library.setVisible(false);
        _marketplace.setVisible(false);
        _community.setVisible(true);
        _community.setBounds(MENU_WIDTH, 30, getParentWidth() - MENU_WIDTH - 40, getParentHeight() - 60);
    } 
    else if (item == "Templates") 
    {
        _sidebar.activateTemplates();
        _community.setVisible(false);
        _hub.setVisible(false);
        _library.setVisible(false);
        _marketplace.setVisible(true);
        _marketplace.setBounds(MENU_WIDTH, 30, getParentWidth() - MENU_WIDTH - 40, getParentHeight() - 60);
    } 
    else if (item == "Community") 
    {
        _sidebar.activateCommunity();
        _library.setVisible(false);
        _marketplace.setVisible(false);
        _community.setVisible(false);
        _hub.setVisible(true);
        _hub.setBounds(MENU_WIDTH, 30, getParentWidth() - MENU_WIDTH - 40, getParentHeight() - 60);
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

