import subutai
from time import sleep
from shutil import copyfile
import os
import stat
from subprocess import Popen, PIPE
import threading


def updateProgress(cocoasudo, p2p, tuntap, total):
    cur = cocoasudo + p2p + tuntap
    val = (int)(100 * cur) / total
    progress = (float)(val/100)
    subutai.SetProgress(progress)


def subutaistart():
    rc = subutai.HelloWorld()
    print(rc[0])
    print(rc[1])
    return 0
    tmpDir = subutai.GetTmpDir()
    installDir = subutai.GetInstallDir()

    cocoasudoFile = "cocoasudo"
    p2pFile = "p2p_osx"
    tuntapFile = "tuntap_20150118_osx.pkg"

    cocoasudoSize = subutai.GetFileSize(cocoasudoFile)
    p2pSize = subutai.GetFileSize(p2pFile)
    tuntapSize = subutai.GetFileSize(tuntapFile)
    totalSize = cocoasudoSize + p2pSize + tuntapSize
    cocoasudoProgress = 0
    p2pProgress = 0
    tuntapProgress = 0

    if not os.path.exists(installDir+"bin/cocoasudo"):
        subutai.AddStatus("Downloading cocoasudo application")
        subutai.download(cocoasudoFile)
        while subutai.isDownloadComplete() != 1:
            sleep(0.05)
            cocoasudoProgress = subutai.GetBytesDownload()
            updateProgress(cocoasudoProgress, p2pProgress, tuntapProgress,
                           totalSize)

        try:
            copyfile(tmpDir+cocoasudoFile, installDir+"bin/cocoasudo")
            st = os.stat(installDir+"bin/cocoasudo")
            os.chmod(installDir+"bin/cocoasudo", st.st_mode | stat.S_IEXEC)
        except:
            subutai.RaiseError("Failed to install cocoasudo. Aborting")
            sleep(10)
            return -99

    cocoasudoProgress = cocoasudoSize
    subutai.AddStatus("Download TUNTAP driver")
    subutai.download(tuntapFile)
    while subutai.isDownloadComplete() != 1:
        sleep(0.05)
        tuntapProgress = subutai.GetBytesDownload()
        updateProgress(cocoasudoProgress, p2pProgress, tuntapProgress,
                       totalSize)

    tuntapProgress = tuntapSize
    subutai.AddStatus("Download p2p binary")

    subutai.download(p2pFile)
    while subutai.isDownloadComplete() != 1:
        sleep(0.05)
        p2pProgress = subutai.GetBytesDownload()
        updateProgress(cocoasudoProgress, p2pProgress, tuntapProgress,
                       totalSize)

    p2pProgress = p2pSize
    subutai.AddStatus("Download finished. Installing")

    try:
        copyfile(tmpDir+p2pFile, installDir+"bin/p2p")
    except:
        subutai.RaiseError("Failed to move p2p binary to " +
                           installDir + "bin/p2p")
        return 21

    subutai.SetProgress(0.99)

    subutai.AddStatus("Making p2p binary executable")
    try:
        st = os.stat(installDir+"/bin/p2p")
        os.chmod(installDir+"/bin/p2p", st.st_mode | stat.S_IEXEC)
    except:
        subutai.RaiseError("Failed to make p2p binary executable")
        return 31

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

    subutai.AddStatus("Configure P2P Daemon")
    installScript = "#!/bin/bash\n\n"
    installScript = installScript + "cp " + tmpDir + daemonFile + " /Library/LaunchDaemons/" + daemonFile + "\n"
    installScript = installScript + "cp " + tmpDir + "p2p.conf /etc/newsyslog.d/p2p.conf\n"
    installScript = installScript + "launchctl load /Library/LaunchDaemons/" + daemonFile + "\n"
    installScript = installScript + "installer -pkg " + tmpDir + "tuntap_20150118.pkg -target /\n"
    installScript = installScript + "ln -s "+installDir+"/bin/p2p /usr/local/bin/p2p\n"

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
        script = 'do shell script "'+tmpDir+'p2p-setup.sh" with administrator privileges'
        p = Popen(['osascript', '-'], stdin=PIPE, stdout=PIPE, stderr=PIPE, universal_newlines=True)
        stdout, stderr = p.communicate(script)
    except:
        subutai.RaiseError("Failed to install p2p daemon")
        sleep(10)
        return 22

    subutai.SetProgress(1.00)

    sleep(5)
    subutai.Shutdown()

    return 0
