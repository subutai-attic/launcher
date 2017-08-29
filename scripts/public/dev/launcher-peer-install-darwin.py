import subutai
import hashlib
from time import sleep
import datetime
from subprocess import call
from shutil import copyfile
import os
import stat


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
        self.coreSize = subutai.GetFileSize(core)
        self.vboxSize = subutai.GetFileSize(vbox)
        self.ubuntuSize = subutai.GetTemplateSize(ubuntu)
        self.openjreSize = subutai.GetTemplateSize(openjre)
        self.managementSize = subutai.GetTemplateSize(management)
        self.totalSize = self.coreSize + self.vboxSize + self.ubuntuSize + self.openjreSize + self.managementSize

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
        self.coreProgress = s

    def setOpenjreProgress(self, s):
        self.openjreProgress = s

    def setManagementProgress(self, s):
        self.managementProgress = s

    def updateProgress(self):
        if self.totalSize == 0:
            return
        cur = self.coreProgress + self.vboxProgress + self.ubuntuProgress + self.openjreProgress + self.managementProgress
        val = (int)(100 * cur) / self.totalSize
        p = (float)(val/100)
        self.lastProgress = p
        subutai.SetProgress(p)

    def spin(self):
        subutai.SetProgress(-1.0)

    def unspin(self):
        subutai.SetProgress(self.lastProgress)



def installCocoasudo(tmpDir, installDir):
    if not os.path.exists(installDir+"bin/cocoasudo"):
        subutai.AddStatus("Downloading cocoasudo application")
        subutai.download("cocoasudo")
        while subutai.isDownloadComplete() != 1:
            sleep(0.05)

        try:
            copyfile(tmpDir+"cocoasudo", installDir+"bin/cocoasudo")
            st = os.stat(installDir+"bin/cocoasudo")
            os.chmod(installDir+"bin/cocoasudo", st.st_mode | stat.S_IEXEC)
        except:
            subutai.RaiseError("Failed to install cocoasudo. Aborting")
            sleep(10)
            return -99


def installVBox(tmpDir, installDir, progress):
    if not os.path.exists("/Applications/VirtualBox.app"):
        subutai.AddStatus("Downloading VirtualBox")
        subutai.download("VirtualBox.pkg")
        while subutai.isDownloadComplete() != 1:
            sleep(0.05)
            progress.setVboxProgress(subutai.GetBytesDownload())
            progress.updateProgress()

        subutai.AddStatus("Installing VirtualBox")
        try:
            call([installDir+"bin/cocoasudo",
                  '--prompt="Install VirtualBox"',
                  'installer',
                  '-pkg',
                  tmpDir+'VirtualBox.pkg',
                  '-target',
                  '/'])
        except:
            subutai.RaiseError("Failed to install VirtualBox. Aborting")
            sleep(10)
            return 45

    progress.setVboxProgress(progress.getVboxSize())
    progress.updateProgress()


def subutaistart():
    coreFile = "core.ova"
    vboxFile = "VirtualBox.pkg"
    ubuntuFile = "ubuntu16-subutai-template_4.0.0_amd64.tar.gz"
    openjreFile = "openjre16-subutai-template_4.0.0_amd64.tar.gz"
    mngFile = "management"
    progress = Progress(coreFile, vboxFile, ubuntuFile, openjreFile, mngFile)

    tmpDir = subutai.GetTmpDir()
    installDir = subutai.GetInstallDir()

    installCocoasudo(tmpDir, installDir)
    installVBox(tmpDir, installDir, progress)

    m = hashlib.md5()
    m.update(datetime.datetime.now().isoformat().encode('utf-8'))
    machineName = "subutai-dd-" + m.hexdigest()[:5]

    call(['ssh-keygen', '-R', '[127.0.0.1]:4567'])

    subutai.SetSSHCredentials("subutai", "ubuntai", "127.0.0.1", 4567)

    enableHostonlyif()

    if setupVm(machineName, progress) != 0:
        subutai.RaiseError("Failed to install Virtual Machine. See the logs for details")
        subutai.Shutdown()
        return

    startVm(machineName)
    subutai.AddStatus("Waiting for peer to start and initialize")
    sleep(40)
    if subutai.CheckVMRunning(machineName) != 0:
        subutai.AddStatus("Failed to start VM. Retrying")
        startVm(machineName)
        sleep(50)

    if subutai.CheckVMRunning(machineName) != 0:
        subutai.RaiseError("Failed to start VM. Aborting")
        sleep(15)
        return 21

    rc = waitSSH()
    if rc != 0:
        return rc

    setupSSH()
    progress.spin()
    installSnapFromStore()
    initBtrfs()
    setAlias()
    peerip = GetPeerIP()
    progress.unspin()
    installUbuntu(ubuntuFile, progress)
    installOpenjre(openjreFile, progress)
    installManagement(mngFile, progress)
    WaitForPeer(peerip)

    sleep(3)
    stopVm(machineName)
    sleep(5)
    if subutai.CheckVMRunning(machineName) == 0:
        subutai.AddStatus("Failed to stop VM. Retrying")
        stopVm(machineName)
        sleep(20)

    if subutai.CheckVMRunning(machineName) == 0:
        subutai.RaiseError("Failed to stop VM. Retrying")
        sleep(20)
        return 22

    reconfigureNic(machineName)
    sleep(3)
    startVm(machineName)
    sleep(10)
    if subutai.CheckVMRunning(machineName) != 0:
        subutai.AddStatus("Failed to start VM. Retrying")
        startVm(machineName)
        sleep(30)

    if subutai.CheckVMRunning(machineName) != 0:
        subutai.RaiseError("Failed to start VM. Aborting")
        sleep(15)
        return 21

    subutai.SetProgress(1.0)
    subutai.Shutdown()

    return 0


