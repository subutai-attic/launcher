import subutai
from time import sleep
from shutil import copyfile
import os
import stat
from subprocess import call
import tarfile


def subutaistart():
    tmpDir = subutai.GetTmpDir()
    installDir = subutai.GetInstallDir()

    if not os.path.exists(installDir+"bin/cocoasudo"):
        subutai.AddStatus("Downloading cocoasudo application")
        subutai.download("cocoasudo")
        while subutai.isDownloadComplete() != 1:
            sleep(0.05)

        try:
            copyfile(tmpDir+"cocoasudo", installDir+"bin/cocoasudo")
            st = os.stat(installDir+"bin/cocoasudo")
            os.chmod(installDir+"bin/cocoasudo", st.st_mode | stat.S_IEXEC)
        except:
            subutai.RaiseError("Failed to install cocoasudo. Aborting")
            sleep(10)
            return -99

    subutai.AddStatus("Download Tray application")

    tray = "SubutaiTray_libs_osx.tar.gz"
    libssh = "libssh2-1.6.0-0_osx.pkg"

    subutai.download(tray)
    while subutai.isDownloadComplete() != 1:
        sleep(0.05)

    subutai.AddStatus("Installing Tray")

    try:
        tar = tarfile.open(tmpDir+"/"+tray, "r:gz")
        tar.extractall("/Applications/Subutai")
        tar.close()
    except:
        subutai.RaiseError("Failed to unpack Tray archive. Aborting")
        sleep(10)
        return 86

    subutai.AddStatus("Installing Tray dependencies")
    subutai.download(libssh)
    while subutai.isDownloadComplete() != 1:
        sleep(0.05)

    try:
        call([installDir+'bin/cocoasudo',
              '-prompt="Install libssh"',
              'installer',
              '-pkg',
              tmpDir+'/'+libssh,
              '-target',
              '/'])
    except:
        subutai.RaiseError("Failed to install libssh. Aborting")
        sleep(10)
        return -99

    subutai.Shutdown()

    return 0
