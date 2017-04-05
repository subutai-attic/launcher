import subutai
from time import sleep
from shutil import copyfile
import os
import stat
from subprocess import call
import tarfile


def subutaistart():
    subutai.AddStatus("Download Tray application")

    tray = "SubutaiTray_libs.tar.gz"
    trayBinary = "SubutaiTray_osx"
    libssh = "libssh2-1.6.0-0_osx.pkg"

    subutai.download(tray)
    while subutai.isDownloadComplete() != 1:
        sleep(0.05)

    subutai.download(trayBinary)
    while subutai.isDownloadComplete() != 1:
        sleep(0.05)

    tmpDir = subutai.GetTmpDir()
    installDir = subutai.GetInstallDir()

    subutai.AddStatus("Installing Tray")

    copyfile(tmpDir+"/"+trayBinary, installDir+"/bin/SubutaiTray")
    st = os.stat(installDir+"/bin/SubutaiTray")
    os.chmod(installDir+"/bin/SubutaiTray", st.st_mode | stat.S_IEXEC)

    tar = tarfile.open(tmpDir+"/"+tray, "r:gz")
    tar.extractall(installDir+"/bin/")
    tar.close()

    subutai.AddStatus("Creating Symlink")
    subutai.MakeLink(installDir+"/bin/SubutaiTray", "/usr/local/bin/SubutaiTray")

    subutai.AddStatus("Installing Tray dependencies")
    subutai.download(libssh)
    while subutai.isDownloadComplete() != 1:
        sleep(0.05)

    call(['installer', '-pkg', tmpDir+'/'+libssh, '-target', installDir])

    subutai.Shutdown()
