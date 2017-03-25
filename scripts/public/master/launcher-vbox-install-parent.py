import subutai
import os
import subprocess
from time import sleep

def subutaistart():
    #sudobin = "gksudo "
    sudobin = 'gksudo'
    sudoarg = ''

    #sudobin = 'pkexec'
    #sudoarg = ' --user root '
    binex = 'vbox_install_1_tt.py'

    subutai.download(binex)
    while subutai.isDownloadComplete() != 1:
        sleep(0.05)

    ret = subprocess.call([sudobin, binex ])
    if ret != 0:
          subutai.RaiseError("Subprocess not completed")
    subutai.Shutdown()
