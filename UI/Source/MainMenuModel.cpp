/*
  ==============================================================================

    MainMenuModel.cpp
    Created: 31 Jul 2016 11:32:02pm
    Author:  crioto

  ==============================================================================
*/

#include "MainMenuModel.h"

MainMenuModel::MainMenuModel() {

}

MainMenuModel::~MainMenuModel() {

}

int MainMenuModel::getNumRows() {
    return 4; // random
}

void MainMenuModel::paintListBoxItem(int rowNumber, Graphics& g, int width, int height, bool rowIsSelected) {
    g.drawText ("Hello World!", Rectangle<int> (width, height).reduced (4, 0), Justification::centred, true);
}
