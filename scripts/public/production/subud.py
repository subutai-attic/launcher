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
# Darwin platform


def GetVirtualMachineName():
    m = hashlib.md5()
    m.update(datetime.datetime.now().isoformat().encode('utf-8'))
    return "subutai-dd-" + m.hexdigest()[:5]


def CleanSSHKeys(host, port):
    try:
        call(['ssh-keygen', '-R', '['+host+']:'+port])
    except:
        subutai.RaiseError("Failed to clean SSH keys")
        return 78
    return 0

def GetVirtualBoxName():
    return 'VirtualBox.pkg'


def CheckCocoasudo(install):
    if not os.path.exists(install+"bin/cocoasudo"):
        return False
    return True


def InstallCocoasudo(tmp, install, progress):
    subutai.AddStatus("Downloading cocoasudo application")
    subutai.download("cocoasudo")
    while subutai.isDownloadComplete() != 1:
        sleep(0.05)
        progress.setCocoasudoProgress(subutai.GetBytesDownload())
        progress.updateProgress()

    progress.setCocoasudoProgress(progress.getCocoasudoSize())
    progress.updateProgress()

    subutai.AddStatus("Installing cocoasudo")
    try:
        copyfile(tmp+"cocoasudo", install+"bin/cocoasudo")
        st = os.stat(install+"bin/cocoasudo")
        os.chmod(install+"bin/cocoasudo", st.st_mode | stat.S_IEXEC)
    except:
        subutai.RaiseError("Failed to install cocoasudo. Aborting")
        return -87
    return 0

def CheckVirtualBox():
    if not os.path.exists("/Applications/VirtualBox.app"):
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


def InstallVirtualBox(tmp, install, progress):
    subutai.AddStatus("Downloading VirtualBox")
    subutai.download("VirtualBox.pkg")
    while subutai.isDownloadComplete() != 1:
        sleep(0.05)
        progress.setVboxProgress(subutai.GetBytesDownload())
        progress.updateProgress()

    subutai.AddStatus("Installing VirtualBox")
    try:
        CheckAndKillVirtualBox()
        if not CheckOsascript():
            returnCode = call([install+"bin/cocoasudo",
                               '--prompt="Install VirtualBox"',
                               'installer',
                               '-pkg',
                               tmp+'VirtualBox.pkg',
                               '-target',
                               '/'])
            if returnCode != 0:
                return 21
        else:
            ins = 'do shell script "installer -pkg '+tmp+'VirtualBox.pkg -target /" with administrator privileges'
            p = Popen(['osascript', '-'], stdin=PIPE, stdout=PIPE, stderr=PIPE, universal_newlines=True)
            stdout, stderr = p.communicate(ins)
            if p.returncode != 0:
                return 23

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
    if p.returncode != 0:
        return False
    return True


def InstallPeerPrerequisites(tmp, install, progress):
    rc = 0
    if not CheckOsascript() and not CheckCocoasudo(install):
        rc = InstallCocoasudo(tmp, install, progress)
        if rc != 0:
            return rc

    if not CheckVirtualBox():
        rc = InstallVirtualBox(tmp, install, progress)

    return rc


