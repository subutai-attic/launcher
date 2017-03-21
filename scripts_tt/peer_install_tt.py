import subutai
import time
from shutil import copyfile
import os
import stat
import sys


def subutaistart():

    subutai.download("core.ova")
    while subutai.isDownloadComplete() != 1:
        time.sleep(0.05)

    sName="subutai"
    ret = subutai.cloneVM("subutai")
    if ret != 1:
         subitai.RaiseError("snappy core not renamed ")
    else:
         subutai.RaiseInfo("snappy core renamed")

    subutai.runScripts("DEV", "1")
    if ret != 1:
         subitai.RaiseError("scripts not completed ")
    else:
         subutai.RaiseInfo("scripts completed")

    subutai.Shutdown()
