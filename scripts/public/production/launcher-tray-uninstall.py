import subutai
from shutil import rmtree
import os
import stat

def subutaistart():
    subutai.CheckDirectories()

    traySource = "SubutaiTray_linux.tar.gz"
    trayBin = "SubutaiTray"
    trayDir = "tray"

    tmpDir = subutai.GetTmpDir()
    installDir = subutai.GetInstallDir()

    if os.path.isdir(installDir+"/bin/"+trayDir):
        rmtree(installDir+"/bin/"+trayDir)
    """
    if os.path.exists("/usr/bin/"+trayBin):
         if os.path.islink("/usr/bin/"+trayBin):
             os.unlink("/usr/bin/"+trayBin)
         else:
             os.remove("/usr/bin/"+trayBin)
    """
    if os.path.islink("/usr/bin/"+trayBin):
        os.unlink("/usr/bin/"+trayBin)

    subutai.Shutdown()
