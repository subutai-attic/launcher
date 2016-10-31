import subutai
from time import sleep

def subutaistart():
    subutai.download("launcher-test-file")
    while subutai.isDownloadComplete() != 1:
        sleep(0.05)

    return 0

