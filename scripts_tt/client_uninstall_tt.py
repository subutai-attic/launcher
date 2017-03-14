import subutai
from shutil import rmtree
import os
import stat

def subutaistart():
    traySource = "SubutaiTray_linux.tar.gz"
    trayBin = "SubutaiTray"
    trayDir = "tray"
    sudobin = "pkexec --user root "
    #sudobin = "gksudo "

    tmpDir = subutai.GetTmpDir()
    installDir = subutai.GetInstallDir()

    if os.path.isdir(installDir+"/bin"):
        os.system(sudobin + "rm -rdf " + installDir + "/bin/")
        #rmtree(installDir+"/bin/"+trayDir)
    """
    if os.path.exists("/usr/bin/"+trayBin):
         if os.path.islink("/usr/bin/"+trayBin):
             os.unlink("/usr/bin/"+trayBin)
         else:
             os.remove("/usr/bin/"+trayBin)
    """
    if os.path.islink("/usr/bin/"+trayBin):
        os.system(sudobin + "rm /usr/bin/" + trayBin)
        #os.unlink("/usr/bin/"+trayBin)

    ret = os.system("which service")
    if ret == 0:
        os.system(sudobin + "service p2p stop")
        ret = os.system(sudobin + "update-rc.d -f p2p remove")

    ret = os.system("which systemctl")
    if ret == 0:
        os.system(sudobin + "systemctl stop p2p.service")

    if os.path.islink("/usr/bin/p2p"):
        os.system(sudobin + "rm /usr/bin/p2p")
        #os.unlink("/usr/bin/p2p")

    if os.path.exists(installDir):
        os.system(sudobin + "rm -rdf " + installDir)
        #rmtree(installDir)

    if os.path.isfile("/etc/init.d/p2p"):
        os.system(sudobin + "rm /etc/init.d/p2p")
        #os.remove("/etc/init.d/p2p")

    subutai.Shutdown()
