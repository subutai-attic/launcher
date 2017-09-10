import subutai
import subuco
import hashlib
import datetime
import os
import stat
import tarfile
from shutil import copyfile
from subprocess import call
from time import sleep
from subprocess import Popen, PIPE


# This file provides functions and classes specific to
# Linux platform


def GetVirtualMachineName():
    m = hashlib.md5()
    m.update(datetime.datetime.now().isoformat().encode('utf-8'))
    return "subutai-ld-" + m.hexdigest()[:5]


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


def CheckAndKillVirtualBox():
    p = Popen(['ps', 'aux'], stdin=PIPE, stdout=PIPE, stderr=PIPE, universal_newlines=True)
    stdout, stderr = p.communicate('')
    count = 0
    index = 1
    substr = 'VBoxHeadless'
    while index >= 0:
        index = stdout.find(substr, index+len(substr), len(stdout))
        print(index)
        count = count + 1
        if count > 10:
            break

    if (count > 1):
        p = Popen(['killall', '-9', substr], stdin=PIPE, stdout=PIPE, stderr=PIPE, universal_newlines=True)
        stdout, stderr = p.communicate('')

    return 0


# TODO: Add support of other operating systems
def GetVirtualBoxName():
    vboxFile = "virtualbox-5.1_xenial_amd64.deb"
    vnum = subutai.GetOSVersionNumber()
    if vnum == "16.10":
        vboxFile = "virtualbox-5.1_yakkety_amd64.deb"
    elif vnum == "17.04":
        vboxFile = "virtualbox-5.1_zesty_amd64.deb"
    return vboxFile


def InstallVirtualBox(tmp, install, progress):
    subutai.AddStatus("Downloading VirtualBox")
    subutai.download(GetVirtualBoxName())
    while subutai.isDownloadComplete() != 1:
        sleep(0.05)
        progress.setVboxProgress(subutai.GetBytesDownload())
        progress.updateProgress()

    progress.setVboxProgress(progress.getVboxSize())
    progress.updateProgress()
    subutai.AddStatus("Installing VirtualBox")
    try:
        subutai.AddStatus("Installing VirtualBox")
        subutai.InstallVBox(tmp+GetVirtualBoxName())
    except:
        subutai.RaiseError("Failed to install VirtualBox. Aborting")
        return 45
    if not CheckVirtualBox():
        subutai.AddStatus("Failed to install VirtualBox. Aborting")
        return 24

    return 0


def InstallPeerPrerequisites(tmp, install, progress):
    rc = 0
    if not CheckVirtualBox():
        rc = InstallVirtualBox(tmp, install, progress)
    return rc


class P2P:
    def __init__(self, tmp, install):
        self.P2PFile = 'p2p'
        self.RemoteP2PFile = 'p2p'
        self.tmp = tmp
        self.install = install
        self.progress = subuco.Progress()
        self.Daemon = 'p2p.service'

    def PreInstall(self):
        self.progress.setP2P(self.RemoteP2PFile)
        self.progress.calculateTotal()
        return 0

    def Download(self):
        rc = 0
        subutai.AddStatus("Downloading")
        subutai.download(self.RemoteP2PFile)
        while subutai.isDownloadComplete() != 1:
            sleep(0.05)
            self.progress.setP2PProgress(subutai.GetBytesDownload())
            self.progress.updateProgress()

        self.progress.setP2PProgress(self.progress.getP2PSize())
        self.progress.updateProgress()

        try:
            st = os.stat(self.install+"/bin/"+self.P2PFile)
            os.chmod(self.install+"/bin/"+self.P2PFile, st.st_mode | stat.S_IEXEC)
        except:
            subutai.RaiseError("Failed to make p2p binary executable")
            return 31

        return rc

    def PostInstall(self):
        self.__writeConfiguration()
        postinst = subuco.PostInstall(self.tmp)
        postinst.append('systemctl stop p2p.service')
        postinst.append('systemctl disable p2p.service')
        postinst.append('cp '+self.tmp+self.RemoteP2PFile+' '+self.install+self.P2PFile)
        postinst.append('cp '+self.tmp+self.Daemon+' /etc/systemd/system/'+self.Daemon)
        postinst.append('systemctl enable '+self.Daemon)
        postinst.append('systemctl start'+self.Daemon)
        p = Popen(['/usr/bin/gksudo', '--message', 'Finalize P2P Installation', postinst.get()], stdin=PIPE, stdout=PIPE, stderr=PIPE, universal_newlines=True)
        stdout, stderr = p.communicate('')
        return 0

    def __writeConfiguration(self):
        service = '''
[Unit]
Description=Service for Subutai p2p daemon
Wants=network-online.target
After=network-online.target

[Service]
ExecStart=/opt/subutai/bin/p2p daemon
Restart=on-failure
WorkingDirectory=/opt/subutai/

TimeoutStopSec=30
Type=simple

[Install]
WantedBy=multi-user.target
        '''.strip()

        f = open(self.tmp+self.Daemon, 'w')
        f.write(service)
        f.close()
        return 0

