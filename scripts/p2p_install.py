import subutai

def subutaistart()
    subutai.download("p2p")
    while !subutai.isDownloadComplete() == 1:
        sleep(0.05)
