import subutai
import hashlib
import datetime
import os
import stat
from shutil import copyfile
from subprocess import call
from time import sleep
from subprocess import Popen, PIPE


# This file provides functions and classes specific to
# Darwin platform

class PostInstall:

    def __init__(self):
        return 0


def GetVirtualMachineName():
    m = hashlib.md5()
    m.update(datetime.datetime.now().isoformat().encode('utf-8'))
    return "subutai-dd-" + m.hexdigest()[:5]


def CleanSSHKeys(host, port):
    call(['ssh-keygen', '-R', '['+host+']:'+port])


def CheckCocoasudo(install):
    if not os.path.exists(install+"bin/cocoasudo"):
        return False
    return True


def InstallCocoasudo(tmp, install, progress):
    subutai.AddStatus("Downloading cocoasudo application")
    subutai.download("cocoasudo")
    while subutai.isDownloadComplete() != 1:
        sleep(0.05)

    try:
        copyfile(tmp+"cocoasudo", install+"bin/cocoasudo")
        st = os.stat(install+"bin/cocoasudo")
        os.chmod(install+"bin/cocoasudo", st.st_mode | stat.S_IEXEC)
    except:
        subutai.RaiseError("Failed to install cocoasudo. Aborting")
        return -87


def CheckVirtualBox():
    if not os.path.exists("/Applications/VirtualBox.app"):
        return False
    return True


def InstallVirtualBox(tmp, install, progress):
    subutai.AddStatus("Downloading VirtualBox")
    subutai.download("VirtualBox.pkg")
    while subutai.isDownloadComplete() != 1:
        sleep(0.05)
        progress.setVboxProgress(subutai.GetBytesDownload())
        progress.updateProgress()

    subutai.AddStatus("Installing VirtualBox")
    try:
        if not CheckOsascript():
            returnCode = call([install+"bin/cocoasudo",
                               '--prompt="Install VirtualBox"',
                               'installer',
                               '-pkg',
                               tmp+'VirtualBox.pkg',
                               '-target',
                               '/'])
            if returnCode == 0:
                return True
            else:
                return False
        else:
            ins = 'do shell script "installer -pkg '+tmp+'VirtualBox.pkg -target /" with administrator privileges'
            p = Popen(['osascript', '-'], stdin=PIPE, stdout=PIPE, stderr=PIPE, universal_newlines=True)
            stdout, stderr = p.communicate(ins)
            if p.returncode != 0:
                return False
            return True

    except:
        subutai.RaiseError("Failed to install VirtualBox. Aborting")
        sleep(10)
        return 45

    progress.setVboxProgress(progress.getVboxSize())
    progress.updateProgress()
    if not os.path.exists("/Applications/VirtualBox.app"):
        subutai.AddStatus("Failed to install VirtualBox. Aborting")
        return 24

    return 0


def CheckOsascript():
    p = Popen(['which', 'osascript'], stdin=PIPE, stdout=PIPE, stderr=PIPE, universal_newlines=True)
    stdout, stderr = p.communicate('')
    if p.returnCode != 0:
        return False
    return True


def InstallPeerPrerequisites(tmp, install, progress):
    rc = 0
    if not CheckOsascript() and not CheckCocoasudo(install):
        rc = InstallCocoasudo(tmp, install, progress)
        if rc != 0:
            return rc

    if not CheckVirtualBox():
        InstallVirtualBox(tmp, install, progress)
