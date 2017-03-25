import subutai
from time import sleep
from shutil import copyfile
import os
import stat

def subutaistart():
    sudobin = "gksudo "

    #ret = os.system("wget -q -O - https://dl-ssl.google.com/linux/linux_signing_key.pub | pkexec --user root apt-key add -")
    ret = os.system("wget -q -O - https://dl-ssl.google.com/linux/linux_signing_key.pub | gksudo apt-key add -")
    if ret != 0:
        subutai.RaiseError("Cannot install Chrome Browser, please install manually")

    ret = os.system(sudobin + " sh -c 'echo deb http://dl.google.com/linux/chrome/deb/ stable main >> /etc/apt/sources.list.d/google.list'")
    if ret != 0:
        raise Exception("Cannot install Chrome Browser, please install manually")
    ret = os.system(sudobin + "apt-get update")
    if ret != 0:
        raise Exception("Cannot install Chrome Browser, please install manually")
    ret = os.system(sudobin + "apt-get -y install google-chrome-stable")
    if ret != 0:
        raise Exception("Cannot install Chrome Browser, please install manually")

    subutai.Shutdown()
