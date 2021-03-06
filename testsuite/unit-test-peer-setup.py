import subutai
from time import sleep
from subprocess import call

def subutaistart():

    machineName = "subutai-unit-test-1"

    call(['ssh-keygen', '-R', '[127.0.0.1]:4567'])

    subutai.SetSSHCredentials("subutai", "ubuntai", "127.0.0.1", 4567)

    setupVm(machineName)
    startVm(machineName)
    waitSSH()
    sleep(60)
    installSnapFromStore()
    sleep(60)
    initBtrfs()
    sleep(60)
    setAlias()
    sleep(60)
    #installSubutai("", "", "", 0)
    reconfigureNic(machineName)
    sleep(10)
    stopVm(machineName)
    sleep(20)
    startVm(machineName)
    sleep(20)
    waitSSH()
    sleep(20)
    installManagement()
    sleep(60)
    setupSSH()

    return;

def waitSSH():
    print("Waiting for machine to bring up SSH")
    attempts = 0
    while subutai.TestSSH() != 0:
        sleep(1)
        attempts = attempts + 1
        if attempts == 30:
            print("SSH timeout for 30 seconds")
            return
    print("SSH Connected")
    return;

def installManagement():
    print("Waiting for machine to bring up SSH")
    attempts = 0
    while subutai.TestSSH() != 0:
        sleep(1)
        attempts = attempts + 1
        if attempts == 30:
            print("SSH timeout for 30 seconds")
            return

    print("Importing management")
    subutai.SSHRun("sudo subutai -d import management")

    return;

def installSnapFromStore():
    print("Installing snap from store")
    subutai.SSHRun("sudo snap install --beta --devmode subutai-dev")

    return;

def initBtrfs():
    print("Initializing btrfs")
    subutai.SSHRun("sudo subutai-dev.btrfsinit /dev/sdb")

    return;

def setAlias():
    print("Setting alias")
    subutai.SSHRun("sudo bash -c 'snap alias subutai-dev subutai'")

    return

def setupSSH():

    print("Waiting for machine to bring up SSH")
    attempts = 0
    while subutai.TestSSH() != 0:
        sleep(1)
        attempts = attempts + 1
        if attempts == 30:
            print("SSH timeout for 30 seconds")
            return

    print("SSH Session is running")
    subutai.SSHRun("mkdir -p /home/subutai/.ssh")
    subutai.InstallSSHKey()

    return;

def startVm( machineName ):

    if subutai.CheckVMRunning(machineName) != 0:
        subutai.VBox("startvm --type headless " + machineName)

    return;

def stopVm( machineName ):

    if subutai.CheckVMRunning(machineName) != 0:
        subutai.VBox("controlvm " + machineName + " poweroff soft")

    return;

def setupVm( machineName ):
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
            print(machineName + " is already exists")

    return;

def installSubutai( snapFile, user, host, port ):
    subutai.download("launcher-prepare-server")
    while subutai.isDownloadComplete() != 1:
        sleep(0.05)

    call(['/usr/bin/scp', '-P4567', '-o', 'StrictHostKeyChecking=no', '/tmp/subutai/launcher-prepare-server', 'ubuntu@127.0.0.1:~/prepare-server'])
    call(['/usr/bin/scp', '-P4567', '-o', 'StrictHostKeyChecking=no', '/tmp/subutai/subutai_4.0.15_amd64-dev.snap', 'ubuntu@127.0.0.1:~/subutai_latest.snap'])

    subutai.SSHRun("sudo chmod +x /home/ubuntu/prepare-server")
    subutai.SSHRun("sudo /home/ubuntu/prepare-server")

    return;

def reconfigureNic( machineName ):
    print ("Reconfiguring NIC on: " + machineName)

    subutai.VBox("controlvm " + machineName + " poweroff soft")

    gateway = subutai.GetDefaultRoutingInterface()
    print ("Default gateway interface: '" + gateway + "'")

    bridged = subutai.GetVBoxBridgedInterface(gateway)
    print ("Bridged interface: '" + bridged + "'")

    subutai.VBox("modifyvm " + machineName + ' --nic1 bridged --bridgeadapter1 ' + bridged)
    subutai.VBox("modifyvm " + machineName + " --nic2 nat")
    subutai.VBox("modifyvm " + machineName + " --cableconnected2 on")
    subutai.VBox("modifyvm " + machineName + ' --natpf2 ssh-fwd,tcp,,4567,,22 --natpf2 https-fwd,tcp,,9999,,8443')

    ret = subutai.VBoxS("hostonlyif ipconfig vboxnet0 --ip 192.168.56.1")
    print("hostonlyif: " + str(ret))

    if ret == 1:
        print("Creating hostonlyif")
        subutai.VBox("hostonlyif create")
        subutai.VBox("hostonlyif ipconfig vboxnet0 --ip 192.168.56.1")
        subutai.VBox("dhcpserver add --ifname vboxnet0 --ip 192.168.56.1 --netmask 255.255.255.0 --lowerip 192.168.56.100 --upperip 192.168.56.200")
        subutai.VBox("dhcpserver modify --ifname vboxnet0 --enable")

    subutai.VBox("modifyvm " + machineName + " --nic3 hostonly --hostonlyadapter3 vboxnet0")
    subutai.VBox("startvm --type headless " + machineName)

    return;

def waitSnap():

    return;
