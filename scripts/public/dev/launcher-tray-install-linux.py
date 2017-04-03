import subutai
from time import sleep
# from shutil import copyfile
# from shutil import rmtree
#import os
# import stat
import tarfile

def subutaistart():
    subutai.AddStatus("Download Tray application")

    tray = "SubutaiTray_macos.tar.gz"

    subutai.download(tray)
    while subutai.isDownloadComplete() != 1:
        sleep(0.05)

    subutai.download("libssh2-1.6.0-0_osx.pkg")
    while subutai.isDownloadComplete() != 1:
        sleep(0.05)

    tmpDir = subutai.GetTmpDir()
    installDir = subutai.GetInstallDir()

    subutai.AddStatus("Unpacking Tray")

    tar = tarfile.open(tmpDir+"/"+tray, "r:gz")
    tar.extractAll(installDir+"/bin/")
    tar.close()

    subutai.AddStatus("Creating Symlink")
    subutai.MakeLink(installDir+"/bin/SubutaiTray", "/usr/local/bin/subutai-tray")

    subutai.Shutdown()
