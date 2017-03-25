import subutai
import time
from shutil import copyfile
import os
import stat
import sys


def subutaistart():

    sName="subutai"
    #remove vm
    subutai.Shutdown()
