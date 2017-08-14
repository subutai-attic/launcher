import subutai
from time import sleep
from shutil import copyfile
import os
import stat
from subprocess import call


def subutaistart():
    tmpDir = subutai.GetTmpDir()
    installDir = subutai.GetInstallDir()

    if not os.path.exists(installDir+"bin/cocoasudo"):
        subutai.AddStatus("Downloading cocoasudo application")
        subutai.download("cocoasudo")
        while subutai.isDownloadComplete() != 1:
            sleep(0.05)

        try:
            copyfile(tmpDir+"cocoasudo", installDir+"bin/cocoasudo")
            st = os.stat(installDir+"bin/cocoasudo")
            os.chmod(installDir+"bin/cocoasudo", st.st_mode | stat.S_IEXEC)
        except:
            subutai.RaiseError("Failed to install cocoasudo. Aborting")
            sleep(10)
            return -99

    subutai.AddStatus("Download TUNTAP driver")
    subutai.download("tuntap_20150118_osx.pkg")
    while subutai.isDownloadComplete() != 1:
        sleep(0.05)

    try:
        call([installDir+"bin/cocoasudo",
              '--prompt="Install TUNTAP driver"',
              'installer',
              '-pkg',
              tmpDir+'tuntap_20150118.pkg',
              '-target',
              '/'])
    except:
        subutai.RaiseError("Failed to install TUNTAP driver. Aborting")
        sleep(10)
        return -98

    subutai.AddStatus("Download p2p binary")

    subutai.download("p2p_osx")
    while subutai.isDownloadComplete() != 1:
        sleep(0.05)

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
        call([installDir+"bin/cocoasudo",
              '--prompt="Install P2P Service"',
              'cp',
              tmpDir+'io.subutai.p2p.daemon.plist',
              '/Library/LaunchDaemons/'])
    except:
        subutai.RaiseError("Failed to create service file for p2p")
        return 25

    subutai.AddStatus("Configuring syslog")
    syslog = '''
# logfilename          [owner:group]    mode count size when  flags [/pid_file] [sig_num]
/var/log/p2p.log                       644  7     *    $D0   J
    '''.strip()

    try:
        sf = open(tmpDir+'p2p.conf', 'w')
        sf.write(syslog)
        sf.close()
        call([installDir+"bin/cocoasudo",
              '--prompt="Setup P2P Logger"',
              'cp',
              tmpDir+'p2p.conf',
              '/etc/newsyslog.d/p2p.conf'])
    except:
        subutai.AddStatus("Failed to configur P2P logger")

    sleep(5)

    try:
        call([installDir+"bin/cocoasudo",
              '--prompt="Start P2P Daemon"',
              'launchctl',
              'load',
              '/Library/LaunchDaemons/io.subutai.p2p.daemon.plist'])
    except:
        subutai.AddStatus("Failed to load P2P Service")

    sleep(5)
    subutai.Shutdown()

    return 0
