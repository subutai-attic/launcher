import subutai
from time import sleep
import zipfile


def subutaistart():
    subutai.AddStatus("Download Tray application")

    tray = "SubutaiTray_libs.zip"
    sshlib = "ssh.zip"

    subutai.download(tray)
    while subutai.isDownloadComplete() != 1:
        sleep(0.05)

    subutai.download(sshlib)
    while subutai.isDownloadComplete() != 1:
        sleep(0.05)

    tmpDir = subutai.GetTmpDir()
    installDir = subutai.GetInstallDir()

    subutai.AddStatus("Installing Tray")

    subutai.ProcessKill("SubutaiTray.exe")
    subutai.ProcessKill("ssh.exe")
    subutai.ProcessKill("ssh-keygen.exe")

    zf = zipfile.ZipFile(tmpDir+"/"+tray, 'r')
    zf.extractall(installDir)
    zf.close()

    zfl = zipfile.ZipFile(tmpDir+"/"+sshlib, 'r')
    zfl.extractall(installDir+"/bin")
    zfl.close()

    subutai.CreateDesktopShortcut(installDir+"/tray/SubutaiTray.exe",
                                  "Subutai Tray")

    subutai.AddStatus("Writing configuration")
    f = open(installDir+"/subutai_tray.conf", "w")
    f.write("P2P_Path="+installDir+"/bin/p2p.exe\n")
    f.write("VBoxManage_Path="+subutai.GetVBoxPath()+"\n")
    f.write("Ssh_Path="+installDir+"/bin/ssh.exe\n")
    f.write("Ssh_Keygen_Cmd="+installDir+"/bin/ssh-keygen.exe\n")
    f.close()

    subutai.Shutdown()

    return 0
