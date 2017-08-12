import subutai
from time import sleep
from shutil import copyfile
import os
import stat


def subutaistart():

    subutai.AddStatus("Download p2p binary")

    subutai.download("p2p_osx")
    while subutai.isDownloadComplete() != 1:
        sleep(0.05)

    tmpDir = subutai.GetTmpDir()
    installDir = subutai.GetInstallDir()

    subutai.AddStatus("Download finished. Installing")
    try:
        copyfile(tmpDir+"p2p_osx", installDir+"bin/p2p")
    except:
        subutai.RaiseError("Failed to move p2p binary to " + installDir + "bin/p2p")
        return 21

    sleep(5)

    subutai.AddStatus("Making p2p binary executable")
    try:
        st = os.stat(installDir+"/bin/p2p")
        os.chmod(installDir+"/bin/p2p", st.st_mode | stat.S_IEXEC)
    except:
        subutai.RaiseError("Failed to make p2p binary executable")
        return 31

    subutai.AddStatus("Creating symlink")
    subutai.MakeLink(installDir+"/bin/p2p", "/usr/local/bin/p2p")

    subutai.AddStatus("Creating p2p service")
    service = '''
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple Computer//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
<dict>
    <key>Label</key>
    <string>io.subutai.p2p.daemon</string> <!-- org.mongodb.mongodb perhaps? -->

    <key>ProgramArguments</key>
    <array>
            <string>/usr/local/bin/p2p</string>
            <string>daemon</string>
    </array>

    <key>KeepAlive</key>
    <true/>

    <key>StandardOutPath</key>
    <string>/var/log/p2p.log</string>

    <key>StandardErrorPath</key>
    <string>/var/log/p2p.log</string>
</dict>
</plist>
'''.strip()

    try:
        f = open(tmpDir+'io.subutai.p2p.daemon.plist', 'w')
        f.write(service)
        f.close()
    except:
        subutai.RaiseError("Failed to create service file for p2p")
        return 25

    sleep(5)
    subutai.Shutdown()

    return 0