class P2P:
    def __init__(self, tmp, install):
        self.P2PFile = 'p2p'
        self.RemoteP2PFile = 'p2p_osx'
        self.CocoasudoFile = 'cocoasudo'
        self.TapFile = 'tuntap_20150118_osx.pkg'
        self.tmp = tmp
        self.install = install
        self.progress = subuco.Progress()
        self.Daemon = 'io.subutai.p2p.daemon.plist'
        self.LogConf = 'p2p.conf'

    def PreInstall(self):
        self.progress.setP2P(self.RemoteP2PFile)
        self.progress.setTuntap(self.TapFile)
        self.progress.setCocoasudo(self.CocoasudoFile)
        self.progress.calculateTotal()
        return 0

    def Download(self):
        rc = 0
        subutai.AddStatus("Installing")
        if not CheckOsascript() and not CheckCocoasudo(self.install):
            rc = InstallCocoasudo(self.tmp, self.install, self.progress)
            if rc != 0:
                return rc

        if not self.__checkTuntap():
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
        self.__writeConfiguration()
        postinst = subuco.PostInstall(self.tmp)
        postinst.append('if [ -e /Library/LaunchDaemons/'+self.Daemon+' ]; then')
        postinst.append('launchctl unload /Library/LaunchDaemons/'+self.Daemon)
        postinst.append('fi')
        postinst.append('cp '+self.tmp+self.Daemon+' /Library/LaunchDaemons/'+self.Daemon)
        postinst.append('cp '+self.tmp+self.LogConf+' /etc/newsyslog.d/'+self.LogConf)
        postinst.append('launchctl load /Library/LaunchDaemons/'+self.Daemon)
        postinst.append('installer -pkg '+self.tmp+self.TapFile+' -target /')
        postinst.append('if [ -e /usr/local/bin/p2p ]; then')
        postinst.append('rm -f /usr/local/bin/p2p')
        postinst.append('fi')
        postinst.append('ln -s '+self.install+'bin/p2p /usr/local/bin/p2p')
        ins = 'do shell script "/bin/sh '+postinst.get()+'" with administrator privileges'
        p = Popen(['osascript', '-'], stdin=PIPE, stdout=PIPE, stderr=PIPE, universal_newlines=True)
        stdout, stderr = p.communicate(ins)
        if p.returncode != 0:
            return 91
        return 0

    def __checkTuntap(self):
        if not os.path.exists('/dev/tap0'):
            return False
        return True

    def __installTuntap(self):
        while subutai.isDownloadComplete() != 1:
            sleep(0.05)
            self.progress.setTuntapProgress(subutai.GetBytesDownload())

        self.progress.setTuntapProgress(self.progress.getTuntapSize())
        self.progress.updateProgress()
        return 0

    def __writeConfiguration(self):
        service = '''
        <?xml version="1.0" encoding="UTF-8"?>
        <!DOCTYPE plist PUBLIC "-//Apple Computer//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
        <plist version="1.0">
        <dict>
        <key>Label</key>
        <string>io.subutai.p2p.daemon</string>

        <key>ProgramArguments</key>
        <array>
        <string>/usr/local/bin/p2p</string>
        <string>daemon</string>
        </array>

        <key>KeepAlive</key>
        <true/>

        <key>StandardOutPath</key>
        <string>/var/log/p2p.log</string>

        <key>StandardErrorPath</key>
        <string>/var/log/p2p.log</string>

        <key>Debug</key>
        <true/>
        </dict>
        </plist>
        '''.strip()

        daemonFile = 'io.subutai.p2p.daemon.plist'

        f = open(self.tmp+daemonFile, 'w')
        f.write(service)
        f.close()

        syslog = '''
        # logfilename          [owner:group]    mode count size when  flags [/pid_file] [sig_num]
        /var/log/p2p.log                       644  7     *    $D0   J
        '''.strip()
        sf = open(self.tmp+'p2p.conf', 'w')
        sf.write(syslog)
        sf.close()
        return 0

class Tray:
    def __init__(self, tmp, install):
        self.TrayFile = 'SubutaiTray_libs_osx.tar.gz'
        self.CocoasudoFile = 'cocoasudo'
        self.LibsshFile = 'libssh2-1.6.0-0_osx.pkg'
        self.tmp = tmp
        self.install = install
        self.progress = subuco.Progress()

    def GetTrayFile(self):
        return self.TrayFile

    def GetCocoasudoFile(self):
        return self.CocoasudoFile

    def GetLibsshFile(self):
        return self.LibsshFile
        
    def Download(self):
        rc = 0
        
        if not CheckOsascript() and not CheckCocoasudo(self.install):
            rc = InstallCocoasudo(self.tmp, self.install, self.progress)
        if rc != 0:
            return rc

        if not self.__checkLibssh():
            subutai.AddStatus("Downloading libssh2")
            subutai.download(self.LibsshFile)
            while subutai.isDownloadComplete() != 1:
                sleep(0.05)
                self.progress.setLibsshProgress(subutai.GetBytesDownload())
                self.progress.updateProgress()

            self.progress.setLibsshProgress(self.progress.getLibsshSize())
            self.progress.updateProgress()
        
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
        self.progress.setCocoasudo(self.CocoasudoFile)
        self.progress.setLibssh(self.LibsshFile)
        self.progress.calculateTotal()
        return 0

    def PostInstall(self):
        try:
            tar = tarfile.open(self.tmp+self.TrayFile, "r:gz")
            tar.extractall("/Applications/Subutai")
            tar.close()
        except:
            subutai.RaiseError("Failed to unpack Tray archive. Aborting")
            return 86
        postinst = subuco.PostInstall(self.tmp)
        postinst.append('installer -pkg '+self.tmp+self.LibsshFile+' -target /')
        ins = 'do shell script "/bin/sh '+postinst.get()+'" with administrator privileges'
        p = Popen(['osascript', '-'], stdin=PIPE, stdout=PIPE, stderr=PIPE, universal_newlines=True)
        stdout, stderr = p.communicate(ins)
        if p.returncode != 0:
            return 91

        try:
            launch = 'tell application "SubutaiTray.app" to activate'
            p = Popen(['osascript', '-'], stdin=PIPE, stdout=PIPE, stderr=PIPE, universal_newlines=True)
            stdout, stderr = p.communicate(launch)
        except:
            subutai.RaiseError("Failed to launch Subutai Tray application")

        return 0

    def __checkLibssh(self):
        if not os.path.exists('/usr/local/lib/libssh2.dylib'):
            return False
        return True


