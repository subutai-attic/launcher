/*
   ==============================================================================

   MainMenuModel.h
Created: 31 Jul 2016 11:32:02pm
Author:  crioto

==============================================================================
*/

#ifndef MAINMENUMODEL_H_INCLUDED
#define MAINMENUMODEL_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

class MainMenuModel : public ListBoxModel {
    public:
        MainMenuModel();
        ~MainMenuModel();
        int getNumRows();
        void paintListBoxItem(int rowNumber, Graphics& g, int width, int height, bool rowIsSelected);
    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainMenuModel);
};



#endif  // MAINMENUMODEL_H_INCLUDED
