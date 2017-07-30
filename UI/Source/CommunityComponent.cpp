#include "CommunityComponent.h"

CommunityComponent::CommunityComponent() {
    _logger = &Poco::Logger::get("subutai");
    _logger->trace("CommunityComponent::CommunityComponent()");

    // Download launcher-front-page.json
    //auto d = SubutaiLauncher::Session::instance()->getDownloader();
    //d.setFilename("launcher-front-page.json");
}

CommunityComponent::~CommunityComponent() {
}

void CommunityComponent::paint(Graphics& g) {
    //g.fillAll (Colour (0xffffffff));
    g.setFont (Font (16.0f));
    g.setColour (Colours::white);
}

void CommunityComponent::resized() {

}
