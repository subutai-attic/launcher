import subutai
from time import sleep
from shutil import copyfile
import os
import stat

def subutaistart():
    sudobin = "gksudo "
    #sudobin = "pkexec --user root "

    ret = os.system(sudobin + "kill -9 $(ps aux | grep chrome | awk '{ print $2 }')")

    ret = os.system(sudobin _ "apt-get -y remove --purge  google-chrome-stable")
    if ret != 0:
        raise Exception("Cannot remove Chrome Browser, please uinstall manually")

    subutai.Shutdown()
