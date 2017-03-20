import subutai
from time import sleep

def subutaistart():
    subutai.download("snappy.ova")
    while subutai.isDownloadComplete() != 1:
        sleep(0.05)

    subutai.download("subutai_4.0.14_amd64.snap")
    while subutai.isDownloadComplete() != 1:
        sleep(0.05)

    machineName = "unit-test-subutai"

    # Configuring virtual machine
    subutai.VBox("import /tmp/subutai/snappy.ova")
    subutai.VBox("modifyvm snappy --cpus 2")
    subutai.VBox("modifyvm snappy --nic1 nat")
    subutai.VBox("modifyvm snappy --cableconnected1 on")
    subutai.VBox("modifyvm snappy --natfp1 \"ssh-fwd,tcp,,4567,,22\" --matpf1 \"https-fwd,tcp,,9999,,8443\"")
    subutai.VBox("modifyvm snappy --rtcuseutc on")
    ret = subutai.VBoxS("modifyvm snappy --name " + machineName)
    if ret != 0:
        print(machineName + " is already exists")

    subutai.VBox("startvm --type headless " + machineName)

    reconfigureNic(machineName)
    return;

def reconfigureNic( machineName ):
    print ("Reconfiguring NIC on: " + machineName)

    subutai.VBox("controlvm " + machineName + " poweroff soft")

    gateway = subutai.GetDefaultRoutingInterface()
    print ("Default gateway interface: '" + gateway + "'")

    bridged = subutai.GetVBoxBridgedInterface(gateway)
    print ("Bridged interface: '" + bridged + "'")

    subutai.VBox("modifyvm " + machineName + ' --nic1 bridged --bridgeadapter1 "' + bridged + '"')
    subutai.VBox("modifyvm " + machineName + " --nic2 nat")
    subutai.VBox("modifyvm " + machineName + " --cableconnected2 on")
    subutai.VBox("modifyvm " + machineName + ' --natpf2 "ssh-fwd,tcp,,4567,,22" --natpf2 "https-fwd,tcp,,9999,,8443"')

    ret = subutai.VBoxS("hostonlyif ipconfig vboxnet0 --ip 192.168.56.1")
    print("hostonlyif: " + str(ret))

    if ret == 1:
        print("Creating hostonlyif")
        subutai.VBox("hostonlyif create")
        subutai.VBox("hostonlyif ipconfig vboxnet0 --ip 192.168.56.1")
        subutai.VBox("dhcpserver add --ifname vboxnet0 --ip 192.168.56.1 --netmask 255.255.255.0 --lowerip 192.168.56.100 --upperip 192.168.56.200")
        subutai.VBox("dhcpserver modify --ifname vboxnet0 --enable")

    subutai.VBox("modifyvm subutai --nic3 hostonly --hostonlyadapter3 vboxnet0")
    subutai.VBox("startvm --type headless " + machineName)

    return;
