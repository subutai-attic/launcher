import subutai
from time import sleep
import os

def subutaistart():
    os.system("gksudo 'apt-get -y  install google-chrome-stable'")
    """
    while subutai.isDownloadComplete() != 1:
        sleep(0.05)
    """
    subutai.Shutdown()