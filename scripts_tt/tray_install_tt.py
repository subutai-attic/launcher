import subutai
from time import sleep
from shutil import copyfile
from shutil import rmtree
import os
import stat
import tarfile

def subutaistart():
    subutai.CheckDirectories()

    traySource = "SubutaiTray_linux.tar.gz"
    trayBin = "SubutaiTray"
    trayDir = "tray"

    subutai.download(traySource)
    while subutai.isDownloadComplete() != 1:
        sleep(0.05)

    tmpDir = subutai.GetTmpDir()
    installDir = subutai.GetInstallDir()

    if not os.path.isdir(installDir+"/bin"):
        os.makedirs(installDir+"/bin")

    if os.path.exists(installDir+"/bin/"+trayDir):
        rmtree(installDir+"/bin/"+trayDir)

    """
    if os.path.exists("/usr/bin/"+trayBin):
         if os.path.isfile("/usr/bin/"+trayBin):
              os.remove("/usr/bin/"+trayBin)
         else:
              os.unlink("/usr/bin/"+trayBin)
    """
    if os.path.islink("/usr/bin/"+trayBin):
        os.unlink("/usr/bin/"+trayBin)

    if os.path.exists(tmpDir+"/"+traySource):
        tar = tarfile.open(tmpDir+"/"+traySource, "r:gz")
        tar.extractall(installDir+"/bin/"+trayDir)
        tar.close()
        os.remove(tmpDir+"/"+traySource)
        if not os.path.exists(installDir+"/bin/"+trayDir+"/bin/"+"trayBinin/"+trayBin):
            os.symlink(installDir+"/bin/"+trayDir+"/bin/"+trayBin, "/usr/bin/"+trayBin)

    subutai.Shutdown()
