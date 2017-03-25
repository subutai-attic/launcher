import subutai
import os

def subutaistart():
    #sudobin = ""
    #sudobin = "gksudo "
    sudobin = "pkexec --user root "

    ret = os.system(sudobin + "kill -9 $(ps aux | grep VirtualBox | awk '{ print $2 }')")
    ret = os.system(sudobin + "kill -9 $(ps aux | grep VB | awk '{ print $2 }')")

    os.system(sudobin + "apt-get -y remove --purge virtualbox-\*");
    ret = os.system(sudobin + "apt-get -y remove --purge snapcraft")
    ret = os.system(sudobin + "apt-get -y remove --purge sshpass")
    ret = os.system(sudobin + "apt -y autoremove")
    subutai.Shutdown()
