import subutai
from time import sleep
from shutil import copyfile
import os
import stat

def subutaistart():
    subutai.CheckDirectories()
    subutai.download("SubutaiTray_dev")
    while subutai.isDownloadComplete() != 1:
        sleep(0.05)

    subutai.download("subutai-tray.desktop")
    while subutai.isDownloadComplete() != 1:
        sleep(0.05)

    subutai.download("libicudata.so.52")
    while subutai.isDownloadComplete() != 1:
        sleep(0.05)

    subutai.download("libicui18n.so.52")
    while subutai.isDownloadComplete() != 1:
        sleep(0.05)

    subutai.download("libicuuc.so.52")
    while subutai.isDownloadComplete() != 1:
        sleep(0.05)

    subutai.download("libssh2_app")
    while subutai.isDownloadComplete() != 1:
        sleep(0.05)

    tmpDir = subutai.GetTmpDir()
    targetDir = subutai.GetInstallDir()

    copyfile(tmpDir+"/SubutaiTray_dev", targetDir+"/bin/subutai-tray")
    if os.path.isfile("/usr/bin/subutai-tray"):
        os.remove("/usr/bin/subutai-tray")

    st = os.stat(targetDir+"/bin/subutai-tray")
    os.chmod(targetDir+"/bin/subutai-tray", st.st_mode | stat.S_IEXEC | stat.S_IXOTH | stat.S_IXGRP)
    os.symlink(targetDir+"/bin/subutai-tray", "/usr/bin/subutai-tray")

    copyfile(tmpDir+"/subutai-tray.desktop", "/usr/share/applications/subutai-tray.desktop")

    copyfile(tmpDir+"/libicudata.so.52", targetDir+"/lib/libicudata.so.52")
    copyfile(tmpDir+"/libicui18n.so.52", targetDir+"/lib/libicui18n.so.52")
    copyfile(tmpDir+"/libicuuc.so.52", targetDir+"/lib/libicuuc.so.52")
    copyfile(tmpDir+"/libssh2_app", targetDir+"/bin/libssh2_app")

    if os.path.isfile("/usr/bin/libssh2_app"):
        os.remove("/usr/bin/libssh2_app")
    os.symlink(targetDir+"/bin/libssh2_app", "/usr/bin/libssh2_app")
    os.chmod(targetDir+"/bin/libssh2_app", st.st_mode | stat.S_IEXEC | stat.S_IXOTH | stat.S_IXGRP)

    os.system("desktop-file-install /usr/share/applications/subutai-tray.desktop")
    os.system("updatedb")

    subutai.Shutdown()
