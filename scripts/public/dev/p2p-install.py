import subutai
import subuco
if subutai.GetOS() == 'w':
    import subuw as subup
elif subutai.GetOS() == 'l':
    import subul as subup
elif subutai.GetOS() == 'd':
    import subud as subup
from time import sleep
from shutil import copyfile
import os
import stat
from subprocess import Popen, PIPE


def subutaistart():
    rc = 0
    tmpDir = subutai.GetTmpDir()
    installDir = subutai.GetInstallDir()

    p2p = subup.P2P(tmpDir, installDir)
    rc = p2p.PreInstall()
    if rc != 0:
        sleep(10)
        subutai.Shutdown()
        return rc
    rc = p2p.Download()
    if rc != 0:
        sleep(10)
        subutai.Shutdown()
        return rc
    rc = p2p.PostInstall()
    if rc != 0:
        sleep(10)
        subutai.Shutdown()
        return rc
    subutai.SetProgress(1.00)

    sleep(3)
    subutai.Shutdown()

    return 0
