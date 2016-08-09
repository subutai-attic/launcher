/*
   ==============================================================================

   This file was auto-generated!

   ==============================================================================
   */

#include "MainComponent.h"


//==============================================================================
MainContentComponent::MainContentComponent() 
{
    _menuItems.push_back("SUBUTAI");
    _menuItems.push_back("LIBRARY");
    _menuItems.push_back("TEMPLATES");
    _menuItems.push_back("YOUR HUB");

    // TODO: Add screen detection size and relative value at startup
    setSize (1024, 768);


    _core = new SubutaiLauncher::Core(std::vector<std::string>());
    _core->run();
    auto sl = new SubutaiLauncher::SL();
    try {
        sl->open("ui-banner");
    } catch (SubutaiLauncher::SubutaiException& e) {
        if (e.code() == 1) {
            std::printf("ui-banner.py file was not found\n");
            return;
        }
    }
    try {
        sl->execute();
    } catch (SubutaiLauncher::SLException& e) {
        std::printf(e.what());
    } catch (std::exception e) {
        std::printf(e.what());
    }

    addAndMakeVisible(_banner);
    _banner.setImage(ImageCache::getFromFile(File("launcher-banner-example1.jpg")));

    _mainMenu.setModel(this);
    _mainMenu.setColour (ListBox::backgroundColourId, Colour::greyLevel (0.2f));
    _mainMenu.selectRow(0);
    _mainMenu.setRowHeight(40);
    addAndMakeVisible(_mainMenu);

    _header.setColour(Toolbar::backgroundColourId, Colour::greyLevel (0.2f));
    addAndMakeVisible(_header);

    // Close button
    _closeButton = new ImageButton("Close");
    _closeButton->setBounds(980, 0, 1000, 20);
    Image closeImg(ImageCache::getFromFile(File("window_close.png")));
    _closeButton->setImages(true, true, true, closeImg, 1.0, Colour(), closeImg, 0.6, Colour(), closeImg, 0.6, Colour());
    _closeButton->addListener(this);
    addAndMakeVisible(_closeButton);
}

MainContentComponent::~MainContentComponent()
{
    //_login->deleteAndZero();
    _login->removeFromDesktop();
    delete(_login);
}

void MainContentComponent::paint (Graphics& g)
{

    g.fillAll (Colour (0xff222222));

    //g.setColour(Colour(0xff222222));
    //g.fillRect(Rectangle<int>(0, 0, 1024, 30));

    g.setFont (Font (16.0f));
    g.setColour (Colours::white);

    // Banner rotator

    //g.fillPath (getLogo(), RectanglePlacement (RectanglePlacement::centred).getTransformToFit (getLogo().getBounds(), getLocalBounds().reduced (250, getHeight() / 4).toFloat()));


    // showLoginScreen();
}

void MainContentComponent::resized()
{
    Rectangle<int> r (getLocalBounds().reduced (0));
    _banner.setBounds(r.withSize(WINDOW_WIDTH, WINDOW_HEIGHT));
    _mainMenu.setBounds (0, 20, MENU_WIDTH, 748);
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
        t.setColour(Colour(176, 224, 230));
    }
    t.setLineSpacing(40);
    t.setJustification(Justification::centred);
    Font f;
    f.setHeight(30);
    t.setFont(f);
    t.draw(g, Rectangle<int>(width + 8, 40).reduced(20, 0).toFloat());
}

void MainContentComponent::selectedRowsChanged(int row) {
    auto item = _menuItems.at(row);
    Rectangle<int> r (MENU_WIDTH, HEADER_HEIGHT, WINDOW_WIDTH, WINDOW_HEIGHT - HEADER_HEIGHT);
    if (item == "LIBRARY") {
        _community.setVisible(false);
        _hub.setVisible(false);
        _library.setVisible(false);
        _marketplace.setVisible(false);
        addAndMakeVisible(_library);
        _library.setBounds(r.withSize(WINDOW_WIDTH - MENU_WIDTH, WINDOW_HEIGHT - HEADER_HEIGHT));
    } else if (item == "SUBUTAI") {
        _community.setVisible(false);
        _hub.setVisible(false);
        _library.setVisible(false);
        _marketplace.setVisible(false);
        addAndMakeVisible(_community);
        _community.setBounds(r.withSize(WINDOW_WIDTH - MENU_WIDTH, WINDOW_HEIGHT - HEADER_HEIGHT));
    } else if (item == "TEMPLATES") {
        _community.setVisible(false);
        _hub.setVisible(false);
        _library.setVisible(false);
        _marketplace.setVisible(false);
        addAndMakeVisible(_marketplace);
        _marketplace.setBounds(r.withSize(WINDOW_WIDTH - MENU_WIDTH, WINDOW_HEIGHT - HEADER_HEIGHT));
    } else if (item == "YOUR HUB") {
        _community.setVisible(false);
        _hub.setVisible(false);
        _library.setVisible(false);
        _marketplace.setVisible(false);
        addAndMakeVisible(_hub);
        _hub.setBounds(r.withSize(WINDOW_WIDTH - MENU_WIDTH, WINDOW_HEIGHT - HEADER_HEIGHT));
    }
}

