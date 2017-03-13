import subutai
from time import sleep
from shutil import copyfile
from shutil import rmtree
import os
import stat
import tarfile

def subutaistart():
    subutai.CheckDirectories()

    traySource = "SubutaiTray_linux.tar.gz"
    trayBin = "SubutaiTray"
    trayDir = "tray"
    sudobin = "pkexec --user root "
    #sudobin = "gksudo "

    subutai.download(traySource)
    while subutai.isDownloadComplete() != 1:
        sleep(0.05)

    subutai.download("p2p")
    while subutai.isDownloadComplete() != 1:
        sleep(0.05)

    subutai.download("p2p-ubuntu-service")
    while subutai.isDownloadComplete() != 1:
        sleep(0.05)


    tmpDir = subutai.GetTmpDir()
    installDir = subutai.GetInstallDir()

    if  os.path.isdir(installDir):
        os.system(sudobin + "rm -rdf " + installDir)
        #os.makedirs(installDir+"/bin")

    if os.path.exists(installDir + "/bin/" + trayDir):
        os.system(sudobin + "rm -rdf " + installDir + "/bin/" + trayDir)
        #rmtree(installDir+"/bin/"+trayDir)

    if os.path.islink("/usr/bin/"+trayBin):
        os.system(sudobin + "rm /usr/bin/"+trayBin)
        #os.unlink("/usr/bin/"+trayBin)

    if not os.path.isdir(installDir):
        os.system(sudobin + "mkdir " + installDir)
        #os.makedirs(installDir+"/bin")

    if not os.path.isdir(installDir + "/bin"):
        os.system(sudobin + "mkdir " + installDir + "/bin")
        #os.makedirs(installDir+"/bin")

    """
    if os.path.exists("/usr/bin/"+trayBin):
         if os.path.isfile("/usr/bin/"+trayBin):
              os.remove("/usr/bin/"+trayBin)
         else:
              os.unlink("/usr/bin/"+trayBin)
    """

    if os.path.exists(tmpDir+"/"+traySource):
        ret = os.system(sudobin + "mkdir -p " + installDir);
        if ret !=0:
               subutai.RaiseError("mkdir instDir error")
        if os.path.exists(installDir):
               ret = os.system(sudobin + "tar xz -f" + tmpDir + "/" + traySource + " -C " + installDir);
               sleep(10)
        #os.system(sudobin + "rm " + tmpDir+"/"+traySource);
    if not os.path.islink(installDir+"/bin/"+trayBin):
        os.system(sudobin + "ln -s " + installDir+"/bin/" + trayBin + " /usr/bin/"+trayBin)
        #os.symlink(installDir+"/bin/"+trayDir+"/bin/"+trayBin, "/usr/bin/"+trayBin)

        """
        tar = tarfile.open(tmpDir+"/"+traySource, "r:gz")
        tar.extractall(installDir+"/bin/"+trayDir)
        tar.close()
        os.remove(tmpDir+"/"+traySource)
        if not os.path.exists(installDir+"/bin/"+trayDir+"/bin/"+trayBin):
            os.symlink(installDir+"/bin/"+trayDir+"/bin/"+trayBin, "/usr/bin/"+trayBin)
        """
    os.system(sudobin + "cp " + tmpDir+"/p2p " +  installDir + "/bin/p2p")
    #copyfile(tmpDir+"/p2p", installDir+"/bin/p2p")

    if os.path.islink("/usr/bin/p2p"):
        os.system(sudobin + "rm /usr/bin/p2p")
        #os.unlink("/usr/bin/p2p")
    st = os.stat(installDir+"/bin/p2p")
    os.system(sudobin + "chmod 0755 " + installDir + "/bin/p2p")
    os.system(sudobin + "ln -s " + installDir + "/bin/p2p /usr/bin/p2p")

    #os.chmod(installDir+"/bin/p2p", st.st_mode | stat.S_IEXEC | stat.S_IXOTH | stat.S_IXGRP)
    #os.symlink(installDir+"/bin/p2p", "/usr/bin/p2p")

    os.system(sudobin + "cp " + tmpDir + "/p2p-ubuntu-service /etc/init.d/p2p")
    os.system(sudobin + "chmod 0755 /etc/init.d/p2p")

    #copyfile(tmpDir+"/p2p-ubuntu-service", "/etc/init.d/p2p")
    #os.chmod("/etc/init.d/p2p", st.st_mode | stat.S_IEXEC | stat.S_IXOTH | stat.S_IXGRP)

    ret = os.system("which service")
    if ret == 0:
        os.system(sudobin + "service p2p restart")
        ret = os.system(sudobin + "update-rc.d p2p defaults")
        if ret != 0:
            os.system(sudobin + "uptate-rc.d p2p enable")

    ret = os.system("which systemctl")
    if ret == 0:
        os.system(sudobin + "systemctl daemon-reload")
        os.system(sudobin + "systemctl enable p2p.service")
        os.system(sudobin + "systemctl restart p2p.service")

    subutai.Shutdown()
