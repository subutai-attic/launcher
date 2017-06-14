import subutai
from time import sleep
from shutil import copyfile
import os
import stat


def subutaistart():

    subutai.AddStatus("Downloading p2p binary")

    subutai.UnregisterService("Subutai P2P")

    subutai.download("p2p.exe")
    while subutai.isDownloadComplete() != 1:
        sleep(0.05)

    subutai.AddStatus("Downloading extra tools")
    subutai.download("nssm.exe")
    while subutai.isDownloadComplete() != 1:
        sleep(0.05)

    tmpDir = subutai.GetTmpDir()
    installDir = subutai.GetInstallDir()

    subutai.AddStatus("Download finished. Installing P2P")
    copyfile(tmpDir+"/p2p.exe", installDir+"/bin/p2p.exe")
    subutai.AddStatus("Installing Service Manageer")
    copyfile(tmpDir+"/nssm.exe", installDir+"/bin/nssm.exe")

    subutai.AddStatus("Making P2P binary executable")
    st = os.stat(installDir+"/bin/p2p.exe")
    os.chmod(installDir+"/bin/p2p.exe", st.st_mode | stat.S_IEXEC)

    subutai.AddStatus("Registering P2P as a Windows service")
    subutai.RegisterService("Subutai P2P", installDir+"bin/p2p.exe|daemon")

    sleep(10)

    subutai.Shutdown()

    return 0
