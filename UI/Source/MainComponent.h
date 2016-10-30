/*
   ==============================================================================

   This file was auto-generated!

   ==============================================================================
   */

#ifndef MAINCOMPONENT_H_INCLUDED
#define MAINCOMPONENT_H_INCLUDED

#include "Core.h"

#include "../JuceLibraryCode/JuceHeader.h"
#include "LoginScreen.h"
#include "SubutaiLauncher.h"
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
class MainContentComponent : public Component, public ButtonListener, public ListBoxModel
{
    public:
        static const int WINDOW_WIDTH = 1024;
        static const int WINDOW_HEIGHT = 768;
        static const int HEADER_HEIGHT = 22;
        static const int MENU_WIDTH = 250;

        //==============================================================================
        MainContentComponent();
        ~MainContentComponent();

        void paint (Graphics&) override;
        void resized() override;

        void buttonClicked(Button* button) override;

        // ListBoxModel overrides
        int getNumRows() override;
        void paintListBoxItem(int rowNumber, Graphics& g, int width, int height, bool rowIsSelected) override;
        void selectedRowsChanged (int lastRowSelected) override;

    private:
        void showLoginScreen();
        //LoginScreen* _login;
        ListBox _mainMenu;
        Toolbar _header;

        // Right-side components
        LibraryComponent _library;
        HubComponent _hub;
        MarketplaceComponent _marketplace;
        CommunityComponent _community;

        ImageButton *_closeButton;
        ImageButton *_minimizeButton;

        Logo _logo;
        SidebarComponent _sidebar;

        // Launcher
        SubutaiLauncher::Core* _core;

        std::vector<std::string> _menuItems;

        //==============================================================================
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};


#endif  // MAINCOMPONENT_H_INCLUDED
