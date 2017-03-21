import subutai
from time import sleep
from shutil import copyfile
import os
import stat

def subutaistart():
    subutai.CheckDirectories()
    subutai.download("p2p")
    while subutai.isDownloadComplete() != 1:
        sleep(0.05)

    subutai.download("p2p-ubuntu-service")
    while subutai.isDownloadComplete() != 1:
        sleep(0.05)

    tmpDir = subutai.GetTmpDir()
    targetDir = subutai.GetInstallDir()

    copyfile(tmpDir+"/p2p", targetDir+"/bin/p2p")
    if os.path.isfile("/usr/bin/p2p"):
        os.remove("/usr/bin/p2p")
    st = os.stat(targetDir+"/bin/p2p")
    os.chmod(targetDir+"/bin/p2p", st.st_mode | stat.S_IEXEC | stat.S_IXOTH | stat.S_IXGRP)
    os.symlink(targetDir+"/bin/p2p", "/usr/bin/p2p")

    copyfile(tmpDir+"/p2p-ubuntu-service", "/etc/init.d/p2p")
    os.chmod("/etc/init.d/p2p", st.st_mode | stat.S_IEXEC | stat.S_IXOTH | stat.S_IXGRP)

    ret = os.system("which service")
    if ret == 0:
        os.system("service p2p restart")
        ret = os.system("update-rc.d p2p defaults")
        if ret != 0:
            os.system("uptate-rc.d p2p enable")

    ret = os.system("which systemctl")
    if ret == 0:
        os.system("systemctl daemon-reload")
        os.system("systemctl enable p2p.service")
        os.system("systemctl restart p2p.service")

    subutai.Shutdown()
