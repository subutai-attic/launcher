import subutai

def subutaistart():
    subutai.NewConfiguration("Latest Tray Client")
    subutai.SetConfigurationDesc("Latest Tray Client", "Tray application is used to connect to your peers via SSH")
    subutai.SetConfigurationFile("Latest Tray Client", "tray_p2p_install")

    subutai.NewConfiguration("Tray Client And Resource Host")
    subutai.SetConfigurationDesc("Tray Client And Resource Host", "Resource host will allow you to run Subutai inside a Virtual Machine on your computer")
    subutai.SetConfigurationFile("Tray Client And Resource Host", "tray_p2p_peer_install")

    return 0

