import subutai
import os
import tarfile

p2p = "p2p"
tray = "SubutaiTray"
trayLibs = "SubutaiTray_libs.tar.gz"

tmpDir = subutai.GetTmpDir()
installDir = subutai.GetInstallDir()

if not os.path.isdir(installDir+"/bin"):
    os.makedirs(installDir+"/bin")

if os.path.exists(tmpDir+"/"+p2p):
    os.rename(tmpDir+"/"+p2p, installDir+"/bin/"+p2p)
    os.symlink(installDir+"/bin"+p2p, "/usr/bin/"+p2p)

if os.path.exists(tmpDir+"/"+tray):
    os.rename(tmpDir+"/"+tray, installDir+"/bin/"+tray)
    os.symlink(installDir+"/bin"+tray, "/usr/bin"+tray)

if os.path.exists(tmpDir+"/"+trayLibs):
    tar = tarfile.open(tmpDir+"/"+trayLibs, "r:gz")
    tar.extractall(installDir+"/bin")
    tar.close()
    os.remove(tmpDir+"/"+trayLibs)


