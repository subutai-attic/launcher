import subutai
import os
from subprocess import call
from time import sleep


def subutaistart():
    tmpDir = subutai.GetTmpDir()
    subutai.AddStatus("Downloading Google Chrome")

    subutai.download("google-chrome-stable_current_amd64.deb")
    while subutai.isDownloadComplete() != 1:
        sleep(0.05)

    try:
        call(['/usr/bin/gksudo',
              '--message',
              'Install Google Chrome',
              'dpkg -i '+tmpDir+'google-chrome-stable_current_amd64.deb'])
        sleep(20)
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
