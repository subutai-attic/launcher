import subutai
from time import sleep
from shutil import copyfile
import os
import stat

def subutaistart():
    #sudobin = "gksudo "
    sudobin = "pkexec --user root "
    #sudobin = ""

    subutai.CheckDirectories()
    version = subutai.getDistro();
    #vboxDeb = "virtualbox-" + "xenial" + ".deb"
    vboxDeb = "virtualbox-" + version + ".deb"

    subutai.download(vboxDeb)

    while subutai.isDownloadComplete() != 1:
        sleep(0.05)

    tmpDir = subutai.GetTmpDir()

    ret = os.system(sudobin + "dpkg -i " + tmpDir + "/" + vboxDeb)
    ret = os.system(sudobin + "apt-get -y install -f")
    ret = os.system(sudobin + "apt-get -y install snapcraft sshpass")
    """
    ret = os.system("dpkg -i " + tmpDir + "/" + vboxDeb)
    ret = os.system("apt-get -y install -f")
    ret = os.system("apt-get -y install snapcraft sshpass")
    """
    subutai.Shutdown()
