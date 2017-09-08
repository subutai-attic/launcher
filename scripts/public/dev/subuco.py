import subutai
from time import sleep


# This file provides common classes and functions used during installation
# process.

class Progress:
    coreSize = 0
    vboxSize = 0
    ubuntuSize = 0
    openjreSize = 0
    managementSize = 0
    p2pSize = 0
    tuntapSize = 0
    cocasudoSize = 0
    traySize = 0
    libsshSize = 0
    chromeSize = 0
    nssmSize = 0
    coreProgress = 0
    vboxProgress = 0
    ubuntuProgress = 0
    openjreProgress = 0
    managementProgress = 0
    p2pProgress = 0
    tuntapProgress = 0
    cocoasudoProgress = 0
    trayProgress = 0
    libsshProgress = 0
    chromeProgress = 0
    nssmProgress = 0
    totalSize = 0
    lastProgress = 0.0

    def __init__(self):
        self.lastProgress = -1.0
        self.totalSize = 0
        self.coreSize = 0
        self.vboxSize = 0
        self.ubuntuSize = 0
        self.openjreSize = 0
        self.managementSize = 0
        self.p2pSize = 0
        self.tuntapSize = 0
        self.cocoasudoSize = 0
        self.traySize = 0
        self.libsshSize = 0
        self.chromeSize = 0
        self.nssmSize = 0
        self.coreProgress = 0
        self.vboxProgress = 0
        self.ubuntuProgress = 0
        self.openjreProgress = 0
        self.managementProgress = 0
        self.p2pProgress = 0
        self.tuntapProgress = 0
        self.cocoasudoProgress = 0
        self.trayProgress = 0
        self.libsshProgress = 0
        self.chromeProgress = 0
        self.nssmProgress = 0

    def setCore(self, core):
        self.coreSize = subutai.GetFileSize(core)
        self.coreProgress = 0

    def setVBox(self, vbox):
        self.vboxSize = subutai.GetFileSize(vbox)
        self.vboxProgress = 0

    def setUbuntu(self, ubuntu):
        self.ubuntuSize = subutai.GetFileSize(ubuntu)
        self.ubuntuProgress = 0

    def setOpenJRE(self, openjre):
        self.openjreSize = subutai.GetFileSize(openjre)
        self.openjreProgress = 0

    def setManagement(self, management):
        self.managementSize = subutai.GetFileSize(management)
        self.managementProgress = 0

    def setP2P(self, p2p):
        self.p2pSize = subutai.GetFileSize(p2p)
        self.p2pProgress = 0

    def setTuntap(self, tuntap):
        self.tuntapSize = subutai.GetFileSize(tuntap)
        self.tuntapProgress = 0

    def setCocoasudo(self, cocoasudo):
        self.cocoasudoSize = subutai.GetFileSize(cocoasudo)
        self.cocoasudoProgress = 0

    def setTray(self, tray):
        self.traySize = subutai.GetFileSize(tray)
        self.trayProgress = 0

    def setChrome(self, chrome):
        self.chromeSize = subutai.GetFileSize(chrome)
        self.chromeProgress = 0

    def setNssm(self, nssm):
        self.nssmSize = subutai.GetFileSize(nssm)
        self.nssmProgress = 0

    def calculateTotal(self):
        t = 0
        t = t + self.coreSize
        t = t + self.vboxSize
        t = t + self.ubuntuSize
        t = t + self.openjreSize
        t = t + self.managementSize
        t = t + self.p2pSize
        t = t + self.tuntapSize
        t = t + self.cocoasudoSize
        t = t + self.traySize
        t = t + self.libsshSize
        t = t + self.chromeSize
        t = t + self.nssmSize
        self.totalSize = t

    def getCoreSize(self):
        return self.coreSize

    def getVboxSize(self):
        return self.vboxSize

    def getUbuntuSize(self):
        return self.ubuntuSize

    def getOpenjreSize(self):
        return self.openjreSize

    def getManagementSize(self):
        return self.managementSize

    def getP2PSize(self):
        return self.p2pSize

    def getTuntapSize(self):
        return self.tuntapSize

    def getCocoasudoSize(self):
        return self.cocoasudoSize

    def getTraySize(self):
        return self.traySize

    def getLibsshSize(self):
        return self.libsshSize

    def getChromeSize(self):
        return self.chromeSize

    def getNssmSize(self):
        return self.nssmSize

    def setCoreProgress(self, s):
        self.coreProgress = s

    def setVboxProgress(self, s):
        self.vboxProgress = s

    def setUbuntuProgress(self, s):
        self.ubuntuProgress = s

    def setOpenjreProgress(self, s):
        self.openjreProgress = s

    def setManagementProgress(self, s):
        self.managementProgress = s

    def setP2PProgress(self, s):
        self.p2pProgress = s

    def setTuntapProgress(self, s):
        self.tuntapProgress = s

    def setCocoasudoProgress(self, s):
        self.cocoasudoProgress = s

    def setTrayProgress(self, s):
        self.trayProgress = s

    def setLibsshProgress(self, s):
        self.libsshProgress = s

    def setChromeProgress(self, s):
        self.chromeProgress = s

    def setNssmProgress(self, s):
        self.nssmProgress = s

    def updateProgress(self):
        if self.totalSize == 0:
            return
        cur = self.coreProgress
        cur = cur + self.vboxProgress
        cur = cur + self.ubuntuProgress
        cur = cur + self.openjreProgress
        cur = cur + self.managementProgress
        cur = cur + self.p2pProgress
        cur = cur + self.tuntapProgress
        cur = cur + self.cocoasudoProgress
        cur = cur + self.chromeProgress
        cur = cur + self.nssmProgress
        val = (int)(100 * cur) / self.totalSize
        p = (float)(val/100)
        self.lastProgress = p
        subutai.SetProgress(p)

    def spin(self):
        subutai.SetProgress(-1.0)

    def unspin(self):
        subutai.SetProgress(self.lastProgress)
        

