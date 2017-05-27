import subutai
from time import sleep
from shutil import copyfile
import os
import stat


def subutaistart():

    subutai.AddStatus("Download p2p binary")

    subutai.download("p2p.exe")
    while subutai.isDownloadComplete() != 1:
        sleep(0.05)

    subutai.AddStatus("Download extra tools")
    subutai.download("nssm.exe")
    while subutai.isDownloadComplete() != 1:
        sleep(0.05)

    tmpDir = subutai.GetTmpDir()
    installDir = subutai.GetInstallDir()

    subutai.AddStatus("Download finished. Installing")
    copyfile(tmpDir+"/p2p.exe", installDir+"/bin/p2p.exe")

    st = os.stat(installDir+"/bin/p2p.exe")
    os.chmod(installDir+"/bin/p2p.exe", st.st_mode | stat.S_IEXEC)

    subutai.AddStatus("Registering P2P as a Windows service")
    subutai.RegisterService("P2P", installDir+"/bin/p2p.exe|daemon")

    sleep(10)

    subutai.Shutdown()
