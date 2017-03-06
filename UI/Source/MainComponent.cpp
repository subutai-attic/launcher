/*
   ==============================================================================

   This file was auto-generated!

   ==============================================================================
   */

#include "MainComponent.h"


//==============================================================================
MainContentComponent::MainContentComponent() 
{
    _menuItems.push_back("Subutai");
    _menuItems.push_back("Library");
    _menuItems.push_back("Templates");
    _menuItems.push_back("Community");

    addAndMakeVisible(_sidebar);
    _sidebar.setBounds(0, 20, 250, 768-20);
    _sidebar.toFront(false);

    // TODO: Add screen detection size and relative value at startup
    setSize (1024, 768);

    addAndMakeVisible(_logo);

    _mainMenu.setModel(this);
    _mainMenu.setColour (ListBox::backgroundColourId, Colour(0x00000000));
    _mainMenu.selectRow(0);
    _mainMenu.setRowHeight(40);
    addAndMakeVisible(_mainMenu);

    _header.setColour(Toolbar::backgroundColourId, Colour::greyLevel (0.2f));
    addAndMakeVisible(_header);

    // Close button
    _closeButton = new ImageButton("Close");
    _closeButton->setBounds(1000, 3, 1020, 20);
    Image closeImg(ImageCache::getFromFile(File("close_window.png")));
    _closeButton->setImages(true, true, true, closeImg, 1.0, Colour(), closeImg, 0.6, Colour(), closeImg, 0.6, Colour());
    _closeButton->addListener(this);
    addAndMakeVisible(_closeButton);

    _minimizeButton = new ImageButton("Minimize");
    _minimizeButton->setBounds(970, 13, 990, 20);
    Image minimizeImg(ImageCache::getFromFile(File("minimize_window.png")));
    _minimizeButton->setImages(true, true, true, minimizeImg, 1.0, Colour(), minimizeImg, 0.6, Colour(), minimizeImg, 0.6, Colour());
    _minimizeButton->addListener(this);
    addAndMakeVisible(_minimizeButton);

}


MainContentComponent::~MainContentComponent()
{
    delete(_minimizeButton);
    delete(_closeButton);
//_login->deleteAndZero();
//_login->removeFromDesktop();
//delete(_login);
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
    juce::Rectangle<int> r (getLocalBounds().reduced (0));
    _mainMenu.setBounds (0, 120, MENU_WIDTH, 728);
    _logo.setBounds(0, 20, MENU_WIDTH, 100);
    _header.setBounds(r.withSize(1024, HEADER_HEIGHT));
    repaint();
}

int MainContentComponent::getNumRows() {
    return 4; // random
}

void MainContentComponent::paintListBoxItem(int rowNumber, Graphics& g, int width, int height, bool rowIsSelected) {
    if (rowNumber > _menuItems.size()) {
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
	juce::Rectangle<int> r (MENU_WIDTH, HEADER_HEIGHT, WINDOW_WIDTH, WINDOW_HEIGHT - HEADER_HEIGHT);
    if (item == "Library") {
        _community.setVisible(false);
        _hub.setVisible(false);
        _library.setVisible(false);
        _marketplace.setVisible(false);
        addAndMakeVisible(_library);
        _library.setBounds(r.withSize(WINDOW_WIDTH - MENU_WIDTH, WINDOW_HEIGHT - HEADER_HEIGHT));
    } else if (item == "Subutai") {
        _community.setVisible(false);
        _hub.setVisible(false);
        _library.setVisible(false);
        _marketplace.setVisible(false);
        addAndMakeVisible(_community);
        _community.setBounds(r.withSize(WINDOW_WIDTH - MENU_WIDTH, WINDOW_HEIGHT - HEADER_HEIGHT));
    } else if (item == "Templates") {
        _community.setVisible(false);
        _hub.setVisible(false);
        _library.setVisible(false);
        _marketplace.setVisible(false);
        addAndMakeVisible(_marketplace);
        _marketplace.setBounds(r.withSize(WINDOW_WIDTH - MENU_WIDTH, WINDOW_HEIGHT - HEADER_HEIGHT));
    } else if (item == "Community") {
        _community.setVisible(false);
        _hub.setVisible(false);
        _library.setVisible(false);
        _marketplace.setVisible(false);
        addAndMakeVisible(_hub);
        _hub.setBounds(r.withSize(WINDOW_WIDTH - MENU_WIDTH, WINDOW_HEIGHT - HEADER_HEIGHT));
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
    if (button == _closeButton) {
        JUCEApplication::getInstance()->systemRequestedQuit();
    } else {
    }
}

