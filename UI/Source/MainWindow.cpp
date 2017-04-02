#include "MainWindow.h"

MainWindow::MainWindow(juce::String name) : juce::DocumentWindow(
        name,
        juce::Colours::lightgrey,
        juce::DocumentWindow::allButtons
        )
{
    //setUsingNativeTitleBar (false);
    //setTitleBarHeight(0);
    setResizeLimits(800, 640, 0, 0);
    setUsingNativeTitleBar(true);
    MainContentComponent* mainComponent = new MainContentComponent();
    setContentOwned(mainComponent, true);

    centreWithSize(getWidth(), getHeight());
    setDraggable(true);
    setVisible(true);
    setResizable(true, true);
    toFront(true);
}

void MainWindow::closeButtonPressed()
{
    juce::JUCEApplication::getInstance()->systemRequestedQuit();
}
