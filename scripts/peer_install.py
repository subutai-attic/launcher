import subutai
from time import sleep
from shutil import copyfile
import os
import stat

def subutaistart():
    subutai.CheckDirectories()
    tmpDir = subutai.GetTmpDir()
    targetDir = subutai.GetInstallDir()

    subutai.download("snappy.ova")
    while subutai.isDownloadComplete() != 1:
        sleep(0.05)

    subutai.download("subutai_"+subutai.GetMasterVersion()+"_amd64.snap")
    while subutai.isDownloadComplete() != 1:
        sleep(0.05)

    #os.system("tar zxf "+tmpDir+"/snappy.ova -C "+targetDir+"/resources/snappy.ovf")

    #subutai.VBox("import "+targetDir+"/resources/snappy.ovf")
    subutai.VBox("import "+tmpDir+"/snappy.ova")
    subutai.VBox("modifyvm snappy --cpus 2")
    subutai.VBox("modifyvm snappy --memory 200000")
    subutai.VBox("modifyvm snappy --nic1 nat")
    subutai.VBox("modifyvm snappy --cableconnected1 on")
    subutai.VBox('modifyvm snappy --natpf1 "ssh-fwd,tcp,,4567,,22" --natpf1 "https-fwd,tcp,,9999,,8443"')
    subutai.VBox("modifyvm snappy --rtcuseutc on")
    subutai.VBox("modifyvm snappy --name subutai-1")
    subutai.VBox("startvm --type headless subutai-1")

    #subutai.VBox("controlvm subuati-1 poweroff soft")

    subutai.Shutdown()
