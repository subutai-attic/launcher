/*
   ==============================================================================

   This file was auto-generated!

   ==============================================================================
   */

#ifndef MAINCOMPONENT_H_INCLUDED
#define MAINCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "LoginScreen.h"
#include "LibraryComponent.h"
#include "CommunityComponent.h"
#include "HubComponent.h"
#include "MarketplaceComponent.h"
#include "Logo.h"
#include "Sidebar.h"

//==============================================================================
/*
   This component lives inside our window, and this is where you should put all
   your controls and content.
   */
class MainContentComponent : public juce::Component, public juce::ButtonListener, public juce::ListBoxModel
{
    public:
        static const int WINDOW_WIDTH = 1024;
        static const int WINDOW_HEIGHT = 768;
        static const int HEADER_HEIGHT = 22;
        static const int MENU_WIDTH = 250;

        //==============================================================================
        MainContentComponent();
        ~MainContentComponent();

        void paint (juce::Graphics&) override;
        void resized() override;

        void buttonClicked(juce::Button* button) override;

        // ListBoxModel overrides
        int getNumRows() override;
        void paintListBoxItem(int rowNumber, juce::Graphics& g, int width, int height, bool rowIsSelected) override;
        void selectedRowsChanged (int lastRowSelected) override;

    private:
        void showLoginScreen();
        //LoginScreen* _login;
        juce::ListBox _mainMenu;
        juce::Toolbar _header;

        // Right-side components
        LibraryComponent _library;
        HubComponent _hub;
        MarketplaceComponent _marketplace;
        CommunityComponent _community;

        juce::ImageButton *_closeButton;
        juce::ImageButton *_minimizeButton;

        Logo _logo;
        SidebarComponent _sidebar;

        std::vector<std::string> _menuItems;

        //==============================================================================
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};


#endif  // MAINCOMPONENT_H_INCLUDED
