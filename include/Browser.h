#ifndef __SL_BROWSER_H__
#define __SL_BROWSER_H__

#include "Vars.h"

/*
#include "Vars.h"

#include "Environment.h"
#include "SubutaiString.h"
#include "FileSystem.h"
#include "SubutaiProcess.h"
#include "Session.h"*/
#include <string>
#include <vector>

namespace SubutaiLauncher {
    class Browser {
        public:
            static const std::string BIN;
            Browser();
            ~Browser();
	    bool findInstallation();
            bool isInstalled();
            bool isRunning();
            bool isUpdateRequired();
            std::string extractVersion();
            std::string execute(const std::string& command);
	    std::string sysExecute(const std::string& command, const std::string& cargs);
	    std::string sysExecute(const std::string& command, const std::vector<std::string>& cargs);

        protected:
        private:
            std::string _version;
            std::string _path;
            std::string _location;
            bool _installed;
            bool _running;
            bool _updateRequired;
    };

}

#endif
