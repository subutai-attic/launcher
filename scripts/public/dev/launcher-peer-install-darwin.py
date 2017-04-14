import subutai
from time import sleep
from subprocess import call


def subutaistart():

    machineName = "subutai-peer-1"

    call(['ssh-keygen', '-R', '[127.0.0.1]:4567'])

    subutai.SetSSHCredentials("subutai", "ubuntai", "127.0.0.1", 4567)

    setupVm(machineName)
    subutai.SetProgress(4.0)
    startVm(machineName)
    subutai.SetProgress(7.0)
    waitSSH()
    subutai.SetProgress(8.0)
    sleep(60)
    installSnapFromStore()
    subutai.SetProgress(10.0)
    sleep(60)
    initBtrfs()
    subutai.SetProgress(20.0)
    sleep(60)
    setAlias()
    subutai.SetProgress(30.0)
    sleep(60)
    # installSubutai("", "", "", 0)
    reconfigureNic(machineName)
    subutai.SetProgress(40.0)
    sleep(10)
    stopVm(machineName)
    subutai.SetProgress(50.0)
    sleep(20)
    startVm(machineName)
    subutai.SetProgress(60.0)
    sleep(20)
    waitSSH()
    subutai.SetProgress(70.0)
    sleep(20)
    installManagement()
    subutai.SetProgress(80.0)
    sleep(60)
    waitSSH()
    subutai.SetProgress(90.0)
    setupSSH()
    subutai.SetProgress(99.0)

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
        subutai.download("subutai_4.0.15_amd64-dev.snap")
        while subutai.isDownloadComplete() != 1:
            sleep(0.05)
        subutai.VBox("import /tmp/subutai/core.ova")
        subutai.VBox("modifyvm core --cpus 2")
        subutai.VBox("modifyvm core --nic1 nat")
        subutai.VBox("modifyvm core --cableconnected1 on")
        subutai.VBox("modifyvm core --natpf1 ssh-fwd,tcp,,4567,,22 --natpf1 https-fwd,tcp,,9999,,8443")
        subutai.VBox("modifyvm core --rtcuseutc on")
        ret = subutai.VBoxS("modifyvm core --name " + machineName)
        if ret != 0:
            subutai.log("error", "Machine already exists")

    return


def installSubutai(snapFile, user, host, port):
    subutai.download("launcher-prepare-server")
    while subutai.isDownloadComplete() != 1:
        sleep(0.05)

    call(['/usr/bin/scp', '-P4567', '-o', 'StrictHostKeyChecking=no', '/tmp/subutai/launcher-prepare-server', 'ubuntu@127.0.0.1:~/prepare-server'])
    call(['/usr/bin/scp', '-P4567', '-o', 'StrictHostKeyChecking=no', '/tmp/subutai/subutai_4.0.15_amd64-dev.snap', 'ubuntu@127.0.0.1:~/subutai_latest.snap'])

    subutai.SSHRun("sudo chmod +x /home/ubuntu/prepare-server")
    subutai.SSHRun("sudo /home/ubuntu/prepare-server")

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

    ret = subutai.VBoxS("hostonlyif ipconfig vboxnet0 --ip 192.168.56.1")

    if ret == 1:
        subutai.VBox("hostonlyif create")
        subutai.VBox("hostonlyif ipconfig vboxnet0 --ip 192.168.56.1")
        subutai.VBox("dhcpserver add --ifname vboxnet0 --ip 192.168.56.1 --netmask 255.255.255.0 --lowerip 192.168.56.100 --upperip 192.168.56.200")
        subutai.VBox("dhcpserver modify --ifname vboxnet0 --enable")

    subutai.VBox("modifyvm " + machineName + " --nic3 hostonly --hostonlyadapter3 vboxnet0")
    subutai.VBox("startvm --type headless " + machineName)

    return


def waitSnap():

    return
