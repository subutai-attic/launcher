import subutai
from time import sleep
from shutil import copyfile
from subprocess import call
import os
import stat


def subutaistart():

    subutai.AddStatus("Downloading p2p binary")

    tmpDir = subutai.GetTmpDir()
    installDir = subutai.GetInstallDir()

    subutai.download("nssm.exe")
    while subutai.isDownloadComplete() != 1:
        sleep(0.05)

    subutai.AddStatus("Installing Service Manageer")
    subutai.UnregisterService("Subutai P2P")
    subutai.ProcessKill("nssm.exe")
    copyfile(tmpDir+"/nssm.exe", installDir+"/bin/nssm.exe")

    subutai.download("p2p.exe")
    while subutai.isDownloadComplete() != 1:
        sleep(0.05)

    subutai.AddStatus("Downloading extra tools")
    subutai.download("tap-windows-9.21.2.exe")
    while subutai.isDownloadComplete() != 1:
        sleep(0.05)

    subutai.ProcessKill("p2p.exe")
    subutai.AddStatus("Download finished. Installing P2P")
    copyfile(tmpDir+"/p2p.exe", installDir+"/bin/p2p.exe")

    subutai.AddStatus("Making P2P binary executable")
    st = os.stat(installDir+"/bin/p2p.exe")
    os.chmod(installDir+"/bin/p2p.exe", st.st_mode | stat.S_IEXEC)

    subutai.AddStatus("Registering P2P as a Windows service")
    subutai.RegisterService("Subutai P2P", installDir+"bin/p2p.exe|daemon")

    subutai.AddStatus("Install TUN/TAP driver")
    call([tmpDir+'tap-windows-9.21.2.exe', '/S'])

    subutai.AddStatus("Post Install Checks")

    sleep(10)

    subutai.Shutdown()

    return 0
