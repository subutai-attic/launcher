import subutai
from time import sleep
from shutil import copyfile
import os
import stat
from subprocess import call
import tarfile


def subutaistart():
    subutai.AddStatus("Download Tray application")

    tray = "SubutaiTray_libs_osx.tar.gz"
    libssh = "libssh2-1.6.0-0_osx.pkg"

    subutai.download(tray)
    while subutai.isDownloadComplete() != 1:
        sleep(0.05)

    tmpDir = subutai.GetTmpDir()
    installDir = subutai.GetInstallDir()

    subutai.AddStatus("Installing Tray")

    st = os.stat(installDir+"/bin/SubutaiTray")
    os.chmod(installDir+"/bin/SubutaiTray", st.st_mode | stat.S_IEXEC)

    tar = tarfile.open(tmpDir+"/"+tray, "r:gz")
    tar.extractall("/Applications/Subutai")
    tar.close()

    subutai.AddStatus("Installing Tray dependencies")
    subutai.download(libssh)
    while subutai.isDownloadComplete() != 1:
        sleep(0.05)

    call(['installer', '-pkg', tmpDir+'/'+libssh, '-target', installDir])

    subutai.Shutdown()