class SubutaiPeer:
    PeerIP = ''
    LocalIP = ''
    UbuntuFile = ''
    OpenJREFile = ''
    ManagementFile = ''

    def __init__(self, name, progress, ubuntuFile, openjreFile, mngFile):
        self.name = name
        self.progress = progress
        self.UbuntuFile = ubuntuFile
        self.OpenJREFile = openjreFile
        self.ManagementFile = mngFile

    def GetName(self):
        return self.name

    def WaitForNetwork(self):
        ping = ('if [ $(timeout 3 ping 8.8.8.8 -c1 2>/dev/null | grep -c "1 received") -ne 1 ]; '
                'then echo 1; else echo 0; fi')
        subutai.AddStatus("Waiting for network")
        attempts = 0
        while True:
            out = subutai.SSHRunOut(ping)
            if out == '0':
                break
            if attempts >= 60:
                subutai.RaiseError("Failed to establish Internet connection on peer")
                return 82
            attempts = attempts + 1
            sleep(1)

        return 0

    def SetupVirtualMachine(self):
        rc = 0
        subutai.AddStatus("Installing VM")
        if subutai.CheckVMExists(self.name) != 0:
            subutai.download("core.ova")
            while subutai.isDownloadComplete() != 1:
                sleep(0.05)
                self.progress.setCoreProgress(subutai.GetBytesDownload())
                self.progress.updateProgress()

            subutai.AddStatus("VM Image downloaded")

        self.progress.setCoreProgress(self.progress.getCoreSize())
        self.progress.updateProgress()
        rc = subutai.VBoxS("import " +
                           subutai.GetTmpDir().replace(" ", "+++") +
                           "core.ova --vsys 0 --vmname "+self.name)
        if rc != 0:
            subutai.RaiseError("Failed to import Virtual Machine")
            return rc

        sleep(3)
        cpus = subutai.GetCoreNum()
        mem = subutai.GetMemSize() * 1024
        subutai.VBox("modifyvm " + self.name + " --cpus " + str(cpus))
        subutai.VBox("modifyvm " + self.name + " --memory " + str(mem))
        return rc

    def ConfigureNetwork(self):
        rc = 0
        subutai.AddStatus("Configuring Network")
        gateway = subutai.GetDefaultRoutingInterface()
        bridged = subutai.GetVBoxBridgedInterface(gateway)
        subutai.VBox("modifyvm " + self.name + ' --nic1 bridged --bridgeadapter1 ' + bridged)
        subutai.VBox("modifyvm " + self.name + " --cableconnected1 on")
        subutai.VBox("modifyvm " + self.name + " --nic2 nat")
        subutai.VBox("modifyvm " + self.name + " --cableconnected2 on")
        subutai.VBox("modifyvm " + self.name + ' --natpf2 ssh-fwd,tcp,,4567,,22 --natpf2 https-fwd,tcp,,9999,,8443')

        adapter = subutai.GetVBoxHostOnlyInterface()
        ret = subutai.VBoxS("hostonlyif ipconfig " + adapter + " --ip 192.168.56.1")

        if ret == 1:
            subutai.VBox("hostonlyif create")
            subutai.VBox("hostonlyif ipconfig " + adapter + " --ip 192.168.56.1")
            subutai.VBox("dhcpserver add --ifname " +
                         adapter +
                         " --ip 192.168.56.1 --netmask 255.255.255.0 --lowerip 192.168.56.100 --upperip 192.168.56.200")
            subutai.VBox("dhcpserver modify --ifname " + adapter + " --enable")

        subutai.VBox("modifyvm " + self.name + " --nic3 hostonly --hostonlyadapter3 " + adapter)
        return rc

    def PreconfigureNetwork(self):
        subutai.VBox("modifyvm " + machineName + " --nic1 nat")
        subutai.VBox("modifyvm " + machineName + " --cableconnected1 on")
        subutai.VBox("modifyvm " + machineName + " --natpf1 ssh-fwd,tcp,,4567,,22 --natpf1 https-fwd,tcp,,9999,,8443")
        subutai.VBox("modifyvm " + machineName + " --rtcuseutc on")
        adapterName = subutai.GetVBoxHostOnlyInterface()
        if adapterName != 'undefined':
            subutai.VBox("modifyvm " + machineName + " --nic3 hostonly --hostonlyadapter3 " + adapterName)
        
        return 0

    def StartVirtualMachine(self):
        subutai.log("info", "Starting virtual machine")
        rc = 0
        if subutai.CheckVMRunning(self.name) != 0:
            rc = subutai.VBoxS("startvm --type headless " + self.name)

        if rc != 0:
            subutai.RaiseError("Failed to start Virtual Machine")

        return rc

    def StopVirtualMachine(self):
        subutai.SSHRun("sync")
        subutai.log("info", "Stopping Virtual machine")
        rc = 0
        if subutai.CheckVMRunning(self.name) == 0:
            rc = subutai.VBoxS("controlvm " + self.name + " poweroff soft")

        return rc

    def WaitSSH(self):
        subutai.log("info", "Waiting for machine to bring SSH")
        attempts = 0
        while subutai.TestSSH() != 0:
            sleep(1)
            attempts = attempts + 1
            if attempts == 30:
                subutai.RaiseError("SSH connection failed after 30 attempts")
                return 34

        attempts = 0
        out = ''
        while out == '':
            out = subutai.SSHRunOut("uptime")
            attempts = attempts + 1
            if attempts >= 30:
                subutai.RaiseError("SSH connection failed after 30 attempts")
                return 35

        return 0

    def InitBTRFS(self):
        subutai.AddStatus("Initializing BTRFS")
        subutai.SSHRun("sudo subutai-dev.btrfsinit /dev/sdb")
        return 0

    def SetupAlias(self):
        subutai.log("info", "Setting Alias")
        subutai.SSHRun("sudo bash -c 'snap alias subutai-dev subutai'")
        return 0

    def RetrievePeerIP(self):
        self.PeerIP = subutai.GetPeerIP()
        self.LocalIP = '127.0.0.1'

        if self.PeerIP == "":
            subutai.RaiseError("Failed to determine peer IP address")
            return 1

        return 0

    def GetPeerIP(self):
        return self.PeerIP

    def GetLocalIP(self):
        return self.LocalIP

    def WaitPeerResponse(self):
        ip = self.LocalIP
        if self.PeerIP != '':
            ip = self.PeerIP

        subutai.AddStatus("Waiting for peer at "+ip)
        attempts = 0
        while subutai.IsPeerReady(ip) != 0:
            sleep(2)
            attempts = attempts + 1
            if attempts >= 30:
                return 14
        return 0

    def InstallUbuntu(self):
        td = "/var/snap/subutai-dev/common/lxc/tmpdir/"
        awk = " | awk '{print $5}'"

        subutai.AddStatus("Downloading Ubuntu Linux")
        rc = subutai.SSHStartSession("mng-setup")

        if rc != 0:
            subutai.RaiseError("Failed to install Ubuntu interactively. Switching to static install")
            subutai.SSHRun("sudo subutai -d import ubuntu16 >/tmp/ubuntu16.log 2>&1")
            return 0

        rc = subutai.SSHExecute("mng-setup", "sudo subutai import ubuntu16 &")
        if rc[0] != 0:
            subutai.RaiseError("Failed to install Ubuntu in background. Switching to static install")
            subutai.SSHRun("sudo subutai -d import ubuntu16 >/tmp/ubuntu16.log 2>&1")
        else:
            checks = 0
            while True:
                out = subutai.SSHRunOut("ps -ef | grep \"subutai import\" | grep -v grep | awk '{print $2}'")
                if out == '':
                    subutai.AddStatus("Download completed")
                    break
                sleep(1)
                checks = checks + 1
                if checks >= 60:
                    subutai.SSHExecute("mng-setup", "ls / &")
                    checks = 0
                out = subutai.SSHRunOut("ls -l "+td+self.UbuntuFile+awk)
                try:
                    val = int(out)
                    self.progress.setUbuntuProgress(val)
                    self.progress.updateProgress()
                except:
                    pass

        subutai.SSHStopSession("mng-setup")
        return 0

    def InstallOpenJRE(self):
        td = "/var/snap/subutai-dev/common/lxc/tmpdir/"
        awk = " | awk '{print $5}'"

        rc = subutai.SSHStartSession("mng-setup2")
        if rc != 0:
            subutai.AddStatus("Downloading JVM")
            subutai.SSHRun("sudo subutai -d import openjre16 1>/tmp/openjre16-1.log 2>/tmp/openjre16-2.log")
            return 0

        subutai.AddStatus("Downloading JVM")
        rc = subutai.SSHExecute("mng-setup2", "sudo subutai import openjre16 &")
        if rc[0] != 0:
            subutai.RaiseError("Failed to install OpenJRE in background. Switching to static install")
            subutai.SSHRun("sudo subutai import openjre16 >/tmp/openjre16.log 2>&1")
        else:
            checks = 0
            while True:
                out = subutai.SSHRunOut("ps -ef | grep \"subutai import\" | grep -v grep | awk '{print $2}'")
                if out == '':
                    break
                sleep(1)
                checks = checks + 1
                if checks >= 60:
                    subutai.SSHExecute("mng-setup2", "ls / &")
                    checks = 0
                out = subutai.SSHRunOut("ls -l "+td+self.OpenJREFile+awk)
                try:
                    val = int(out)
                    self.progress.setOpenjreProgress(val)
                    self.progress.updateProgress()
                except:
                    pass

        subutai.SSHStopSession("mng-setup2")
        return 0

    def installManagement(self):
        td = "/var/snap/subutai-dev/common/lxc/tmpdir/"
        awk = " | awk '{print $5}'"

        rc = subutai.SSHStartSession("mng-setup3")
        if rc != 0:
            subutai.AddStatus("Installing Management Container")
            subutai.SSHRun("sudo subutai -d import management 1>/tmp/management-1.log 2>/tmp/management-2.log")
            return 0

        subutai.AddStatus("Installing Management Container")
        rc = subutai.SSHExecute("mng-setup3", "sudo subutai import management &")
        if rc[0] != 0:
            subutai.RaiseError("Failed to install Management in background. Switching to static install")
            subutai.SSHRun("sudo subutai import management >/tmp/management.log 2>&1")
        else:
            checks = 0
            while True:
                out = subutai.SSHRunOut("ps -ef | grep \"subutai import\" | grep -v grep | awk '{print $2}'")
                if out == '':
                    break
                sleep(1)
                checks = checks + 1
                if checks >= 60:
                    subutai.SSHExecute("mng-setup3", "ls / &")
                    checks = 0
                out = subutai.SSHRunOut("ls -l "+td+"*"+self.ManagementFile+"*"+awk)
                try:
                    val = int(out)
                    self.progress.setManagementProgress(val)
                    self.progress.updateProgress()
                except:
                    pass

        subutai.SSHStopSession("mng-setup3")
        return 0

    def InstallSnap(self):
        subutai.AddStatus("Installing Subutai. This may take a few minutes")
        subutai.SSHRun("sudo snap install --beta --devmode subutai-dev > /tmp/subutai-snap.log 2>&1")

        out = subutai.SSHRunOut("which subutai-dev >/dev/null; echo $?")
        if out != '0':
            return 55

        return 0

    def SetupSSH(self):
        subutai.log("info", "Setting up SSH")
        subutai.SSHRun("mkdir -p /home/subutai/.ssh")
        subutai.InstallSSHKey()
        return 0


class PostInstall:
    filename = ''
    tmpdir = ''
    def __init__(self, tmp):
        self.tmpdir = tmp
        m = hashlib.md5()
        m.update(datetime.datetime.now().isoformat().encode('utf-8'))
        self.filename = "postinst-" + m.hexdigest()[:5]
        if os.path.exists(tmp+self.filename):
            os.remove(tmp+self.filename)
        append("#!/bin/bash\n\n")

    def append(self, line):
        f = open(self.tmpdir+self.filename+"\n", 'a')
        f.write(line)
        f.close()
        
    def get(self):
        return self.tmpdir + self.filename

    def destroy(self):
        try:
            if os.path.exists(self.tmpdir+self.filename):
                os.remove(self.tmpdir+self.filename)
            return 0
        except:
            return -1

    
def DownloadP2P(p2p, progress):
    subutai.download(p2p)
    while subutai.isDownloadComplete() != 1:
        sleep(0.05)
        progress.setP2PProgress(subutai.GetBytesDownload())
        progress.updateProgress()

    progress.setP2PProgress(subutai.getP2PSize())

    return 0