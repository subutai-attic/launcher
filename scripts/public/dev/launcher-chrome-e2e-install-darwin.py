import subutai
import os


def subutaistart():
    subutai.AddStatus("Installing Browser Plugin")

    location = os.environ['HOME'] + '/Library/Application Support/Google/Chrome/External Extensions'
    if not os.path.exists(location):
        os.makedirs(location)

    ete = '{\n\t"external_update_url": "https://clients2.google.com/service/update2/crx"\n}'

    f = open(location+"/kpmiofpmlciacjblommkcinncmneeoaa.json", 'w')
    f.write(ete)  # python will convert \n to os.linesep
    f.close()

    subutai.Shutdown()

    return 0