void MainContentComponent::showLoginScreen() {
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
}

void MainContentComponent::buttonClicked(Button* button) {
    JUCEApplication::getInstance()->systemRequestedQuit();
}

Path MainContentComponent::getLogo() {
    return Drawable::parseSVGPath(
            "M42,43c-0.8-0.4-1.5-0.9-2.3-1.3c-1.4-0.8-2.9-1.7-4.3-2.5c-0.8-0.5-1.6-0.8-2.4-0.8c-0.8,0-1.6,0.2-2.4,0.7"
            "L30,39.6c-1.9,1.2-3.9,2.3-6.1,2.8c-0.7,0.2-1.5,0.2-2.3,0.2c-3.6,0-6.9-1.9-9.7-3.6c-0.7-0.4-1.4-0.7-2.1-0.7c-0.7,0-1.3,0.2-2,0.6"
            "c-1.6,0.9-3.1,1.8-4.7,2.7l-2.4,1.4c-0.1,0.1-0.4,0.2-0.4,0.3l0,0.2c0,0.1,0.3,0.2,0.5,0.4c0,0,5.5,3.3,7.4,4.4"
            "c0.4,0.2,0.9,0.4,1.5,0.4l0.3,0c0,0,0.2,0,0.3,0c0.4,0,0.9-0.2,1.4-0.5c0.6-0.4,1.2-0.7,1.9-1.1c0.9-0.5,1.8-1.1,2.7-1.7"
            "c1.7-1.1,3.4-1.6,5.1-1.6c1.7,0,3.4,0.5,5.1,1.6c1.6,1,3.3,2,4.7,2.8c0.6,0.3,1.2,0.5,1.8,0.5c0.5,0,1.1-0.2,1.6-0.5"
            "c2-1.1,4-2.4,5.9-3.6l1.6-1c0.1-0.1,0.3-0.2,0.4-0.2l0-0.2c0-0.1-0.3-0.2-0.4-0.3L42,43z"
            "M27.6,52.3L27,51.9c-1.5-1-2.9-1.9-4.4-2.4c-0.4-0.1-0.7-0.2-1.1-0.2c-1.5,0-3,1-4.5,2.1"
            "c-0.6,0.4-1.1,0.8-1.7,1.1c0.4,0.3,0.9,0.6,1.3,0.9c1.7,1.1,3.2,2.2,4.9,2.5c1.7-0.3,3.3-1.3,5-2.5c0.4-0.3,0.8-0.6,1.3-0.8"
            "C27.7,52.4,27.7,52.4,27.6,52.3z"
            "M42,25.6c-0.8-0.4-1.5-0.9-2.3-1.3c-1.4-0.8-2.9-1.7-4.3-2.5c-0.8-0.5-1.6-0.8-2.4-0.8"
            "c-0.8,0-1.6,0.2-2.4,0.7L30,22.2c-1.9,1.2-3.9,2.3-6.1,2.8c-0.7,0.2-1.5,0.2-2.3,0.2c-3.6,0-6.9-1.9-9.7-3.6"
            "c-0.7-0.4-1.4-0.7-2.1-0.7c-0.7,0-1.3,0.2-2,0.6c-1.6,0.9-3.1,1.8-4.7,2.7l-2.4,1.4c-0.1,0.1-0.4,0.2-0.4,0.3l0,0.2"
            "c0,0.1,0.3,0.2,0.5,0.4c0,0,5.5,3.3,7.4,4.4c0.4,0.2,0.9,0.4,1.5,0.4l0.3,0c0,0,0.2,0,0.3,0c0.4,0,0.9-0.2,1.4-0.5"
            "c0.6-0.4,1.2-0.7,1.9-1.1c0.9-0.5,1.8-1.1,2.7-1.7c1.7-1.1,3.4-1.6,5.1-1.6c1.7,0,3.4,0.5,5.1,1.6c1.6,1,3.3,2,4.7,2.8"
            "c0.6,0.3,1.2,0.5,1.8,0.5c0.5,0,1.1-0.2,1.6-0.5c2-1.1,4-2.4,5.9-3.6l1.6-1c0.1-0.1,0.3-0.2,0.4-0.2l0-0.2c0-0.1-0.3-0.2-0.4-0.3"
            "L42,25.6z"
            "M16.6,35.9c1.7,1.1,3.2,2.2,4.9,2.5c1.7-0.3,3.3-1.3,5-2.5c0.4-0.3,0.8-0.6,1.3-0.8c0,0,0-0.1-0.1-0.2"
            "l-0.1,0c0,0,0,0,0,0l0-0.2c0-0.1-0.3-0.2-0.4-0.3l-0.7-0.4c-1.1-0.7-2.3-1.3-3.4-1.9c-0.5-0.3-1-0.4-1.5-0.4c-0.5,0-0.9,0.1-1.4,0.3"
            "c-1.1,0.6-2.3,1.2-3.4,1.9l-0.9,0.5c-0.4,0.2-0.4,0.3-0.4,0.4c0,0,0,0,0.1,0.1c-0.1,0.1-0.2,0.1-0.2,0.2"
            "C15.7,35.3,16.1,35.6,16.6,35.9z"
            "M15.9,17.8c1.7,1.1,3.5,1.7,5.6,1.7l0.1,0c2,0,3.8-0.6,5.6-1.7c0.1-0.1,0.3-0.2,0.4-0.3l0-0.2"
            "c0-0.1-0.3-0.2-0.4-0.3l-0.7-0.4c-1.1-0.7-2.3-1.3-3.4-1.9c-0.5-0.3-1-0.4-1.5-0.4c-0.5,0-0.9,0.1-1.4,0.3c-1.1,0.6-2.3,1.2-3.4,1.9"
            "L15.8,17c-0.4,0.2-0.4,0.3-0.4,0.4C15.5,17.5,15.7,17.7,15.9,17.8z"
            "M66.1,34.7H64c-0.6,0-0.8,0.2-0.8,0.8v4.2h-3.7v-4.2c0-0.6-0.2-0.8-0.8-0.8h-2.1c-0.6,0-0.8,0.2-0.8,0.8"
            "v11.8c0,0.6,0.2,0.8,0.8,0.8h2.1c0.6,0,0.8-0.2,0.8-0.8v-4.3h3.7v4.3c0,0.6,0.2,0.8,0.8,0.8h2.1c0.6,0,0.8-0.2,0.8-0.8V35.5"
            "C66.9,34.9,66.7,34.7,66.1,34.7z"
            "M79.2,34.7h-2.1c-0.6,0-0.8,0.2-0.8,0.8v7.8c0,1.1-0.7,1.7-2,1.7s-1.9-0.6-1.9-1.7v-7.8"
            "c0-0.6-0.2-0.8-0.8-0.8h-2.1c-0.6,0-0.8,0.2-0.8,0.8v7.7c0,3.2,2,5.2,5.7,5.2c3.7,0,5.7-2,5.7-5.2v-7.7"
            "C80.1,34.9,79.8,34.7,79.2,34.7z"
            "M91,41.1L91,41.1c1-0.7,1.4-1.6,1.4-2.8c0-2.3-1.6-3.6-4.2-3.6h-5.5c-0.6,0-0.8,0.2-0.8,0.8v11.8"
            "c0,0.6,0.2,0.8,0.8,0.8h5.6c2.7,0,4.6-1.3,4.6-3.7C92.9,42.7,92.2,41.6,91,41.1z M85.6,37.7h1.8c0.8,0,1.2,0.3,1.2,1.1s-0.4,1-1.2,1"
            "h-1.8V37.7z M87.7,45h-2.2v-2.2h2.2c0.8,0,1.2,0.3,1.2,1.1C88.9,44.7,88.6,45,87.7,45z"
            "M61,25.9c0-1.7-1.4-1.9-2.7-2.1c-1.1-0.1-2.2-0.3-2.2-1.4c0-0.9,0.7-1.4,2-1.4c1.1,0,1.7,0.4,2.1,1.2"
            "c0.1,0.1,0.1,0.2,0.3,0.1l0.3-0.1c0.1-0.1,0.2-0.1,0.1-0.3c-0.5-1.1-1.3-1.6-2.8-1.6c-1.8,0-2.7,0.9-2.7,2.2c0,1.7,1.5,1.9,2.8,2.1"
            "c1.1,0.1,2.1,0.3,2.1,1.4c0,1-0.7,1.5-2.1,1.5c-1.1,0-1.8-0.5-2.1-1.4c-0.1-0.1-0.1-0.2-0.3-0.1L55.4,26c-0.1,0.1-0.2,0.1-0.1,0.3"
            "c0.5,1.2,1.3,1.8,2.9,1.8C60,28.1,61,27.3,61,25.9z"
            "M64.2,28.1c1,0,1.6-0.2,2-0.3c0.3-0.1,0.4-0.3,0.4-0.6v-4.5c0-0.1-0.1-0.2-0.2-0.2H66c-0.1,0-0.2,0-0.2,0.2"
            "v4.2c0,0.1,0,0.2-0.2,0.3c-0.3,0.1-0.8,0.2-1.4,0.2c-1,0-1.5-0.5-1.5-1.5v-3.3c0-0.1,0-0.2-0.2-0.2h-0.3c-0.1,0-0.2,0-0.2,0.2V26"
            "C62.1,27.3,62.7,28.1,64.2,28.1z"
            "M72.4,25.9v-1.5c0-1.3-0.6-2.1-2.1-2.1c-0.8,0-1.4,0.2-1.7,0.3v-2.4c0-0.1-0.1-0.2-0.2-0.2h-0.3"
            "c-0.1,0-0.2,0-0.2,0.2v6.9c0,0.4,0.1,0.4,0.4,0.6c0.4,0.2,1,0.3,1.8,0.3C71.7,28.1,72.4,27.3,72.4,25.9z M68.8,27.2"
            "c-0.2-0.1-0.2-0.1-0.2-0.3v-3.6c0.3-0.1,0.9-0.3,1.6-0.3c1,0,1.5,0.5,1.5,1.4v1.5c0,1-0.5,1.5-1.6,1.5"
            "C69.6,27.5,69.1,27.4,68.8,27.2z"
            "M75.7,28.1c1,0,1.6-0.2,2-0.3c0.3-0.1,0.4-0.3,0.4-0.6v-4.5c0-0.1-0.1-0.2-0.2-0.2h-0.3"
            "c-0.1,0-0.2,0-0.2,0.2v4.2c0,0.1,0,0.2-0.2,0.3c-0.3,0.1-0.8,0.2-1.4,0.2c-1,0-1.5-0.5-1.5-1.5v-3.3c0-0.1,0-0.2-0.2-0.2h-0.3"
            "c-0.1,0-0.2,0-0.2,0.2V26C73.6,27.3,74.3,28.1,75.7,28.1z"
            "M82.4,27.9v-0.3c0-0.1,0-0.2-0.2-0.2c-1.1,0-1.5-0.2-1.5-1.5v-2.8h1.5c0.1,0,0.2-0.1,0.2-0.2v-0.3"
            "c0-0.1-0.1-0.2-0.2-0.2h-1.5v-1.1c0-0.1,0-0.2-0.2-0.2h-0.3c-0.1,0-0.2,0-0.2,0.2v1.1h-1c-0.1,0-0.2,0-0.2,0.2V23"
            "c0,0.1,0.1,0.2,0.2,0.2h1v2.8c0,1.7,0.6,2.2,2.2,2.2C82.4,28.1,82.4,28.1,82.4,27.9z"
            "M85.3,24.8c-1.7,0-2.3,0.4-2.3,1.6c0,1.1,0.4,1.6,2.3,1.6c0.8,0,1.4-0.1,1.8-0.3c0.3-0.1,0.4-0.2,0.4-0.6"
            "v-2.8c0-1.1-0.4-2.1-2.1-2.1c-1.1,0-1.8,0.4-2.1,1.1c-0.1,0.1,0,0.2,0.1,0.3l0.3,0.1c0.1,0.1,0.2,0,0.3-0.1c0.3-0.5,0.7-0.8,1.5-0.8"
            "c1.2,0,1.4,0.6,1.4,1.5v0.5C86.5,24.9,85.9,24.8,85.3,24.8z M86.7,27c0,0.2,0,0.2-0.2,0.3c-0.3,0.1-0.7,0.2-1.3,0.2"
            "c-1.2,0-1.5-0.4-1.5-1.1c0-0.7,0.4-1,1.5-1c0.6,0,1.3,0.1,1.5,0.1V27z"
            "M89,21.4L89,21.4c0.4,0,0.5-0.1,0.5-0.4v-0.1c0-0.3-0.1-0.4-0.4-0.4H89c-0.3,0-0.4,0.1-0.4,0.4V21"
            "C88.6,21.3,88.7,21.4,89,21.4z"
            "M89.2,22.5h-0.3c-0.1,0-0.2,0-0.2,0.2v5.1c0,0.1,0.1,0.2,0.2,0.2h0.3c0.1,0,0.2,0,0.2-0.2v-5.1"
            "C89.4,22.5,89.4,22.5,89.2,22.5z"
            "M96.2,23.8C95,23.7,94,23.5,94,22.4c0-0.9,0.7-1.4,2-1.4c1.1,0,1.7,0.4,2.1,1.2c0.1,0.1,0.1,0.2,0.3,0.1"
            "l0.3-0.1c0.1-0.1,0.2-0.1,0.1-0.3c-0.5-1.1-1.3-1.6-2.8-1.6c-1.8,0-2.7,0.9-2.7,2.2c0,1.7,1.5,1.9,2.8,2.1c1.1,0.1,2.1,0.3,2.1,1.4"
            "c0,1-0.7,1.5-2.1,1.5c-1.1,0-1.8-0.5-2.1-1.4c-0.1-0.1-0.1-0.2-0.3-0.1L93.2,26c-0.1,0.1-0.2,0.1-0.1,0.3c0.5,1.2,1.3,1.8,2.9,1.8"
            "c1.9,0,2.9-0.8,2.9-2.2C98.8,24.2,97.4,23.9,96.2,23.8z"
            "M102.1,22.3c-1.4,0-2.3,0.7-2.3,2v1.9c0,1.2,0.9,1.9,2.3,1.9c1.5,0,2.3-0.7,2.3-1.9v-1.9"
            "C104.4,23.1,103.6,22.3,102.1,22.3z M103.7,26.2c0,0.8-0.6,1.3-1.6,1.3c-1,0-1.6-0.5-1.6-1.3v-1.9c0-0.8,0.6-1.3,1.6-1.3"
            "c1,0,1.6,0.5,1.6,1.3V26.2z"
            "M109.6,26.3c-0.2,0-0.2,0-0.3,0.1c-0.2,0.7-0.7,1.1-1.6,1.1c-1,0-1.6-0.6-1.6-1.4v-1.6"
            "c0-0.9,0.5-1.4,1.6-1.4c0.9,0,1.3,0.3,1.6,1c0.1,0.1,0.1,0.2,0.3,0.1l0.3-0.1c0.1-0.1,0.2-0.1,0.1-0.3c-0.3-0.9-0.9-1.4-2.2-1.4"
            "c-1.6,0-2.3,0.8-2.3,2.1V26c0,1.3,0.8,2.1,2.3,2.1c1.3,0,2-0.5,2.3-1.5c0.1-0.2,0-0.2-0.1-0.3L109.6,26.3z"
            "M111.6,22.5h-0.3c-0.1,0-0.2,0-0.2,0.2v5.1c0,0.1,0.1,0.2,0.2,0.2h0.3c0.1,0,0.2,0,0.2-0.2v-5.1"
            "C111.8,22.5,111.8,22.5,111.6,22.5z"
            "M111.5,20.5L111.5,20.5c-0.4,0-0.5,0.1-0.5,0.4V21c0,0.3,0.1,0.4,0.4,0.4h0.1c0.3,0,0.4-0.1,0.4-0.4v-0.1"
            "C111.9,20.6,111.8,20.5,111.5,20.5z"
            "M115.2,22.3c-1.1,0-1.8,0.4-2.1,1.1c-0.1,0.1,0,0.2,0.1,0.3l0.3,0.1c0.1,0.1,0.2,0,0.3-0.1"
            "c0.3-0.5,0.7-0.8,1.5-0.8c1.2,0,1.4,0.6,1.4,1.5v0.5c-0.2,0-0.8-0.1-1.4-0.1c-1.7,0-2.3,0.4-2.3,1.6c0,1.1,0.4,1.6,2.3,1.6"
            "c0.8,0,1.4-0.1,1.8-0.3c0.3-0.1,0.4-0.2,0.4-0.6v-2.8C117.4,23.3,116.9,22.3,115.2,22.3z M116.6,27c0,0.2,0,0.2-0.2,0.3"
            "c-0.3,0.1-0.7,0.2-1.3,0.2c-1.2,0-1.5-0.4-1.5-1.1c0-0.7,0.4-1,1.5-1c0.6,0,1.3,0.1,1.5,0.1V27z"
            "M119.2,20.1h-0.3c-0.1,0-0.2,0-0.2,0.2v7.5c0,0.1,0,0.2,0.2,0.2h0.3c0.1,0,0.2,0,0.2-0.2v-7.5"
            "C119.4,20.1,119.4,20.1,119.2,20.1z"
            );
}