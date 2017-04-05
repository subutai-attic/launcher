#include "MainWindow.h"

MainWindow::MainWindow(juce::String name) : juce::DocumentWindow(
        name,
        juce::Colours::lightgrey,
        juce::DocumentWindow::allButtons
        )
{
    //setUsingNativeTitleBar (false);
    //setTitleBarHeight(0);
    auto display = juce::Desktop::getInstance().getDisplays().getMainDisplay();
    auto ua = display.userArea;
    setResizeLimits(800, 640, ua.getWidth(), ua.getHeight());
    setUsingNativeTitleBar(true);
    MainContentComponent* mainComponent = new MainContentComponent();
    setContentOwned(mainComponent, true);

    centreWithSize(getWidth(), getHeight());
    setDraggable(true);
    setVisible(true);
    setResizable(true, true);
    toFront(true);
    setSize(960, 720);
}

void MainWindow::closeButtonPressed()
{
    juce::JUCEApplication::getInstance()->systemRequestedQuit();
}
