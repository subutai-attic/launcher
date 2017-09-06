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
    coreProgress = 0
    vboxProgress = 0
    ubuntuProgress = 0
    openjreProgress = 0
    managementProgress = 0
    totalSize = 0
    lastProgress = 0.0

    def __init__(self, core, vbox, ubuntu, openjre, management):
        self.lastProgress = -1.0
        t = 0
        self.coreSize = subutai.GetFileSize(core)
        t = t + self.coreSize
        self.vboxSize = subutai.GetFileSize(vbox)
        t = t + self.vboxSize
        self.ubuntuSize = subutai.GetTemplateSize(ubuntu)
        t = t + self.ubuntuSize
        self.openjreSize = subutai.GetTemplateSize(openjre)
        t = t + self.openjreSize
        self.managementSize = subutai.GetTemplateSize(management)
        t = t + self.managementSize
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

    def updateProgress(self):
        if self.totalSize == 0:
            return
        cur = self.coreProgress
        cur = cur + self.vboxProgress
        cur = cur + self.ubuntuProgress
        cur = cur + self.openjreProgress
        cur = cur + self.managementProgress
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
