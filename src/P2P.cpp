#include "P2P.h"

namespace SubutaiLauncher {

    const std::string SubutaiLauncher::P2P::BIN = "p2p";

    P2P::P2P() 
    {
        _version = "";
    }

    P2P::~P2P() 
    {

    }

    bool P2P::findInstallation() 
    {
        auto env = new Environment();
        String pathVar(env->getVar("PATH", ""));
        std::vector<std::string> path;
        pathVar.split(':', path);
        FileSystem fs;
        for (auto it = path.begin(); it != path.end(); it++) {
            fs.setPath((*it));
            if (fs.isFileExists(BIN)) {
                _installed = true;
                _path = (*it);
                _location = _path;
                _path.append(FileSystem::DELIM);
                _path.append(BIN);
                return true;
            }
        }
        return false;
    }

    std::string P2P::extractVersion()
    {
        if (_version != "") {
            return _version;
        }

        std::vector<std::string> args;
        args.push_back("version");

        Process p;
        p.launch(BIN, args, _location);
        if (p.wait() == 0) {
            _version = p.getOutputBuffer();
            return _version;
        } 
        return "";
    }

    void P2P::start()
    {

    }

    void P2P::stop()
    {

    }

    bool P2P::isRunning()
    {
        return false;
    }

};
