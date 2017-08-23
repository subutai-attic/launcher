import subutai
import os
import stat
from time import sleep
from shutil import copyfile
from subprocess import call


def updateProgress(cocoasudo, chrome, total):
    cur = cocoasudo + chrome
    val = (int)(100 * cur) / total
    progress = (float)(val/100)
    subutai.SetProgress(progress)


def subutaistart():
    tmpDir = subutai.GetTmpDir()
    installDir = subutai.GetInstallDir()

    cocoasudoSize = subutai.GetFileSize("cocoasudo")
    chromeSize = subutai.GetFileSize("GoogleChrome_osx.tar.gz")
    totalSize = cocoasudoSize + chromeSize
    cocoasudoProgress = 0
    chromeProgress = 0

    if not os.path.exists(installDir+"bin/cocoasudo"):
        subutai.AddStatus("Downloading cocoasudo application")
        subutai.download("cocoasudo")
        while subutai.isDownloadComplete() != 1:
            sleep(0.05)
            cocoasudoProgress = subutai.GetBytesDownload()
            updateProgress(cocoasudoProgress, chromeProgress, totalSize)

        try:
            copyfile(tmpDir+"cocoasudo", installDir+"bin/cocoasudo")
            st = os.stat(installDir+"bin/cocoasudo")
            os.chmod(installDir+"bin/cocoasudo", st.st_mode | stat.S_IEXEC)
        except:
            subutai.RaiseError("Failed to install cocoasudo. Aborting")
            sleep(10)
            return -99

    cocoasudoProgress = cocoasudoSize

    subutai.AddStatus("Checking Google Chrome Installation")
    if not os.path.exists("/Applications/Google Chrome.app"):
        subutai.AddStatus("Downloading Google Chrome")
        subutai.download("GoogleChrome_osx.tar.gz")
        while subutai.isDownloadComplete() != 1:
            sleep(0.05)
            chromeProgress = subutai.GetBytesDownload()
            updateProgress(cocoasudoProgress, chromeProgress, totalSize)

        chromeProgress = chromeSize

        sleep(5)
        subutai.AddStatus("Installing Google Chrome")
        try:
            call([installDir+"bin/cocoasudo",
                  '--prompt="Install Google Chrome"',
                  '/usr/bin/tar',
                  '-xf',
                  tmpDir+'GoogleChrome_osx.tar.gz',
                  '-C',
                  '/Applications'])
        except:
            subutai.RaiseError("Failed to install Google Chrome")

    subutai.AddStatus("Installing Browser Plugin")
    sleep(3)

    location = os.environ['HOME'] + '/Library/Application Support/Google/Chrome/External Extensions'
    if not os.path.exists(location):
        os.makedirs(location)

    ete = '{\n\t"external_update_url": "https://clients2.google.com/service/update2/crx"\n}'

    f = open(location+"/kpmiofpmlciacjblommkcinncmneeoaa.json", 'w')
    f.write(ete)  # python will convert \n to os.linesep
    f.close()

    subutai.Shutdown()

    return 0
