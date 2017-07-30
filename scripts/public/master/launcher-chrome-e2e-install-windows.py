import subutai
import os


def subutaistart():
    subutai.AddStatus("Installing Browser Plugin")

    subutai.RegisterPlugin()

    subutai.Shutdown()

    return 0
