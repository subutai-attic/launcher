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
    <string>io.subutai.p2p.daemon</string>

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

    <key>Debug</key>
    <true/>
</dict>
</plist>
'''.strip()

    daemonFile = 'io.subutai.p2p.daemon.plist'

    f = open(tmpDir+daemonFile, 'w')
    f.write(service)
    f.close()

    syslog = '''
# logfilename          [owner:group]    mode count size when  flags [/pid_file] [sig_num]
/var/log/p2p.log                       644  7     *    $D0   J
    '''.strip()
    sf = open(tmpDir+'p2p.conf', 'w')
    sf.write(syslog)
    sf.close()

    subutai.AddStatus("Configur P2P Daemon")
    installScript = "#!/bin/bash\n\n"
    installScript = installScript + "cp " + tmpDir + daemonFile + " /Library/LaunchDaemons/" + daemonFile + "\n"
    installScript = installScript + "cp " + tmpDir + "p2p.conf /etc/newsyslog.d/p2p.conf\n"
    installScript = installScript + "launchctl load /Library/LaunchDaemons/" + daemonFile + "\n"

    f = open(tmpDir+"p2p-setup.sh", 'w')
    f.write(installScript)
    f.close()

    try:
        st = os.stat(tmpDir+"p2p-setup.sh")
        os.chmod(tmpDir+"p2p-setup.sh", st.st_mode | stat.S_IEXEC)
    except:
        subutai.RaiseError("Failed to configure p2p daemon")
        sleep(10)
        return 31

    try:
        call([installDir+"bin/cocoasudo",
              '--prompt="Setup P2P Daemon"',
              'sudo',
              tmpDir+"p2p-setup.sh",
              ])
    except:
        subutai.RaiseError("Failed to install p2p daemon")
        sleep(10)
        return 22

    sleep(5)
    subutai.Shutdown()

    return 0
