import subutai
import subuco
import hashlib
import datetime
import os
import stat
import zipfile
from shutil import copyfile
from subprocess import call
from time import sleep
from subprocess import Popen, PIPE


# This file provides functions and classes specific to
# Darwin platform


def GetVirtualMachineName():
    m = hashlib.md5()
    m.update(datetime.datetime.now().isoformat().encode('utf-8'))
    return "subutai-dw-" + m.hexdigest()[:5]


def CleanSSHKeys(host, port):
    try:
        call(['ssh-keygen', '-R', '['+host+']:'+port])
    except:
        subutai.RaiseError("Failed to clean SSH keys")
        return 78
    return 0


def CheckVirtualBox():
    if subutai.IsVBoxInstalled() != 0:
        return False
    return True


def InstallVirtualBox(tmp, install, progress):
    subutai.AddStatus("Downloading VirtualBox")
    vboxfile = "VirtualBox.exe"
    subutai.download(vboxfile)
    while subutai.isDownloadComplete() != 1:
        progress.setVboxProgress(subutai.GetBytesDownload())
        progress.updateProgress()

    subutai.AddStatus("Installing VirtualBox")
    try:
        call([tmp+vboxfile, '-silent'])
    except:
        subutai.RaiseError("Failed to install VirtualBox. Aborting")
        return 45

    return 0


def InstallPeerPrerequisites(tmp, install, progress):
    rc = 0
    if not CheckVirtualBox():
        rc = InstallVirtualBox(tmp, install, progress)
    return rc


class P2P:
    def __init__(self, tmp, install):
        self.P2PFile = 'p2p.exe'
        self.RemoteP2PFile = 'p2p.exe'
        self.NssmFile = 'nssm.exe'
        self.TapFile = 'tap-windows-9.21.2.exe'
        self.tmp = tmp
        self.install = install
        self.progress = subuco.Progress()

    def PreInstall(self):
        self.progress.setP2P(self.RemoteP2PFile)
        self.progress.setTuntap(self.TapFile)
        self.progress.setNssm(self.NssmFile)
        self.progress.calculateTotal()
        return 0

    def Download(self):
        rc = 0
        subutai.AddStatus("Installing")
        if not self.__checkNssm():
            subutai.download(self.NssmFile)
            while subutai.isDownloadComplete() != 1:
                sleep(0.05)
                self.progress.setNssmProgress(subutai.GetBytesDownload())
                self.progress.updateProgress()

            self.progress.setNssmProgress(self.progress.getNssmSize())
            self.progress.updateProgress()
            try:
                copyfile(self.tmp+self.NssmFile, self.install+"bin/"+self.NssmFile)
            except:
                subutai.RaiseError("Failed to move NSSM file to it's destination")
                return 14
        else:
            subutai.UnregisterService("Subutai P2P")
            subutai.ProcessKill("nssm.exe")
            subutai.ProcessKill("p2p.exe")
            

        self.__installTuntap()    

        subutai.download(self.RemoteP2PFile)
        while subutai.isDownloadComplete() != 1:
            sleep(0.05)
            self.progress.setP2PProgress(subutai.GetBytesDownload())
            self.progress.updateProgress()

        self.progress.setP2PProgress(self.progress.getP2PSize())
        self.progress.updateProgress()
        try:
            copyfile(self.tmp+self.RemoteP2PFile, self.install+"bin/"+self.P2PFile)
        except:
            subutai.RaiseError("Failed to move p2p binary to " +
                            self.install + "bin/"+self.P2PFile)
            return 19

        try:
            st = os.stat(self.install+"/bin/"+self.P2PFile)
            os.chmod(self.install+"/bin/"+self.P2PFile, st.st_mode | stat.S_IEXEC)
        except:
            subutai.RaiseError("Failed to make p2p binary executable")
            return 31

        return rc

    def PostInstall(self):
        subutai.RegisterService("Subutai P2P", self.install+"bin/p2p.exe|daemon")
        return 0

    def __checkNssm(self):
        if os.path.exists(self.install+self.NssmFile):
            return True
        return False

    def __installTuntap(self):
        subutai.AddStatus("Downloading TUN/TAP driver")
        subutai.download(self.TapFile)
        while subutai.isDownloadComplete() != 1:
            sleep(0.05)
            self.progress.setTuntapProgress(subutai.GetBytesDownload())
            self.progress.updateProgress()
        
        self.progress.setTuntapProgress(self.progress.getTuntapSize())
        self.progress.updateProgress()
        subutai.AddStatus("Installing TUN/TAP driver")
        call([self.tmp+self.TapFile, '/S'])
        return 0


