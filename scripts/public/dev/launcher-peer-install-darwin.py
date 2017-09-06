import subutai
import subuco
import subud as subup
from time import sleep


def subutaistart():
    coreFile = "core.ova"
    vboxFile = "VirtualBox.pkg"
    ubuntuFile = "ubuntu16-subutai-template_4.0.0_amd64.tar.gz"
    openjreFile = "openjre16-subutai-template_4.0.0_amd64.tar.gz"
    mngFile = "management"
    progress = subuco.Progress(coreFile,
                               vboxFile,
                               ubuntuFile,
                               openjreFile,
                               mngFile)

    tmpDir = subutai.GetTmpDir()
    installDir = subutai.GetInstallDir()
    rc = subup.InstallPeerPrerequisites(tmpDir, installDir, progress)
    if rc != 0:
        subutai.RaiseError("Failed to install prerequisites")
        sleep(10)
        return 29

    sleep(3)

    subup.CleanSSHKeys()
    subutai.SetSSHCredentials("subutai", "ubuntai", "127.0.0.1", 4567)

    peer = subuco.SubutaiPeer(subup.GetVirtualMachineName(),
                              progress,
                              ubuntuFile,
                              openjreFile,
                              mngFile)

    enableHostonlyif()
    if peer.SetupVirtualMachine() != 0:
        sleep(10)
        subutai.Shutdown()
        return

    peer.ConfigureNetwork()

    rc = peer.StartVirtualMachine()

    subutai.AddStatus("Waiting for peer to start and initialize")
    sleep(40)
    if subutai.CheckVMRunning(peer.GetName()) != 0:
        peer.StartVirtualMachine()
        sleep(50)

    if subutai.CheckVMRunning(peer.GetName()) != 0:
        subutai.RaiseError("Aborting")
        sleep(15)
        subutai.Shutdown()
        return 21

    progress.spin()
    rc = peer.WaitSSH()
    if rc != 0:
        sleep(10)
        subutai.Shutdown()
        return rc

    setupSSH()
    rc = peer.waitForNetwork()
    if rc != 0:
        sleep(10)
        return rc
    rc = peer.InstallSnap()
    if rc != 0:
        subutai.RaiseError("Failed to install Subutai. Aborting")
        sleep(10)
        subutai.Shutdown()
        return rc
    peer.InitBTRFS()
    peer.SetupAlias()
    peer.RetrievePeerIP()
    progress.unspin()
    peer.InstallUbuntu()
    peer.installOpenJRE()
    peer.installManagement()
    peer.WaitPeerResponse()
    subutai.SetProgress(1.0)
    subutai.Shutdown()

    return 0


def setupSSH():
    subutai.log("info", "Setting up SSH")
    subutai.SSHRun("mkdir -p /home/subutai/.ssh")
    subutai.InstallSSHKey()
    return


def enableHostonlyif():
    adapterName = subutai.GetVBoxHostOnlyInterface()

    if adapterName == 'undefined':
        subutai.VBox("hostonlyif create")
        adapterName = subutai.GetVBoxHostOnlyInterface()
        subutai.VBox("hostonlyif ipconfig " + adapterName + " --ip 192.168.56.1")
        out = subutai.VBox("list dhcpservers")
        if out == '':
            subutai.VBox("dhcpserver add --ifname " +
                         adapterName +
                         " --ip 192.168.56.1 --netmask 255.255.255.0 --lowerip 192.168.56.100 --upperip 192.168.56.200")
            subutai.VBox("dhcpserver modify --ifname " + adapterName + " --enable")

    return 0
