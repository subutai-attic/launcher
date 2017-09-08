import subutai
import subuco
if subutai.GetOS() == 'w':
    import subuw as subup
elif subutai.GetOS() == 'l':
    import subul as subup
elif subutai.GetOS() == 'd':
    import subud as subup



def subutaistart():
    tmpDir = subutai.GetTmpDir()
    installDir = subutai.GetInstallDir()

    tray = subup.Tray(tmpDir, installDir)
    tray.PreInstall()
    tray.Download()
    tray.PostInstall()

    subutai.Shutdown()

    return 0

