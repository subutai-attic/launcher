#include "LoginScreen.h"

LoginScreen::LoginScreen() : 
    _button("Login"), 
    _password("Password", (juce::juce_wchar) 0x2022),
    _skipButton("Skip")
{
    _logger = &Poco::Logger::get("subutai");
    auto font = Font("Encode Sans", 17, 1);
    addAndMakeVisible(_login);
    _login.setBounds(10, 50, 180, 24);
    _login.setColour(juce::TextEditor::outlineColourId, juce::Colour::fromRGB(247, 249, 252));
    _login.setColour(juce::TextEditor::backgroundColourId, juce::Colour::fromRGB(247, 249, 252));
    _login.setColour(juce::TextEditor::focusedOutlineColourId, juce::Colour::fromRGB(247, 249, 252));
    _login.setFont(font);
    _login.setText("");

    addAndMakeVisible(_password);
    _password.setBounds(10, 85, 180, 24);
    _password.setColour(juce::TextEditor::outlineColourId, juce::Colour::fromRGB(247, 249, 252));
    _password.setColour(juce::TextEditor::backgroundColourId, juce::Colour::fromRGB(247, 249, 252));
    _password.setColour(juce::TextEditor::focusedOutlineColourId, juce::Colour::fromRGB(247, 249, 252));
    _password.setFont(font);
    _password.setText("");

    addAndMakeVisible(_button);
    _button.addListener(this);
    _button.setBounds(10, 105, 180, 24);

    addAndMakeVisible(_skipButton);
    _skipButton.addListener(this);

    std::string pLogoFile(SubutaiLauncher::Session::instance()->getSettings()->getTmpPath() + "launcher-logo.png");
    std::string pUserFile(SubutaiLauncher::Session::instance()->getSettings()->getTmpPath() + "user.png");
    std::string pPassFile(SubutaiLauncher::Session::instance()->getSettings()->getTmpPath() + "password.png");

    Poco::File pLogo(pLogoFile);
    if (pLogo.exists())
    {
        _logo.setImage(juce::ImageCache::getFromFile(juce::File(pLogoFile)));
    }
    else
    {
        _logger->error("launcher-logo.png doesn't exists");
    }
    addAndMakeVisible(_logo);

    Poco::File pUser(pUserFile);
    if (pUser.exists())
    {
        _userIcon.setImage(juce::ImageCache::getFromFile(juce::File(pUserFile)));
    }
    else
    {
        _logger->error("user.png doesn't exists");
    }
    addAndMakeVisible(_userIcon);

    Poco::File pPass(pPassFile);
    if (pPass.exists())
    {
        _passwordIcon.setImage(juce::ImageCache::getFromFile(juce::File(pPassFile)));
    }
    else
    {
        _logger->error("password.png doesn't exists");
    }
    addAndMakeVisible(_passwordIcon);

    auto linkFont = juce::Font("Encode Sans", 14, 0);
    _resetLink.setURL(URL("https://hub.subut.ai/account-recovery"));
    _resetLink.setFont(linkFont, false);
    _resetLink.setButtonText("Reset Password");
    _resetLink.setBounds(getWidth() / 2 - (INPUT_FIELD_WIDTH / 2), 300, 200, 20);


    _registerLink.setURL(URL("https://hub.subut.ai/register"));
    _registerLink.setFont(linkFont, false);
    _registerLink.setButtonText("Create Account");
    _registerLink.setBounds(getWidth() / 2 - (INPUT_FIELD_WIDTH / 2), 300, 200, 20);

    addAndMakeVisible(_resetLink);
    addAndMakeVisible(_registerLink);
}

LoginScreen::~LoginScreen() 
{

}

void LoginScreen::paint(juce::Graphics& g) 
{
    g.fillAll(juce::Colour::fromRGB(247, 249, 252));
    g.setColour(juce::Colour::fromRGB(101, 117, 141));
    g.drawLine(getWidth() / 2 - (INPUT_FIELD_WIDTH / 2), 252, getWidth() / 2 + (INPUT_FIELD_WIDTH / 2), 252, 2);
    g.drawLine(getWidth() / 2 - (INPUT_FIELD_WIDTH / 2), 302, getWidth() / 2 + (INPUT_FIELD_WIDTH / 2), 302, 2);
}

void LoginScreen::resized()
{
    int pLogoWidth = 64;
    int pLogoHeight = 64;
    pLogoWidth = _logo.getImage().getWidth();
    pLogoHeight = _logo.getImage().getHeight();
    _login.setBounds(getWidth() / 2 - (INPUT_FIELD_WIDTH / 2), 220, INPUT_FIELD_WIDTH, 30);
    _password.setBounds(getWidth() / 2 - (INPUT_FIELD_WIDTH / 2), 270, INPUT_FIELD_WIDTH, 30);
    _logo.setBounds(getWidth() / 2 - pLogoWidth + 30, 40, pLogoWidth, pLogoHeight);
    _userIcon.setBounds(getWidth() / 2 - (INPUT_FIELD_WIDTH / 2) - 30, 230, 20, 20);
    _passwordIcon.setBounds(getWidth() / 2 - (INPUT_FIELD_WIDTH / 2) - 30, 280, 20, 20);
    _button.setBounds(getWidth() / 2 - (_button.getWidth() / 2), 320, _button.getWidth(), 24);
    _registerLink.setBounds(getWidth() / 2 - (INPUT_FIELD_WIDTH / 2), 375, 200, 20);
    _resetLink.setBounds(getWidth() / 2 - (INPUT_FIELD_WIDTH / 2), 395, 200, 20);
}

void LoginScreen::buttonClicked(juce::Button* button) 
{
    if (button == &_button) 
    {
        //_button.setVisible(false);
        removeFromDesktop();
    }
    else if (button == &_skipButton)
    {
        juce::String title = "Skip login?";
        juce::String message = "Are you sure you want to not login into Hub? We will use your login details";

        if (NativeMessageBox::showOkCancelBox(AlertWindow::QuestionIcon, title,
                    message, nullptr, nullptr)) {
        }
    }
}
