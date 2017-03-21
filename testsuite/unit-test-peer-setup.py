import subutai
from time import sleep
from subprocess import call

def subutaistart():

    machineName = "subutai-unit-test-1"

    setupVm(machineName)
    startVm(machineName)
    setupSSH()
    installSubutai("", "", "", 0)
    reconfigureNic(machineName)
    installManagement()

    return;

def installManagement():
    print("Waiting for machine to bring up SSH")
    attempts = 0
    while subutai.TestSSH("ubuntu", "localhost", 4567) != 0:
        sleep(1)
        attempts = attempts + 1
        if attempts == 30:
            print("SSH timeout for 30 seconds")
            return

    subutai.SSHRun("ubuntu", "localhost", 4567, "sudo subutai import management", 1)

    return;


def setupSSH():

    print("Waiting for machine to bring up SSH")
    attempts = 0
    while subutai.TestSSH("ubuntu", "localhost", 4567) != 0:
        sleep(1)
        attempts = attempts + 1
        if attempts == 30:
            print("SSH timeout for 30 seconds")
            return

    print("SSH Session is running")
    subutai.InstallSSHKey("ubuntu", "localhost", 4567)

    return;

def startVm( machineName ):

    if subutai.CheckVMRunning(machineName) != 0:
        subutai.VBox("startvm --type headless " + machineName)

    return;

def setupVm( machineName ):
    if subutai.CheckVMExists(machineName) != 0:
        subutai.download("snappy.ova")
        while subutai.isDownloadComplete() != 1:
            sleep(0.05)
        subutai.download("subutai_4.0.14_amd64.snap")
        while subutai.isDownloadComplete() != 1:
            sleep(0.05)
        subutai.VBox("import /tmp/subutai/snappy.ova")
        subutai.VBox("modifyvm snappy --cpus 2")
        subutai.VBox("modifyvm snappy --nic1 nat")
        subutai.VBox("modifyvm snappy --cableconnected1 on")
        subutai.VBox("modifyvm snappy --natpf1 ssh-fwd,tcp,,4567,,22 --natpf1 https-fwd,tcp,,9999,,8443")
        subutai.VBox("modifyvm snappy --rtcuseutc on")
        ret = subutai.VBoxS("modifyvm snappy --name " + machineName)
        if ret != 0:
            print(machineName + " is already exists")

    return;

def installSubutai( snapFile, user, host, port ):
    subutai.download("launcher-prepare-server")
    while subutai.isDownloadComplete() != 1:
        sleep(0.05)

    call(['/usr/bin/scp', '-P4567', '-o', 'StrictHostKeyChecking=no', '/tmp/subutai/launcher-prepare-server', 'ubuntu@localhost:~/prepare-server'])
    call(['/usr/bin/scp', '-P4567', '-o', 'StrictHostKeyChecking=no', '/tmp/subutai/subutai_4.0.14_amd64.snap', 'ubuntu@localhost:~/subutai_latest.snap'])

    subutai.SSHRun("ubuntu", "localhost", 4567, "sudo chmod +x /home/ubuntu/prepare-server", 1)
    subutai.SSHRun("ubuntu", "localhost", 4567, "sudo /home/ubuntu/prepare-server", 1)

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
    subutai.VBox("modifyvm " + machineName + ' --natpf2 ssh-fwd,tcp,,4568,,22 --natpf2 https-fwd,tcp,,9999,,8443')

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
