import subutai

class Progress:
    coreSize = 0
    vboxSize = 0
    ubuntuSize = 0
    openjreSize = 0
    managementSize = 0
    coreProgress = 0
    vboxProgress = 0
    ubuntuProgress = 0
    openjreProgress = 0
    managementProgress = 0
    totalSize = 0
    lastProgress = 0.0

    def __init__(self, core, vbox, ubuntu, openjre, management):
        self.lastProgress = -1.0
        t = 0
        self.coreSize = subutai.GetFileSize(core)
        t = t + self.coreSize
        self.vboxSize = subutai.GetFileSize(vbox)
        t = t + self.vboxSize
        self.ubuntuSize = subutai.GetTemplateSize(ubuntu)
        t = t + self.ubuntuSize
        self.openjreSize = subutai.GetTemplateSize(openjre)
        t = t + self.openjreSize
        self.managementSize = subutai.GetTemplateSize(management)
        t = t + self.managementSize
        self.totalSize = t

    def getCoreSize(self):
        return self.coreSize

    def getVboxSize(self):
        return self.vboxSize

    def getUbuntuSize(self):
        return self.ubuntuSize

    def getOpenjreSize(self):
        return self.openjreSize

    def getManagementSize(self):
        return self.managementSize

    def setCoreProgress(self, s):
        self.coreProgress = s

    def setVboxProgress(self, s):
        self.vboxProgress = s

    def setUbuntuProgress(self, s):
        self.ubuntuProgress = s

    def setOpenjreProgress(self, s):
        self.openjreProgress = s

    def setManagementProgress(self, s):
        self.managementProgress = s

    def updateProgress(self):
        if self.totalSize == 0:
            return
        cur = self.coreProgress
        cur = cur + self.vboxProgress
        cur = cur + self.ubuntuProgress
        cur = cur + self.openjreProgress
        cur = cur + self.managementProgress
        val = (int)(100 * cur) / self.totalSize
        p = (float)(val/100)
        self.lastProgress = p
        subutai.SetProgress(p)

    def spin(self):
        subutai.SetProgress(-1.0)

    def unspin(self):
        subutai.SetProgress(self.lastProgress)
