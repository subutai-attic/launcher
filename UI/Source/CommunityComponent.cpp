/*
   ==============================================================================

   CommunityComponent.cpp
Created: 8 Aug 2016 2:15:00pm
Author:  crioto

==============================================================================
*/

#include "CommunityComponent.h"

CommunityComponent::CommunityComponent() {
    auto d = SubutaiLauncher::Session::instance()->getDownloader();
    d->setFilename("ui-banner.py");
    auto t = d->download();
    t.join();

    auto sl = new SubutaiLauncher::SL();
    try {
        sl->open("ui-banner");
    } catch (SubutaiLauncher::SubutaiException& e) {
        delete sl;
        if (e.code() == 1) {
            std::printf("ui-banner.py file was not found\n");
            return;
        }
    }
    try {
        sl->execute();
    } catch (SubutaiLauncher::SLException& e) {
        delete sl;
        std::printf("Exception: %s\n", e.displayText().c_str());
    } catch (std::exception e) {
        delete sl;
        std::printf("Exception: %s\n", e.what());
    }
    delete sl;

    // TODO: Make this not hard coded
    // Right now we only know that we have 4 images
    
    std::vector<std::string> images;
    images.push_back("/tmp/subutai/launcher-ad-1.png");
    images.push_back("/tmp/subutai/launcher-ad-2.png");
    images.push_back("/tmp/subutai/launcher-ad-3.png");
    images.push_back("/tmp/subutai/launcher-ad-4.png");

    for (auto it = images.begin(); it != images.end(); it++) {
        auto b = new FrontBanner();
        b->Create(ImageCache::getFromFile(File((*it))), "Some random text for test how it works");
        addAndMakeVisible(b);
        _imgs.push_back(b);
    }

    return;

    auto mainFont = new Font(30.0);
    auto secFont = new Font(24.0);

    _mainBanner.setImage(ImageCache::getFromFile(File("/tmp/subutai/launcher-ad-1.png")));
    _mainBanner.setBounds(20, 20, 357, 708);
    addAndMakeVisible(_mainBanner);

    _mainLabel.setText("Learn how cloud computing makes your life easier", dontSendNotification);
    _mainLabel.setColour(Label::backgroundColourId, Colour(0xaa222222));
    _mainLabel.setColour(Label::textColourId, Colours::white);
    _mainLabel.setJustificationType(Justification::centred);
    _mainLabel.setFont(*mainFont);
    _mainLabel.setBounds(20, 708-202, 357, 222);
    addAndMakeVisible(_mainLabel);

    _banner1.setImage(ImageCache::getFromFile(File("/tmp/subutai/launcher-ad-2.png")));
    _banner1.setBounds(397, 20, 357, 222);
    addAndMakeVisible(_banner1);

    _banner2.setImage(ImageCache::getFromFile(File("/tmp/subutai/launcher-ad-3.png")));
    _banner2.setBounds(397, 264, 357, 222);
    addAndMakeVisible(_banner2);

    _banner3.setImage(ImageCache::getFromFile(File("/tmp/subutai/launcher-ad-4.png")));
    _banner3.setBounds(397, 506, 357, 222);
    addAndMakeVisible(_banner3);

    _label1.setText("Make your own cloud", dontSendNotification);
    _label1.setColour(Label::backgroundColourId, Colour(0xaa222222));
    _label1.setColour(Label::textColourId, Colours::white);
    _label1.setJustificationType(Justification::centred);
    _label1.setFont(*secFont);
    _label1.setBounds(397, 142, 357, 100);
    addAndMakeVisible(_label1);

    _label2.setText("New era of cloud computing is here", dontSendNotification);
    _label2.setColour(Label::backgroundColourId, Colour(0xaa222222));
    _label2.setColour(Label::textColourId, Colours::white);
    _label2.setJustificationType(Justification::centred);
    _label2.setFont(*secFont);
    _label2.setBounds(397, 392, 357, 100);
    addAndMakeVisible(_label2);

    _label3.setText("4.0.15 is live! See patch notes", dontSendNotification);
    _label3.setColour(Label::backgroundColourId, Colour(0xaa222222));
    _label3.setColour(Label::textColourId, Colours::white);
    _label3.setJustificationType(Justification::centred);
    _label3.setFont(*secFont);
    _label3.setBounds(397, 642, 357, 100);
    addAndMakeVisible(_label3);

    /*  
        b1.img.setImage(ImageCache::getFromFile(File("launcher-banner-example1.jpg")));
        b1.label.setText("Subutai text example 1");
        b1.label.setColour(Label::textColourId, Colours::white);
        b1.label.setColour(Label::backgroundColourId, Colours::grey);
        */

    /* 
       addAndMakeVisible(_banner);
       _banner.setImage(ImageCache::getFromFile(File("launcher-banner-example1.jpg")));
       */
}

CommunityComponent::~CommunityComponent() {

}

void CommunityComponent::paint(Graphics& g) {
    g.fillAll (Colour (0xff222222));

    //g.setGradientFill(ColourGradient(Colour(0xff16243b), 0, 0, Colour(0xff34557c), 1024-250, 768-20, false));

    //g.setColour(Colour(0xff222222));
    //g.fillRect(Rectangle<int>(0, 0, 1024, 30));

    g.setFont (Font (16.0f));
    g.setColour (Colours::white);
    // Banner rotator

}

void CommunityComponent::resized() {

}