class E2E:
    def __init__(self, tmp, install):
        self.GoogleChromeFile = 'GoogleChrome_osx.tar.gz'
        self.CocoasudoFile = 'cocoasudo'
        self.tmp = tmp
        self.install = install
        self.progress = subuco.Progress()

    def PreInstall(self):
        subutai.SetAction('PREINST')
        self.progress.setChrome(self.GoogleChromeFile)
        self.progress.setCocoasudo(self.CocoasudoFile)
        self.progress.calculateTotal()
        return 0

    def Download(self):
        subutai.SetAction('DWL')
        rc = 0
        if not CheckOsascript() and not CheckCocoasudo(self.install):
            rc = InstallCocoasudo(self.tmp, self.install, self.progress)
        if rc != 0:
            return rc

        if not self.__checkGoogleChrome():
            subutai.AddStatus("Downloading Google Chrome")
            subutai.download(self.GoogleChromeFile)
            while subutai.isDownloadComplete() != 1:
                sleep(0.05)
                self.progress.setChromeProgress(subutai.GetBytesDownload())
                self.progress.updateProgress()

            self.progress.setChromeProgress(self.progress.getChromeSize())
            self.updateProgress()
        
        return rc

    def Install(self):
        subutai.SetAction('INST')
        if not os.path.exists(self.tmp+self.GoogleChromeFile):
            return 0
        try:
            script = 'do shell script "/usr/bin/tar -xf '+self.tmp+self.GoogleChromeFile+' -C /Applications" with administrator privileges'
            p = Popen(['osascript', '-'], stdin=PIPE, stdout=PIPE, stderr=PIPE, universal_newlines=True)
            stdout, stderr = p.communicate(script)
        except:
            subutai.RaiseError("Failed to install Google Chrome")
            return 64
        return 0

    def PostInstall(self):
        subutai.SetAction('POSTINST')
        pfile = 'ffddnlbamkjlbngpekmdpnoccckapcnh.json'
        location = os.environ['HOME'] + '/Library/Application Support/Google/Chrome/External Extensions'
        slocation = os.environ['HOME'] + '/Library/Application\ Support/Google/Chrome/External\ Extensions/'
        script = 'tell application "Google Chrome" to quit'
        p = Popen(['osascript', '-'], stdin=PIPE, stdout=PIPE, stderr=PIPE, universal_newlines=True)
        stdout, stderr = p.communicate(script)
        postinst = subuco.PostInstall(self.tmp)
        postinst.append('if [ ! -d "'+location+'" ]; then')
        postinst.append('mkdir -p ' + slocation)
        postinst.append('fi')
        postinst.append('rm -f '+slocation+pfile)
        postinst.append('cp '+self.tmp+pfile+' '+slocation+pfile)
        postinst.append('chown -R '+os.environ['USER']+' '+os.environ['HOME']+'/Library/Application\ Support/Google' )
        ins = 'do shell script "/bin/sh '+postinst.get()+'" with administrator privileges'
        p = Popen(['osascript', '-'], stdin=PIPE, stdout=PIPE, stderr=PIPE, universal_newlines=True)
        stdout, stderr = p.communicate(ins)
        if p.returncode != 0:
            return 95
        return 0

    def __checkGoogleChrome(self):
        if not os.path.exists('/Applications/Google Chrome.app'):
            return False
        return True