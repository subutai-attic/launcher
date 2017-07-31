import subutai
from time import sleep
from shutil import copyfile
import os
import stat


def subutaistart():

    subutai.AddStatus("Download p2p binary")

    subutai.download("p2p_osx")
    while subutai.isDownloadComplete() != 1:
        sleep(0.05)

    tmpDir = subutai.GetTmpDir()
    installDir = subutai.GetInstallDir()

    subutai.AddStatus("Download finished. Installing")
    copyfile(tmpDir+"/p2p_osx", installDir+"/bin/p2p")

    st = os.stat(installDir+"/bin/p2p")
    os.chmod(installDir+"/bin/p2p", st.st_mode | stat.S_IEXEC)

    subutai.AddStatus("Creating symlink")
    subutai.MakeLink(installDir+"/bin/p2p", "/usr/local/bin/p2p")

    subutai.Shutdown()

    return 0
