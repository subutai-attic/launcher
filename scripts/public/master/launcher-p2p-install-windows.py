import subutai
from time import sleep
from shutil import copyfile
from subprocess import call
import os
import stat


def updateProgress(nssm, p2p, tuntap, total):
    cur = nssm + p2p + tuntap
    val = (int)(100 * cur) / total
    progress = (float)(val/100)
    subutai.SetProgress(progress)


def subutaistart():

    tmpDir = subutai.GetTmpDir()
    installDir = subutai.GetInstallDir()

    nssmSize = subutai.GetFileSize("nssm.exe")
    p2pSize = subutai.GetFileSize("p2p.exe")
    tuntapSize = subutai.GetFileSize("tap-windows-9.21.2.exe")
    totalSize = nssmSize + p2pSize + tuntapSize
    nssmProgress = 0
    p2pProgress = 0
    tuntapProgress = 0

    subutai.AddStatus("Installing Service Manager")

    if os.path.exists(installDir+"nssm.exe"):
        subutai.UnregisterService("Subutai P2P")
        sleep(30)
        subutai.ProcessKill("nssm.exe")
        subutai.download("nssm.exe")
        while subutai.isDownloadComplete() != 1:
            sleep(0.05)
            nssmProgress = subutai.GetBytesDownload()
            updateProgress(nssmProgress, p2pProgress, tuntapProgress, totalSize)

        copyfile(tmpDir+"nssm.exe", installDir+"bin/nssm.exe")

    if not os.path.exists(installDir+"nssm.exe"):
        subutai.download("nssm.exe")
        while subutai.isDownloadComplete() != 1:
            sleep(0.05)
            nssmProgress = subutai.GetBytesDownload()
            updateProgress(nssmProgress, p2pProgress, tuntapProgress, totalSize)

        subutai.UnregisterService("Subutai P2P")
        sleep(30)
        subutai.ProcessKill("nssm.exe")
        copyfile(tmpDir+"nssm.exe", installDir+"bin/nssm.exe")

    nssmProgress = nssmSize

    subutai.AddStatus("Downloading p2p binary")
    subutai.download("p2p.exe")
    while subutai.isDownloadComplete() != 1:
        sleep(0.05)
        p2pProgress = subutai.GetBytesDownload()
        updateProgress(nssmProgress, p2pProgress, tuntapProgress, totalSize)

    p2pProgress = p2pSize

    subutai.AddStatus("Downloading extra tools")
    subutai.download("tap-windows-9.21.2.exe")
    while subutai.isDownloadComplete() != 1:
        sleep(0.05)
        tuntapProgress = subutai.GetBytesDownload()
        updateProgress(nssmProgress, p2pProgress, tuntapProgress, totalSize)

    tuntapProgress = tuntapSize

    subutai.ProcessKill("p2p.exe")
    subutai.AddStatus("Installing P2P")
    if os.path.exists(installDir+"bin/p2p.exe"):
        subutai.AddStatus("Removing previous installation")
        os.remove(installDir+"bin/p2p.exe")

    if os.path.exists(installDir+"bin/p2p.exe"):
        subutai.RaiseError("Failed to remove p2p binary")

    subutai.AddStatus("Copying files")
    copyfile(tmpDir+"p2p.exe", installDir+"bin/p2p.exe")

    st = os.stat(installDir+"bin/p2p.exe")
    os.chmod(installDir+"bin/p2p.exe", st.st_mode | stat.S_IEXEC)

    subutai.AddStatus("Registering P2P as a Windows service")
    subutai.RegisterService("Subutai P2P", installDir+"bin/p2p.exe|daemon")

    subutai.AddStatus("Installing TUN/TAP driver")
    call([tmpDir+'tap-windows-9.21.2.exe', '/S'])

    sleep(5)

    subutai.Shutdown()

    return 0

