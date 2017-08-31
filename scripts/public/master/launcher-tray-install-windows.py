import subutai
from time import sleep
import zipfile


def updateProgress(tray, libssh, total):
    cur = tray + libssh
    val = (int)(100 * cur) / total
    progress = (float)(val/100)
    subutai.SetProgress(progress)


def subutaistart():
    subutai.AddStatus("Download Tray application")

    trayFile = "SubutaiTray_libs.zip"
    sshlibFile = "ssh.zip"

    traySize = subutai.GetFileSize(trayFile)
    libsshSize = subutai.GetFileSize(sshlibFile)
    totalSize = traySize + libsshSize
    trayProgress = 0
    libsshProgress = 0

    subutai.download(trayFile)
    while subutai.isDownloadComplete() != 1:
        sleep(0.05)
        trayProgress = subutai.GetBytesDownload()
        updateProgress(trayProgress, libsshProgress, totalSize)

    trayProgress = traySize

    subutai.download(sshlibFile)
    while subutai.isDownloadComplete() != 1:
        sleep(0.05)
        libsshProgress = subutai.GetBytesDownload()
        updateProgress(trayProgress, libsshProgress, totalSize)

    libsshProgress = libsshSize

    tmpDir = subutai.GetTmpDir()
    installDir = subutai.GetInstallDir()

    subutai.AddStatus("Installing Tray")

    subutai.ProcessKill("SubutaiTray.exe")
    subutai.ProcessKill("ssh.exe")
    subutai.ProcessKill("ssh-keygen.exe")

    try:
        zf = zipfile.ZipFile(tmpDir+"/"+trayFile, 'r')
        zf.extractall(installDir)
        zf.close()
        zfl = zipfile.ZipFile(tmpDir+"/"+sshlibFile, 'r')
        zfl.extractall(installDir+"/bin")
        zfl.close()
    except:
        subutai.RaiseError("Failed to replace tray")

    subutai.CreateDesktopShortcut(installDir+"/tray/SubutaiTray.exe",
                                  "Subutai Tray")

    subutai.AddStatus("Writing configuration")
    unPath = installDir.replace('\\', '/')
    unVBoxPath = subutai.GetVBoxPath().replace('\\', '/')
    f = open(unPath+"/tray/subutai_tray.ini", "w")
    f.write("P2P_Path="+unPath+"/bin/p2p.exe\n")
    if unVBoxPath != "":
        f.write("VBoxManage_Path="+unVBoxPath+"\n")
    f.write("Ssh_Path="+unPath+"/bin/ssh.exe\n")
    f.write("Ssh_Keygen_Cmd="+unPath+"/bin/ssh-keygen.exe\n")
    f.close()

    subutai.Shutdown()

    return 0
