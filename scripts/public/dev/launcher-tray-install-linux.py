import subutai
import tarfile
from time import sleep
from subprocess import call


def updateProgress(tray, total):
    cur = tray
    val = (int)(100 * cur) / total
    progress = (float)(val/100)
    subutai.SetProgress(progress)


def subutaistart():
    subutai.AddStatus("Download Tray application")

    trayFile = "SubutaiTray_libs.tar.gz"

    traySize = subutai.GetFileSize(trayFile)
    totalSize = traySize
    trayProgress = 0

    subutai.download(trayFile)
    while subutai.isDownloadComplete() != 1:
        sleep(0.05)
        trayProgress = subutai.GetBytesDownload()
        updateProgress(trayProgress, totalSize)

    trayProgress = traySize

    tmpDir = subutai.GetTmpDir()
    installDir = subutai.GetInstallDir()

    subutai.AddStatus("Unpacking Tray")

    call(['/usr/bin/killall', '-9', 'SubutaiTray'])

    tar = tarfile.open(tmpDir+"/"+trayFile, "r:gz")
    tar.extractall(installDir+"/bin/")
    tar.close()

    subutai.AddStatus("Creating Symlink")
    subutai.MakeLink(installDir+"/bin/SubutaiTray",
                     "/usr/local/bin/SubutaiTray")

    desktop = '''
[Desktop Entry]
Version=1.0
Name=Subutai Tray
Comment=Subutai Tray
Keywords=Internet;
Exec=SubutaiTray
Terminal=false
Type=Application
Icon=/usr/share/icons/launcher-logo.png
Categories=GNOME;Network;
StartupNotify=true
    '''.strip()

    f = open(tmpDir+"SubutaiTray.desktop", 'w')
    f.write(desktop)
    f.close()

    subutai.DesktopFileInstall(tmpDir+"SubutaiTray.desktop")
    subutai.StartTray()

    subutai.Shutdown()

    return 0
