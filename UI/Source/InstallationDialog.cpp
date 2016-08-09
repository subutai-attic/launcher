/*
  ==============================================================================

    InstallationDialog.cpp
    Created: 8 Aug 2016 1:21:29pm
    Author:  crioto

  ==============================================================================
*/

#include "InstallationDialog.h"

InstallationDialog::InstallationDialog(const String& name, Colour backgroundColour, int buttonsNeeded) :
    DocumentWindow (name, backgroundColour, buttonsNeeded)
{

}

InstallationDialog::~InstallationDialog() {

}

void InstallationDialog::closeButtonPressed() {
    delete this;
}
