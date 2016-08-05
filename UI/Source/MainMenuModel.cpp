/*
   ==============================================================================

   MainMenuModel.cpp
Created: 31 Jul 2016 11:32:02pm
Author:  crioto

==============================================================================
*/

#include "MainMenuModel.h"

MainMenuModel::MainMenuModel() {
    _menuItems.push_back("SUBUTAI");
    _menuItems.push_back("LIBRARY");
    _menuItems.push_back("TEMPLATES");
    _menuItems.push_back("YOUR HUB");
}

MainMenuModel::~MainMenuModel() {

}

int MainMenuModel::getNumRows() {
    return 4; // random
}

void MainMenuModel::paintListBoxItem(int rowNumber, Graphics& g, int width, int height, bool rowIsSelected) {
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
