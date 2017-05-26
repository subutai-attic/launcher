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

    tmpDir = subutai.GetTmpDir()
    installDir = subutai.GetInstallDir()

    subutai.AddStatus("Download finished. Installing")
    copyfile(tmpDir+"/p2p.exe", installDir+"/bin/p2p.exe")

    st = os.stat(installDir+"/bin/p2p.exe")
    os.chmod(installDir+"/bin/p2p.exe", st.st_mode | stat.S_IEXEC)

    subutai.Shutdown()
