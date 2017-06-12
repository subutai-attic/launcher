import subutai
from time import sleep

def subutaistart():
    print("SetProgressTest started")
    subutai.SetProgress(10.0)
    sleep(1)
    subutai.SetProgress(50.0)
    sleep(1)
    subutai.SetProgress(70.0)
    sleep(1)
    subutai.SetProgress(100.0)
    sleep(1)
    print("SetProgressTest finished")

    subutai.Shutdown()

    return 0

