import subutai
import subuco
if subutai.GetOS() == 'w':
    import subuw as subup
elif subutai.GetOS() == 'l':
    import subul as subup
elif subutai.GetIS() == 'd':
    import subud as subup


def subutaistart():
    tmpDir = subutai.GetTmpDir()
    installDir = subutai.GetInstallDir()

    rc = 0

    tray = subup.Tray(tmpDir, installDir)
    rc = tray.PreInstall()
    if rc != 0:
        sleep(10)
        subutai.Shutdown()
        return rc
    rc = tray.Download()
    if rc != 0:
        sleep(10)
        subutai.Shutdown()
        return rc
    rc = tray.install()
    if rc != 0:
        sleep(10)
        subutai.Shutdown()
        return rc
    rc = tray.PostInstall()
    if rc != 0:
        sleep(10)
        subutai.Shutdown()
        return rc

    subutai.Shutdown()

    return 0
