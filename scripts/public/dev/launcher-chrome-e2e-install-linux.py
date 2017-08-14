import subutai
import os
from subprocess import call
from time import sleep


def subutaistart():
    subutai.AddStatus("Installing Google Chrome")
    try:
        call(['/usr/bin/gksudo', 'apt-get', 'install', 'google-chrome-stable'])
    except:
        subutai.RaiseError("Failed to install Google Chrome")
        sleep(5)

    subutai.AddStatus("Installing Browser Plugin")

    location = os.environ['HOME'] + '/.config/google-chrome/Default/External Extensions'
    if not os.path.exists(location):
        os.makedirs(location)

    ete = '{\n\t"external_update_url": "https://clients2.google.com/service/update2/crx"\n}'

    f = open(location+"/kpmiofpmlciacjblommkcinncmneeoaa.json", 'w')
    f.write(ete)  # python will convert \n to os.linesep
    f.close()

    subutai.Shutdown()

    return 0
