import subutai
from time import sleep

def subutaistart():
    subutai.download("launcher-ad-1.png")
    while subutai.isDownloadComplete() != 1:
        sleep(0.05)

    subutai.download("launcher-ad-2.png")
    while subutai.isDownloadComplete() != 1:
        sleep(0.05)

    subutai.download("launcher-ad-3.png")
    while subutai.isDownloadComplete() != 1:
        sleep(0.05)

    subutai.download("launcher-ad-4.png")
    while subutai.isDownloadComplete() != 1:
        sleep(0.05)
