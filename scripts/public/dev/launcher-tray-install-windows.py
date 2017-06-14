import subutai
from time import sleep
import os
import zipfile


def subutaistart():
    subutai.AddStatus("Download Tray application")

    tray = "SubutaiTray_libs.zip"
    sshlib = "ssh.zip"

    subutai.download(tray)
    while subutai.isDownloadComplete() != 1:
        sleep(0.05)

    subutai.download(sshlib)
    while subutai.isDownloadComplete() != 1:
        sleep(0.05)

    tmpDir = subutai.GetTmpDir()
    installDir = subutai.GetInstallDir()

    subutai.AddStatus("Installing Tray")

    zf = zipfile.ZipFile(tmpDir+"/"+tray, 'r')
    zf.extractall(installDir)
    zf.close()

    zfl = zipfile.ZipFile(tmpDir+"/"+sshlib, 'r')
    zfl.extractall(installDir+"/bin")
    zfl.close()

    subutai.CreateDesktopShortcut(installDir+"/tray/SubutaiTray.exe",
                                  "Subutai Tray")

    subutai.Shutdown()

    return 0
