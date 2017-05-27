import subutai
from time import sleep
from shutil import copyfile
import os
import stat
from subprocess import call
import zipfile


def subutaistart():
    subutai.AddStatus("Download Tray application")

    tray = "SubutaiTray_libs.zip"

    subutai.download(tray)
    while subutai.isDownloadComplete() != 1:
        sleep(0.05)

    tmpDir = subutai.GetTmpDir()
    installDir = subutai.GetInstallDir()

    subutai.AddStatus("Installing Tray")

    zf = zipfile.ZipFile(tmpDir+"/"+tray, 'r')
    zf.extractall(installDir)
    zf.close()
 
    subutai.Shutdown()
