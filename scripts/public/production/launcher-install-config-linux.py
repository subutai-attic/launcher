import subutai

def subutaistart():
    subutai.NewConfiguration("Subutai Client")
    subutai.SetConfigurationDesc("Subutai Client", "Tray and P2P applications are used to connect to your peers via SSH")
    subutai.SetConfigurationFile("Subutai Client", "client_install_tt")

    subutai.NewConfiguration("Browser Plugin")
    subutai.SetConfigurationDesc("Browser Plugin", "Plugin to manage access to your peers")
    subutai.SetConfigurationFile("Browser Plugin", "chrome_e2e_install")

    subutai.NewConfiguration("VBox")
    subutai.SetConfigurationDesc("VBox", "Needed to manage your peers")
    subutai.SetConfigurationFile("VBox", "vbox_install_tt")

    subutai.NewConfiguration("RH only")
    subutai.SetConfigurationDesc("RH only", "Resource Host")
    subutai.SetConfigurationFile("RH only", "wiz_rh_install")

    subutai.NewConfiguration("RH + Client")
    subutai.SetConfigurationDesc("RH + Client", "Resource Host and Subutai Client")
    subutai.SetConfigurationFile("RH + Client", "wiz_rhc_install")

    subutai.NewConfiguration("MH: RH+MH+Client")
    subutai.SetConfigurationDesc("MH: RH+MH+Client", "Management Host and Subutai Client")
    subutai.SetConfigurationFile("MH: RH+MH+Client", "wiz_mh_install")

    subutai.NewConfiguration("Subutai Client only")
    subutai.SetConfigurationDesc("Subutai Client only", "Subutai Client")
    subutai.SetConfigurationFile("Subutai Client only", "wiz_sc_install")

    return 0

