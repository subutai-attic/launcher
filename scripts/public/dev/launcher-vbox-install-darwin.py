import subutai
from time import sleep
from shutil import copyfile
import os
import stat
from subprocess import call
import tarfile


def subutaistart():
    subutai.AddStatus("Download VirtualBox")

    vbox = "VirtualBox_osx.pkg"

    subutai.download(vbox)
    while subutai.isDownloadComplete() != 1:
        sleep(0.05)

    tmpDir = subutai.GetTmpDir()
    installDir = subutai.GetInstallDir()

    subutai.AddStatus("Installing VirtualBox")

    tar = tarfile.open(tmpDir+"/"+tray, "r:gz")
    tar.extractall("/Applications/Subutai")
    tar.close()

    subutai.AddStatus("Installing Tray dependencies")
    subutai.download(libssh)
    while subutai.isDownloadComplete() != 1:
        sleep(0.05)

    call(['installer', '-pkg', tmpDir+'/'+libssh, '-target', installDir])

    subutai.Shutdown()
