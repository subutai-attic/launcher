import subutai
import os
import subprocess
from time import sleep

def subutaistart():
    #sudobin = "gksudo "
    sudobin = 'pkexec'
    sudoarg = ' --user root '
    binex = 'vbox_uninstall_1_tt.py'

    subutai.download(binex)
    while subutai.isDownloadComplete() != 1:
        sleep(0.05)

    subprocess.call([sudobin,sudoarg + binex])

    subutai.Shutdown()
