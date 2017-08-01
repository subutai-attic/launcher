#include "Sidebar.h"
#include "MainComponent.h"

SidebarComponent::SidebarComponent() :
    _root(false)
{
    _logger = &Poco::Logger::get("subutai");
#if LAUNCHER_LINUX
    if (!getuid()) _root = true;
#endif
    _social = new SocialMediaIcons();
    _social->setBounds(0, getParentHeight()-20-40, getParentWidth(), 40);
    _social->toFront(true);
    addAndMakeVisible(_social);

    auto font = Font("Encode Sans", 17, 1);

    // Menu Items Labels
    _news.setText("News", juce::dontSendNotification);
    _news.setBounds(89, 191, 150, 40);
    _news.setFont(font);
    _news.setJustificationType(Justification::top);
    _news.setColour(Label::textColourId, juce::Colour::fromRGB(105, 116, 144));
    addAndMakeVisible(_news);

    _library.setText("Library", juce::dontSendNotification);
    _library.setBounds(89, 246, 150, 40);
    _library.setFont(font);
    _library.setJustificationType(Justification::top);
    _library.setColour(Label::textColourId, juce::Colour::fromRGB(105, 116, 144));
    addAndMakeVisible(_library);

    _templates.setText("Templates", juce::dontSendNotification);
    _templates.setBounds(89, 301, 150, 40);
    _templates.setFont(font);
    _templates.setJustificationType(Justification::top);
    _templates.setColour(Label::textColourId, juce::Colour::fromRGB(105, 116, 144));
    addAndMakeVisible(_templates);

    _community.setText("Community", juce::dontSendNotification);
    _community.setBounds(89, 356, 150, 40);
    _community.setFont(font);
    _community.setJustificationType(Justification::top);
    _community.setColour(Label::textColourId, juce::Colour::fromRGB(105, 116, 144));
    addAndMakeVisible(_community);

    // Menu Items Images
    std::string pDir = SubutaiLauncher::Session::instance()->getSettings()->getTmpPath();
    std::string pNewsActive = pDir + "launcher-news-active.png";
    std::string pNewsInactive = pDir + "launcher-news-inactive.png";
    std::string pLibraryActive = pDir + "launcher-library-active.png";
    std::string pLibraryInactive = pDir + "launcher-library-inactive.png";
    std::string pTemplatesActive = pDir + "launcher-templates-active.png";
    std::string pTemplatesInactive = pDir + "launcher-templates-inactive.png";
    std::string pCommunityActive = pDir + "launcher-community-active.png";
    std::string pCommunityInactive = pDir + "launcher-community-inactive.png";

    _logger->debug("Loading %s", pNewsActive);
    juce::ImageCache::getFromFile(juce::File(pNewsActive));
    _logger->debug("Loading %s", pNewsInactive);
    juce::ImageCache::getFromFile(juce::File(pNewsInactive));
    _logger->debug("Loading %s", pLibraryActive);
    juce::ImageCache::getFromFile(juce::File(pLibraryActive));
    _logger->debug("Loading %s", pLibraryInactive);
    juce::ImageCache::getFromFile(juce::File(pLibraryInactive));
    _logger->debug("Loading %s", pTemplatesActive);
    juce::ImageCache::getFromFile(juce::File(pTemplatesActive));
    _logger->debug("Loading %s", pTemplatesInactive);
    juce::ImageCache::getFromFile(juce::File(pTemplatesInactive));
    _logger->debug("Loading %s", pCommunityActive);
    juce::ImageCache::getFromFile(juce::File(pCommunityActive));
    _logger->debug("Loading %s", pCommunityInactive);
    juce::ImageCache::getFromFile(juce::File(pCommunityInactive));

    _newsActive.setImage(juce::ImageCache::getFromFile(juce::File(pNewsActive)));
    _newsActive.setBounds(55, 191, 20, 18);
    _newsInactive.setImage(juce::ImageCache::getFromFile(juce::File(pNewsInactive)));
    _newsInactive.setBounds(55, 191, 20, 18);

    _libraryActive.setImage(juce::ImageCache::getFromFile(juce::File(pLibraryActive)));
    _libraryActive.setBounds(55, 246, 20, 18);
    _libraryInactive.setImage(juce::ImageCache::getFromFile(juce::File(pLibraryInactive)));
    _libraryInactive.setBounds(55, 246, 20, 18);

    _templatesActive.setImage(juce::ImageCache::getFromFile(juce::File(pTemplatesActive)));
    _templatesActive.setBounds(55, 301, 20, 18);
    _templatesInactive.setImage(juce::ImageCache::getFromFile(juce::File(pTemplatesInactive)));
    _templatesInactive.setBounds(55, 301, 20, 18);

    _communityActive.setImage(juce::ImageCache::getFromFile(juce::File(pCommunityActive)));
    _communityActive.setBounds(55, 356, 20, 18);
    _communityInactive.setImage(juce::ImageCache::getFromFile(juce::File(pCommunityInactive)));
    _communityInactive.setBounds(55, 356, 20, 18);

    addAndMakeVisible(_newsActive);
    addAndMakeVisible(_libraryInactive);
    addAndMakeVisible(_templatesInactive);
    addAndMakeVisible(_communityInactive);

    addChildComponent(_newsInactive);
    addChildComponent(_libraryActive);
    addChildComponent(_templatesActive);
    addChildComponent(_communityActive);

    auto warningFont = juce::Font("Encode Sans", 17, 1);
    _rootWarning.setText("Warning!\n\nRunning as root may harm\nyour system!");
    _rootWarning.setJustification(juce::Justification::centred);
    _rootWarning.setBounds(10, 450, 240, 80);
    _rootWarning.setColour(Colours::white);
    _rootWarning.setFont(warningFont, true);
    _rootWarning.setBoundingBox(RelativeParallelogram(Point<float>(10, 450),
                Point<float>(240, 450),
                Point<float>(0, 540)));
    if (_root) addAndMakeVisible(_rootWarning);

    resized();
}

