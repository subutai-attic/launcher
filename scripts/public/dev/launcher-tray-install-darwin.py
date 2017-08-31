import subutai
from time import sleep
from shutil import copyfile
import os
import stat
from subprocess import call
from subprocess import Popen, PIPE
import tarfile


def updateProgress(cocoasudo, tray, libssh, total):
    cur = cocoasudo + tray + libssh
    val = (int)(100 * cur) / total
    progress = (float)(val/100)
    subutai.SetProgress(progress)


def subutaistart():

    tmpDir = subutai.GetTmpDir()
    installDir = subutai.GetInstallDir()

    trayFile = "SubutaiTray_libs_osx.tar.gz"
    libsshFile = "libssh2-1.6.0-0_osx.pkg"
    cocoasudoFile = "cocoasudo"

    cocoasudoSize = subutai.GetFileSize(cocoasudoFile)
    traySize = subutai.GetFileSize(trayFile)
    libsshSize = subutai.GetFileSize(libsshFile)
    totalSize = cocoasudoSize + traySize + libsshSize
    cocoasudoProgress = 0
    trayProgress = 0
    libsshProgress = 0

    if not os.path.exists(installDir+"bin/cocoasudo"):
        subutai.AddStatus("Downloading cocoasudo application")
        subutai.download(cocoasudoFile)
        while subutai.isDownloadComplete() != 1:
            sleep(0.05)
            cocoasudoProgress = subutai.GetBytesDownload()
            updateProgress(cocoasudoProgress, trayProgress, libsshProgress,
                           totalSize)

        try:
            copyfile(tmpDir+cocoasudoFile, installDir+"bin/cocoasudo")
            st = os.stat(installDir+"bin/cocoasudo")
            os.chmod(installDir+"bin/cocoasudo", st.st_mode | stat.S_IEXEC)
        except:
            subutai.RaiseError("Failed to install cocoasudo. Aborting")
            sleep(10)
            return -99

    cocoasudoProgress = cocoasudoSize

    subutai.AddStatus("Download Tray application")

    subutai.download(trayFile)
    while subutai.isDownloadComplete() != 1:
        sleep(0.05)
        trayProgress = subutai.GetBytesDownload()
        updateProgress(cocoasudoProgress, trayProgress, libsshProgress,
                       totalSize)

    trayProgress = traySize

    subutai.AddStatus("Installing Tray")

    try:
        tar = tarfile.open(tmpDir+"/"+trayFile, "r:gz")
        tar.extractall("/Applications/Subutai")
        tar.close()
    except:
        subutai.RaiseError("Failed to unpack Tray archive. Aborting")
        sleep(10)
        return 86

    subutai.AddStatus("Installing Tray dependencies")
    subutai.download(libsshFile)
    while subutai.isDownloadComplete() != 1:
        sleep(0.05)
        libsshProgress = subutai.GetBytesDownload()
        updateProgress(cocoasudoProgress, trayProgress, libsshProgress,
                       totalSize)

    libsshProgress = libsshSize

    try:
        call([installDir+'bin/cocoasudo',
              '-prompt="Install libssh"',
              'installer',
              '-pkg',
              tmpDir+'/'+libsshFile,
              '-target',
              '/'])
    except:
        subutai.RaiseError("Failed to install libssh. Aborting")
        sleep(10)
        return -99

    try:
        launch = 'tell application "SubutaiTray.app" to activate'
        p = Popen(['osascript', '-'], stdin=PIPE, stdout=PIPE, stderr=PIPE, universal_newlines=True)
        stdout, stderr = p.communicate(launch)
    except:
        subutai.RaiseError("Failed to launch Subutai Tray application")

    subutai.Shutdown()

    return 0