class Tray:
    def __init__(self, tmp, install):
        self.TrayFile = 'SubutaiTray_libs.zip'
        self.LibsshFile = 'ssh.zip'
        self.tmp = tmp
        self.install = install
        self.progress = subuco.Progress()

    def GetTrayFile(self):
        return self.TrayFile

    def GetLibsshFile(self):
        return self.LibsshFile
        
    def Download(self):
        rc = 0
        subutai.AddStatus("Downloading libssh2")
        subutai.download(self.LibsshFile)
        while subutai.isDownloadComplete() != 1:
            sleep(0.05)
            self.progress.setLibsshProgress(subutai.GetBytesDownload())
            self.progress.updateProgress()

        self.progress.setLibsshProgress(self.progress.getLibsshSize())
        self.progress.updateProgress()
        
        subutai.ProcessKill("SubutaiTray.exe")
        subutai.ProcessKill("ssh.exe")
        subutai.ProcessKill("ssh-keygen.exe")

        subutai.download(self.TrayFile)
        while subutai.isDownloadComplete() != 1:
            sleep(0.05)
            self.progress.setTrayProgress(subutai.GetBytesDownload())
            self.progress.updateProgress()

        self.progress.setTrayProgress(self.progress.getTraySize())
        self.progress.updateProgress()

        try:
            zf = zipfile.ZipFile(self.tmp+self.TrayFile, 'r')
            zf.extractall(self.install)
            zf.close()
            zfl = zipfile.ZipFile(self.tmp+self.LibsshFile, 'r')
            zfl.extractall(self.install+"/bin")
            zfl.close()
        except:
            subutai.RaiseError("Failed to replace tray")

        return rc

    def PreInstall(self):
        self.progress.setTray(self.TrayFile)
        self.progress.setLibssh(self.LibsshFile)
        self.progress.calculateTotal()
        return 0

    def PostInstall(self):
        subutai.AddStatus("Writing configuration")
        unPath = self.install.replace('\\', '/')
        unVBoxPath = subutai.GetVBoxPath().replace('\\', '/')
        f = open(unPath+"/tray/subutai_tray.ini", "w")
        f.write("P2P_Path="+unPath+"/bin/p2p.exe\n")
        if unVBoxPath != "":
            f.write("VBoxManage_Path="+unVBoxPath+"\n")
        f.write("Ssh_Path="+unPath+"/bin/ssh.exe\n")
        f.write("Ssh_Keygen_Cmd="+unPath+"/bin/ssh-keygen.exe\n")
        f.close()

        return 0


class E2E:
    def __init__(self, tmp, install):
        self.GoogleChromeFile = 'GoogleChromeStandaloneEnterprise64.msi'
        self.tmp = tmp
        self.install = install
        self.progress = subuco.Progress()

    def PreInstall(self):
        self.progress.setChrome(self.GoogleChromeFile)
        self.progress.calculateTotal()
        return 0

    def Download(self):
        rc = 0
        if not self.__checkGoogleChrome():
            subutai.AddStatus("Downloading Google Chrome")
            subutai.download(self.GoogleChromeFile)
            while subutai.isDownloadComplete() != 1:
                sleep(0.05)
                self.progress.setChromeProgress(subutai.GetBytesDownload())
                self.progress.updateProgress()

            self.progress.setChromeProgress(self.progress.getChromeSize())
            self.progress.updateProgress()
            
        return rc

    def Install(self):
        subutai.AddStatus("Installing Chrome")
        try:
            call(['msiexec', '/qn', '/i', self.tmp+self.GoogleChromeFile])
        except:
            subutai.RaiseError("Failed to install Google Chrome")
            return 28
        return 0

    def PostInstall(self):
        subutai.AddStatus("Installing Browser Plugin")
        subutai.RegisterPlugin()
        return 0

    def __checkGoogleChrome(self):
        if subutai.IsChromeInstalled() != 0:
            return False
        return True