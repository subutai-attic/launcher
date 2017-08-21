import subutai
import tarfile
from time import sleep
from subprocess import call


def subutaistart():
    subutai.AddStatus("Download Tray application")

    tray = "SubutaiTray_libs.tar.gz"

    subutai.download(tray)
    while subutai.isDownloadComplete() != 1:
        sleep(0.05)

    tmpDir = subutai.GetTmpDir()
    installDir = subutai.GetInstallDir()

    subutai.AddStatus("Unpacking Tray")

    call(['/usr/bin/killall', '-9', 'SubutaiTray'])

    tar = tarfile.open(tmpDir+"/"+tray, "r:gz")
    tar.extractall(installDir+"/bin/")
    tar.close()

    subutai.AddStatus("Creating Symlink")
    subutai.MakeLink(installDir+"/bin/SubutaiTray", "/usr/local/bin/SubutaiTray")

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

    subutai.Shutdown()

    return 0
