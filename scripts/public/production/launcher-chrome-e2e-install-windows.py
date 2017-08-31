import subutai
from time import sleep
from subprocess import call


def updateProgress(chrome, total):
    cur = chrome
    val = (int)(100 * cur) / total
    progress = (float)(val/100)
    subutai.SetProgress(progress)


def subutaistart():

    tmpDir = subutai.GetTmpDir()

    chromeFilename = "GoogleChromeStandaloneEnterprise64.msi"

    chromeSize = subutai.GetFileSize(chromeFilename)
    totalSize = chromeSize
    chromeProgress = 0

    if subutai.IsChromeInstalled() != 0:
        subutai.AddStatus("Downloading Chrome")
        subutai.download(chromeFilename)
        while subutai.isDownloadComplete() != 1:
            sleep(0.05)
            chromeProgress = subutai.GetBytesDownload()
            updateProgress(chromeProgress, totalSize)

        chromeProgress = chromeSize

        subutai.AddStatus("Installing Chrome")
        try:
            call(['msiexec', '/qn', '/i', tmpDir+chromeFilename])
        except:
            subutai.RaiseError("Failed to install Google Chrome")
            sleep(5)

    subutai.AddStatus("Installing Browser Plugin")

    subutai.RegisterPlugin()

    subutai.Shutdown()

    return 0
