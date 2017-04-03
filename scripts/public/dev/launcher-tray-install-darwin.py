import subutai
from time import sleep
# from shutil import copyfile
# from shutil import rmtree
# import os
# import stat
from subprocess import call
import tarfile


def subutaistart():
    subutai.AddStatus("Download Tray application")

    tray = "SubutaiTray_macos.tar.gz"
    libssh = "libssh2-1.6.0-0_osx.pkg"

    subutai.download(tray)
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

    subutai.AddStatus("Installing Tray dependencies")
    subutai.download(libssh)
    while subutai.isDownloadComplete() != 1:
        sleep(0.05)

    call(['installer', '-pkg', tmpDir+'/'+libssh, '-target', installDir])

    subutai.Shutdown()
