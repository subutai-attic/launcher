import subutai
import tarfile
from time import sleep


def subutaistart():
    subutai.AddStatus("Download Tray application")

    tray = "SubutaiTray_libs.tar.gz"

    subutai.download(tray)
    while subutai.isDownloadComplete() != 1:
        sleep(0.05)

    tmpDir = subutai.GetTmpDir()
    installDir = subutai.GetInstallDir()

    subutai.AddStatus("Unpacking Tray")

    tar = tarfile.open(tmpDir+"/"+tray, "r:gz")
    tar.extractall(installDir+"/bin/")
    tar.close()

    subutai.AddStatus("Creating Symlink")
    subutai.MakeLink(installDir+"/bin/SubutaiTray", "/usr/local/bin/SubutaiTray")

    subutai.Shutdown()

    return 0
