import subutai
import os
from time import sleep
from subprocess import call


def subutaistart():
    
    tmpDir = subutai.GetTmpDir()

    if subutai.IsChromeInstalled() != 0:
        subutai.AddStatus("Downloading Chrome")
        subutai.download("GoogleChromeStandaloneEnterprise64.msi")
        while subutai.isDownloadComplete() != 1:
            sleep(0.05)

        subutai.AddStatus("Installing Chrome")
        try:
            call(['msiexec', '/qn', '/i' tmpDir+"GoogleChromeStandaloneEnterprise64.msi"])
        except:
            subutai.RaiseError("Failed to install Google Chrome")
            sleep(5)

    subutai.AddStatus("Installing Browser Plugin")

    subutai.RegisterPlugin()

    subutai.Shutdown()

    return 0
