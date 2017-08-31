#include "LoginScreen.h"

LoginScreen::LoginScreen() : 
    _button("Login"), 
    _password("Password", (juce::juce_wchar) 0x2022),
    _skipButton("Skip")
{
    _logger = &Poco::Logger::get("subutai");
    auto font = Font("Encode Sans", 17, 1);
    auto fontVersion = juce::Font("Encode Sans", 15, 1);
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

    _registerLink.setURL(URL("https://"+SubutaiLauncher::Hub::URL+"/register"));
    _registerLink.setFont(linkFont, false);
    _registerLink.setButtonText("Create Account");
    _registerLink.setBounds(getWidth() / 2 - (INPUT_FIELD_WIDTH / 2), 300, 200, 20);

    addAndMakeVisible(_resetLink);
    addAndMakeVisible(_registerLink);

    _title.setText("Subutai Launcher", dontSendNotification);
    _title.setColour(Label::textColourId, juce::Colour(105, 116, 144));
    _title.setBounds(0, 110, 250, 25);
    _title.setFont(font);
    _title.setJustificationType(Justification::centred);
    addAndMakeVisible(_title);

    _version.setText("Version 5.0.0", dontSendNotification);
    _version.setColour(Label::textColourId, juce::Colour(105, 116, 144));
    _version.setBounds(0, 130, 250, 25);
    _version.setFont(fontVersion);
    _version.setJustificationType(Justification::centred);
    addAndMakeVisible(_version);

    _error.setText("Email or password is invalid", dontSendNotification);
    _error.setColour(Label::textColourId, juce::Colours::red);
    _error.setBounds(0, 0, 0, 0);
    _error.setFont(font);
    _error.setJustificationType(Justification::centred);
    addChildComponent(_error);
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
    _error.setBounds(getWidth() / 2 - (INPUT_FIELD_WIDTH / 2), 190, INPUT_FIELD_WIDTH, 30);
    _login.setBounds(getWidth() / 2 - (INPUT_FIELD_WIDTH / 2), 220, INPUT_FIELD_WIDTH, 30);
    _password.setBounds(getWidth() / 2 - (INPUT_FIELD_WIDTH / 2), 270, INPUT_FIELD_WIDTH, 30);
    _logo.setBounds(getWidth() / 2 - pLogoWidth + 30, 40, pLogoWidth, pLogoHeight);
    _userIcon.setBounds(getWidth() / 2 - (INPUT_FIELD_WIDTH / 2) - 30, 230, 20, 20);
    _passwordIcon.setBounds(getWidth() / 2 - (INPUT_FIELD_WIDTH / 2) - 30, 280, 20, 20);
    _button.setBounds(getWidth() / 2 - (_button.getWidth() / 2), 320, _button.getWidth(), 24);
    _registerLink.setBounds(getWidth() / 2 - (INPUT_FIELD_WIDTH / 2), 375, 200, 20);
    _resetLink.setBounds(getWidth() / 2 - (INPUT_FIELD_WIDTH / 2), 395, 200, 20);
    _title.setBounds(getWidth() / 2 - 150, 110, 300, 25);
    _version.setBounds(getWidth() / 2 - 150, 130, 300, 25);
    _skipButton.setBounds(getWidth() - _skipButton.getWidth() - 20, getHeight() - 44, _skipButton.getWidth(), 24);
}

void LoginScreen::buttonClicked(juce::Button* button) 
{
    if (button == &_button) 
    {
        _button.setEnabled(false);
        _error.setVisible(false);
        // Loggin user in
        SubutaiLauncher::Hub* _pHub = SubutaiLauncher::Session::instance()->getHub();
        _pHub->setLogin(_login.getText().toStdString());
        _pHub->setPassword(_password.getText().toStdString());
        bool rc = _pHub->auth();
        if (rc)
        {
            setVisible(false);
        }
        else
        {
            _error.setVisible(true);
            _button.setEnabled(true);
        }
    }
    else if (button == &_skipButton)
    {
        juce::String title = "Skip login?";
        juce::String message = "Although optional, we highly recommend logging into your Hub account to track the installation process due to several advantages:\n\n" \
        "1.    Every system's hardware resources are different. Each fits into a specific resource grade. The resource grades are important for resource comparison and your peer's reputation. Resource grade determination takes place at installation time and is associated with your peer on the Hub.\n" \
        "2.    Every software configuration is different and several problems may result. Logging these from cradle to grave with analytics help to automatically determine better installation options during installation and afterwards for maintenance. Having a cradle to grave view of your peer and client configuration is key.\n" \
        "3.    This information helps us to provide better options and services for you and others that might use your resources. It helps to improve the overall system.\n\n" \
        "Only the resource grade (A-F) of your peer and its resource hosts are made available on the Hub if and only if you choose to make your peer visible to friends or the public. Besides this information, all the installation logs are deemed private information associated with your peer. In no way shape or form will it be shared or sold to any other entity. Please see our privacy bylaws on the Optimal Dynamics website.";

        if (NativeMessageBox::showOkCancelBox(AlertWindow::QuestionIcon, title,
                    message, nullptr, nullptr)) {
            setVisible(false);
        }
    }
}

