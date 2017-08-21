import subutai
from time import sleep
from shutil import copyfile
import os
import stat


def subutaistart():
    subutai.AddStatus("Download p2p binary")

    sfile = "/etc/systemd/system/p2p.service"
    if os.path.exists(sfile):
        subutai.RemoveSystemdUnit("p2p.service")

    subutai.download("p2p")
    while subutai.isDownloadComplete() != 1:
        sleep(0.05)

    tmpDir = subutai.GetTmpDir()
    installDir = subutai.GetInstallDir()

    subutai.AddStatus("Download finished. Installing")
    copyfile(tmpDir+"/p2p", installDir+"/bin/p2p")

    st = os.stat(installDir+"/bin/p2p")
    os.chmod(installDir+"/bin/p2p", st.st_mode | stat.S_IEXEC)

    subutai.AddStatus("Creating symlink")
    subutai.MakeLink(installDir+"/bin/p2p", "/usr/bin/p2p")

    subutai.AddStatus("Creating systemd unit")

    unit = '''
[Unit]
Description=Service for Subutai p2p daemon
Wants=network-online.target
After=network-online.target

[Service]
ExecStart=/opt/subutai/bin/p2p daemon
Restart=on-failure
WorkingDirectory=/opt/subutai/

TimeoutStopSec=30
Type=simple

[Install]
WantedBy=multi-user.target
    '''.strip()

    f = open(tmpDir+"p2p.service", 'w')
    f.write(unit)
    f.close()

    subutai.AddSystemdUnit("p2p.service", tmpDir+"p2p.service")

    subutai.Shutdown()

    return 0
