import subutai
import hashlib
from time import sleep
from subprocess import call


def subutaistart():

    m = hashlib.md5()
    m.update(b'subutaivm')
    machineName = "subutai-w" + m.hexdigest()

    #call(['ssh-keygen.exe', '-R', '[127.0.0.1]:4567'])

    subutai.SetSSHCredentials("subutai", "ubuntai", "127.0.0.1", 4567)

    setupVm(machineName)
    subutai.SetProgress(4.0)
    sleep(6)
    startVm(machineName)
    sleep(60)
    waitSSH()
    sleep(60)
    setupSSH()
    installSnapFromStore()
    subutai.SetProgress(10.0)
    sleep(60)
    initBtrfs()
    subutai.SetProgress(20.0)
    sleep(60)
    setAlias()
    subutai.SetProgress(30.0)
    sleep(10)
    installManagement()
    subutai.SetProgress(80.0)
    sleep(60)
    stopVm(machineName)
    sleep(5)
    reconfigureNic(machineName)
    sleep(5)
    startVm(machineName)

    subutai.Shutdown()

    return


def waitSSH():
    subutai.log("info", "Waiting for machine to bring SSH")
    attempts = 0
    while subutai.TestSSH() != 0:
        sleep(1)
        attempts = attempts + 1
        if attempts == 30:
            subutai.log("error", "SSH timeout for 30 second")
            return
    subutai.log("info", "SSH Connected")
    return


def installManagement():
    subutai.AddStatus("Installing Management Container")
    subutai.log("info", "Installing management")
    subutai.SSHRun("sudo subutai -d import management")

    return


def installSnapFromStore():
    subutai.AddStatus("Installing Subutai")
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
    subutai.log("info", "Stopping Virtual machine")
    if subutai.CheckVMRunning(machineName) != 0:
        subutai.VBox("controlvm " + machineName + " poweroff soft")

    return


def setupVm(machineName):
    subutai.log("info", "Setting up a VM")
    subutai.AddStatus("Installing VM")
    if subutai.CheckVMExists(machineName) != 0:
        subutai.download("core.ova")
        while subutai.isDownloadComplete() != 1:
            sleep(0.05)
        subutai.VBox("import " + subutai.GetTmpDir().replace(" ", "+++") + "core.ova")
        subutai.VBox("modifyvm core --cpus 2")
        subutai.VBox("modifyvm core --nic1 nat")
        subutai.VBox("modifyvm core --cableconnected1 on")
        subutai.VBox("modifyvm core --natpf1 ssh-fwd,tcp,,4567,,22 --natpf1 https-fwd,tcp,,9999,,8443")
        subutai.VBox("modifyvm core --rtcuseutc on")
        adapterName = "VirtualBox+++Host-Only+++Ethernet+++Adapter"
        subutai.VBox("modifyvm core --nic3 hostonly --hostonlyadapter3 "+adapterName)
        ret = subutai.VBoxS("modifyvm core --name " + machineName)
        if ret != 0:
            subutai.log("error", "Machine already exists")

    return


def reconfigureNic(machineName):
    subutai.AddStatus("Configuring Network")
    subutai.log("info", "Reconfiguring NIC")
    gateway = subutai.GetDefaultRoutingInterface()
    bridged = subutai.GetVBoxBridgedInterface(gateway)
    subutai.VBox("modifyvm " + machineName + ' --nic1 bridged --bridgeadapter1 ' + bridged)
    subutai.VBox("modifyvm " + machineName + " --nic2 nat")
    subutai.VBox("modifyvm " + machineName + " --cableconnected2 on")
    subutai.VBox("modifyvm " + machineName + ' --natpf2 ssh-fwd,tcp,,4567,,22 --natpf2 https-fwd,tcp,,9999,,8443')

    adapterName = "VirtualBox+++Host-Only+++Ethernet+++Adapter"
    ret = subutai.VBoxS("hostonlyif ipconfig " + adapterName + " --ip 192.168.56.1")

    if ret == 1:
        subutai.VBox("hostonlyif create")
        subutai.VBox("hostonlyif ipconfig " + adapterName + " --ip 192.168.56.1")
        subutai.VBox("dhcpserver add --ifname " + adapterName + " --ip 192.168.56.1 --netmask 255.255.255.0 --lowerip 192.168.56.100 --upperip 192.168.56.200")
        subutai.VBox("dhcpserver modify --ifname " + adapterName + " --enable")

    subutai.VBox("modifyvm " + machineName + " --nic3 hostonly --hostonlyadapter3 " + adapterName)

    return


def waitSnap():

    return