class Tray:
    def __init__(self, tmp, install):
        self.TrayFile = 'SubutaiTray_libs.tar.gz'
        self.tmp = tmp
        self.install = install
        self.progress = subuco.Progress()

    def GetTrayFile(self):
        return self.TrayFile

    def Download(self):
        rc = 0
        subutai.download(self.TrayFile)
        while subutai.isDownloadComplete() != 1:
            sleep(0.05)
            self.progress.setTrayProgress(subutai.GetBytesDownload())
            self.progress.updateProgress()

        self.progress.setTrayProgress(self.progress.getTraySize())
        self.progress.updateProgress()
        return rc

    def PreInstall(self):
        self.progress.setTray(self.TrayFile)
        self.progress.calculateTotal()
        return 0

    def PostInstall(self):
        try:
            tar = tarfile.open(self.tmp+self.TrayFile, "r:gz")
            tar.extractall(self.install+'bin')
            tar.close()
        except:
            subutai.RaiseError("Failed to unpack Tray archive. Aborting")
            return 86

        self.__writeDesktopFile()
        postinst = subuco.PostInstall(self.tmp)
        postinst.append('ln -s '+self.install+'bin/SubutaiTray /usr/local/bin/SubutaiTray')
        postinst.append('desktop-file-install '+self.tmp+'SubutaiTray.desktop')
        try:
            p = Popen(['/usr/bin/gksudo', '--message', 'Finalize Tray Installation', postinst.get()], stdin=PIPE, stdout=PIPE, stderr=PIPE, universal_newlines=True)
            stdout, stderr = p.communicate('')
        except:
            subutai.RaiseError("Failed to launch Subutai Tray application")

        return 0

    def __writeDesktopFile(self):
        desktop = '''
[Desktop Entry]
Version=1.0
Name=Subutai Tray
Comment=Subutai Tray
Keywords=Internet;
Exec=SubutaiTray
Terminal=false
Type=Application
Icon=/usr/share/icons/launcher-logo.png
Categories=GNOME;Network;
StartupNotify=true
        '''.strip()
        f = open(self.tmp+"SubutaiTray.desktop", 'w')
        f.write(desktop)
        f.close()

class E2E:
    def __init__(self, tmp, install):
        self.GoogleChromeFile = 'google-chrome-stable_current_amd64.deb'
        self.Plugin = 'kpmiofpmlciacjblommkcinncmneeoaa.json'
        self.tmp = tmp
        self.install = install
        self.progress = subuco.Progress()

    def PreInstall(self):
        subutai.SetAction("PREINST")
        self.progress.setChrome(self.GoogleChromeFile)
        self.progress.calculateTotal()
        return 0

    def Download(self):
        rc = 0
        subutai.SetAction("DWL")
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
        subutai.SetAction("INST")
        pass

    def PostInstall(self):
        subutai.SetAction("POSINST")
        location = '/opt/google/chrome/extensions/'
        postinst = subuco.PostInstall(self.tmp)
        postinst.append('dpkg -i '+self.tmp+self.GoogleChromeFile)
        postinst.append('apt-get install -f')
        if not os.path.exists(location):
            postinst.append('mkdir -p '+location)
        postinst.append('cp '+self.tmp+self.Plugin+' '+location)
        try:
            p = Popen(['/usr/bin/gksudo', '--message', 'Finalize E2E Installation', postinst.get()], stdin=PIPE, stdout=PIPE, stderr=PIPE, universal_newlines=True)
            stdout, stderr = p.communicate('')
        except:
            subutai.RaiseError("Failed to install Google Chrome")
            return 64
        return 0

    def __writeJson(self):
        ete = '{\n\t"external_update_url": "https://clients2.google.com/service/update2/crx"\n}\n'
        f = open(self.tmp+self.Plugin, 'w')
        f.write(ete)
        f.close()
        return 0

    def __checkGoogleChrome(self):
        return False
