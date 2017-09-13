import subutai
import subuco
from time import sleep
if subutai.GetOS() == 'w':
    import subuw as subup
elif subutai.GetOS() == 'l':
    import subul as subup
elif subutai.GetOS() == 'd':
    import subud as subup


def subutaistart():
    tmpDir = subutai.GetTmpDir()
    installDir = subutai.GetInstallDir()

    rc = 0

    e2e = subup.E2E(tmpDir, installDir)
    rc = e2e.PreInstall()
    if rc != 0:
        sleep(10)
        subutai.Shutdown()
        return rc
    rc = e2e.Download()
    if rc != 0:
        sleep(10)
        subutai.Shutdown()
        return rc
    rc = e2e.Install()
    if rc != 0:
        sleep(10)
        subutai.Shutdown()
        return rc
    rc = e2e.PostInstall()
    if rc != 0:
        sleep(10)
        subutai.Shutdown()
        return rc

    subutai.Shutdown()
    return 0