def waitSSH():
    subutai.log("info", "Waiting for machine to bring SSH")
    attempts = 0
    while subutai.TestSSH() != 0:
        sleep(1)
        attempts = attempts + 1
        if attempts == 30:
            subutai.RaiseError("SSH connection failed after 30 attempts")
            subutai.log("error", "SSH timeout for 30 second")
            return 34
    subutai.log("info", "SSH Connected")
    return 0


def GetPeerIP():
    ip = subutai.GetPeerIP()

    if ip == "":
        subutai.RaiseError("Failed to determine peer IP address")
        return "127.0.0.1"

    ip = "127.0.0.1"
    return ip


def WaitForPeer(ip):
    attempts = 0
    while subutai.IsPeerReady(ip) != 0:
        sleep(2)
        attempts = attempts + 1
        if attempts >= 30:
            break
    return


def installUbuntu(ubuntuFile, progress):
    td = "/var/snap/subutai-dev/common/lxc/tmpdir/"
    awk = " | awk '{print $5}'"

    subutai.AddStatus("Downloading Ubuntu Linux")
    rc = subutai.SSHStartSession("mng-setup")

    if rc != 0:
        subutai.RaiseError("Failed to install Ubuntu interactively. Switching to static install")
        subutai.SSHRun("sudo subutai -d import ubuntu16 >/tmp/ubuntu16.log 2>&1")
        return 0

    #rc = subutai.SSHExecute("mng-setup", "sudo subutai import -d ubuntu16 >/tmp/ubuntu16.log 2>&1 &")
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
            out = subutai.SSHRunOut("ls -l "+td+ubuntuFile+awk)
            try:
                val = int(out)
                progress.setUbuntuProgress(val)
                progress.updateProgress()
            except:
                pass

    subutai.SSHStopSession("mng-setup")


def installOpenjre(openjreFile, progress):
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
            out = subutai.SSHRunOut("ls -l "+td+openjreFile+awk)
            try:
                val = int(out)
                progress.setOpenjreProgress(val)
                progress.updateProgress()
            except:
                pass

    subutai.SSHStopSession("mng-setup2")


def installManagement(mngFile, progress):
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
            out = subutai.SSHRunOut("ls -l "+td+"*"+mngFile+"*"+awk)
            try:
                val = int(out)
                progress.setManagementProgress(val)
                progress.updateProgress()
            except:
                pass

    subutai.SSHStopSession("mng-setup3")
    return


def installSnapFromStore():
    subutai.AddStatus("Installing Subutai. This may take a few minutes")
    subutai.log("info", "Installing subutai snap")
    subutai.SSHRun("sudo snap install --beta --devmode subutai-dev")

    return


def initBtrfs():
    subutai.log("info", "Initializing BTRFS")
    subutai.AddStatus("Initializing BTRFS")
    subutai.SSHRun("sudo subutai-dev.btrfsinit /dev/sdb")

    return


def setAlias():
    subutai.log("info", "Setting Alias")
    subutai.SSHRun("sudo bash -c 'snap alias subutai-dev subutai'")
    return