SidebarComponent::~SidebarComponent() {
    delete _social;
}

void SidebarComponent::paint(Graphics& g) {
    //g.setGradientFill(ColourGradient(Colour(0xff16243b), 0, 0, Colour(0xff34557c), 250, 768-20, false));
    //g.fillAll(Colour(0xff222222));
    //g.fillAll (Colour::greyLevel (0.2f));
    //g.fillAll(Colour::fromRGB(19, 31, 51));
    if (_root)
    {
        g.setColour(Colours::maroon);
        g.fillRoundedRectangle(5, 450, 240, 100, 4.0);
    }
}

void SidebarComponent::resized() {
    _social->setBounds(0, getParentHeight()-20-40, getParentWidth(), 40);
}

void SidebarComponent::activateNews()
{
    _newsActive.setVisible(true);
    _libraryActive.setVisible(false);
    _templatesActive.setVisible(false);
    _communityActive.setVisible(false);

    _newsInactive.setVisible(false);
    _libraryInactive.setVisible(true);
    _templatesInactive.setVisible(true);
    _communityInactive.setVisible(true);
}

void SidebarComponent::activateLibrary()
{
    _newsActive.setVisible(false);
    _libraryActive.setVisible(true);
    _templatesActive.setVisible(false);
    _communityActive.setVisible(false);

    _newsInactive.setVisible(true);
    _libraryInactive.setVisible(false);
    _templatesInactive.setVisible(true);
    _communityInactive.setVisible(true);
}

void SidebarComponent::activateTemplates()
{
    _newsActive.setVisible(false);
    _libraryActive.setVisible(false);
    _templatesActive.setVisible(true);
    _communityActive.setVisible(false);

    _newsInactive.setVisible(true);
    _libraryInactive.setVisible(true);
    _templatesInactive.setVisible(false);
    _communityInactive.setVisible(true);
}

void SidebarComponent::activateCommunity()
{
    _newsActive.setVisible(false);
    _libraryActive.setVisible(false);
    _templatesActive.setVisible(false);
    _communityActive.setVisible(true);

    _newsInactive.setVisible(true);
    _libraryInactive.setVisible(true);
    _templatesInactive.setVisible(true);
    _communityInactive.setVisible(false);
}
