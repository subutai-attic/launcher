import subutai

def subutaistart():
    subutai.NewConfiguration("Tray Client")
    subutai.SetConfigurationDesc("Tray Client", "Tray application is used to connect to your peers via SSH. Along with it a p2p client will be installed")
    subutai.SetConfigurationFile("Tray Client", "tray_install_linux")

    subutai.NewConfiguration("P2P")
    subutai.SetConfigurationDesc("P2P", "A peer-to-peer application that helps you to connect to your peers")
    subutai.SetConfigurationFile("P2P", "p2p_install_linux")

    subutai.NewConfiguration("Browser Plugin")
    subutai.SetConfigurationDesc("Browser Plugin", "Plugin to manage your peers")
    subutai.SetConfigurationFile("Browser Plugin", "e2e_install_linux")

    return 0