def setupSSH():
    subutai.log("info", "Setting up SSH")
    subutai.SSHRun("mkdir -p /home/subutai/.ssh")
    subutai.InstallSSHKey()
    return


def startVm(machineName):
    subutai.log("info", "Starting virtual machine")
    if subutai.CheckVMRunning(machineName) != 0:
        subutai.VBox("startvm --type headless " + machineName)

    return


def stopVm(machineName):
    subutai.SSHRun("sync")
    subutai.log("info", "Stopping Virtual machine")
    if subutai.CheckVMRunning(machineName) == 0:
        subutai.VBox("controlvm " + machineName + " poweroff soft")

    return


def setupVm(machineName, progress):
    subutai.log("info", "Setting up a VM")
    subutai.AddStatus("Installing VM")
    if subutai.CheckVMExists(machineName) != 0:
        subutai.download("core.ova")
        while subutai.isDownloadComplete() != 1:
            sleep(0.05)
            progress.setCoreProgress(subutai.GetBytesDownload())
            progress.updateProgress()

        subutai.AddStatus("VM Image downloaded")

    progress.setCoreProgress(progress.getCoreSize())
    progress.updateProgress()
    subutai.VBox("import " +
                 subutai.GetTmpDir().replace(" ", "+++") + "core.ova --vsys 0 --vmname "+machineName)
    sleep(3)

    cpus = subutai.GetCoreNum()
    mem = subutai.GetMemSize() * 1024

    subutai.VBox("modifyvm " + machineName + " --cpus " + str(cpus))
    subutai.VBox("modifyvm " + machineName + " --memory " + str(mem))
    subutai.VBox("modifyvm " + machineName + " --nic1 nat")
    subutai.VBox("modifyvm " + machineName + " --cableconnected1 on")
    subutai.VBox("modifyvm " + machineName + " --natpf1 ssh-fwd,tcp,,4567,,22 --natpf1 https-fwd,tcp,,9999,,8443")
    subutai.VBox("modifyvm " + machineName + " --rtcuseutc on")
    adapterName = subutai.GetVBoxHostOnlyInterface()
    adapterName = adapterName.replace(' ', '+++')
    subutai.VBox("modifyvm " + machineName + " --nic3 hostonly --hostonlyadapter3 " + adapterName)
    sleep(1)

    return 0


def reconfigureNic(machineName):
    subutai.AddStatus("Configuring Network")
    subutai.log("info", "Reconfiguring NIC")
    gateway = subutai.GetDefaultRoutingInterface()
    bridged = subutai.GetVBoxBridgedInterface(gateway)
    subutai.VBox("modifyvm " + machineName + ' --nic1 bridged --bridgeadapter1 ' + bridged)
    subutai.VBox("modifyvm " + machineName + " --nic2 nat")
    subutai.VBox("modifyvm " + machineName + " --cableconnected2 on")
    subutai.VBox("modifyvm " + machineName + ' --natpf2 ssh-fwd,tcp,,4567,,22 --natpf2 https-fwd,tcp,,9999,,8443')

    adapterName = subutai.GetVBoxHostOnlyInterface()
    ret = subutai.VBoxS("hostonlyif ipconfig " + adapterName + " --ip 192.168.56.1")

    if ret == 1:
        subutai.VBox("hostonlyif create")
        subutai.VBox("hostonlyif ipconfig " + adapterName + " --ip 192.168.56.1")
        subutai.VBox("dhcpserver add --ifname " + adapterName + " --ip 192.168.56.1 --netmask 255.255.255.0 --lowerip 192.168.56.100 --upperip 192.168.56.200")
        subutai.VBox("dhcpserver modify --ifname " + adapterName + " --enable")

    subutai.VBox("modifyvm " + machineName + " --nic3 hostonly --hostonlyadapter3 " + adapterName)

    return


def enableHostonlyif():
    adapterName = subutai.GetVBoxHostOnlyInterface()

    if adapterName == 'undefined':
        subutai.VBox("hostonlyif create")
        adapterName = subutai.GetVBoxHostOnlyInterface()
        subutai.VBox("hostonlyif ipconfig " + adapterName + " --ip 192.168.56.1")
        subutai.VBox("dhcpserver add --ifname " + adapterName + " --ip 192.168.56.1 --netmask 255.255.255.0 --lowerip 192.168.56.100 --upperip 192.168.56.200")
        subutai.VBox("dhcpserver modify --ifname " + adapterName + " --enable")

    return
