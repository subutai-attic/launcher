import subutai
import os
from subprocess import call
from time import sleep


def updateProgress(chrome, total):
    cur = chrome
    val = (int)(100 * cur) / total
    progress = (float)(val/100)
    subutai.SetProgress(progress)


def subutaistart():
    tmpDir = subutai.GetTmpDir()
    subutai.AddStatus("Downloading Google Chrome")

    chromeFilename = "google-chrome-stable_current_amd64.deb"

    chromeSize = subutai.GetFileSize(chromeFilename)
    totalSize = chromeSize
    chromeProgress = 0

    subutai.download(chromeFilename)
    while subutai.isDownloadComplete() != 1:
        sleep(0.05)
        chromeProgress = subutai.GetBytesDownload()
        updateProgress(chromeProgress, totalSize)

    chromeProgress = chromeSize

    try:
        call(['/usr/bin/gksudo',
              '--message',
              'Install Google Chrome Dependencies',
              'apt-get install libappindicator1 -y'])

        call(['/usr/bin/gksudo',
              '--message',
              'Install Google Chrome',
              'dpkg -i '+tmpDir+chromeFilename])
        sleep(20)
    except:
        subutai.RaiseError("Failed to install Google Chrome")
        sleep(5)

    subutai.AddStatus("Installing Browser Plugin")

    #location = os.environ['HOME'] + '/.config/google-chrome/Default/External Extensions'
    location = '/opt/google/chrome/extensions/'
    if not os.path.exists(location):
        try:
            call(['/usr/bin/gksudo',
                  '--message',
                  'Create extension directory',
                  'mkdir -p '+location])
        except:
            subutai.RaiseError("Failed to create "+location+" directory")
            sleep(5)
            return -22

    ete = '{\n\t"external_update_url": "https://clients2.google.com/service/update2/crx"\n}\n'

    f = open(tmpDir+"kpmiofpmlciacjblommkcinncmneeoaa.json", 'w')
    f.write(ete)  # python will convert \n to os.linesep
    f.close()

    try:
        call(['/usr/bin/gksudo',
              '--message',
              'Create extension directory',
              'cp '+tmpDir+'kpmiofpmlciacjblommkcinncmneeoaa.json '+location])
    except:
        subutai.RaiseError("Failed to move extension file")
        sleep(5)
        return -22

    subutai.Shutdown()

    return 0
