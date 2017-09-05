import subutai
from time import sleep

def subutaistart():
    subutai.SetProgress(10.0)
    sleep(0.1)
    subutai.SetProgress(50.0)
    sleep(0.1)
    subutai.SetProgress(70.0)
    sleep(0.1)
    subutai.SetProgress(100.0)
    sleep(0.1)
    subutai.Shutdown()

    return 0

