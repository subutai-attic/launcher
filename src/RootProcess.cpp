#include "RootProcess.h"

namespace SubutaiLauncher {
    
    RootProcess::RootProcess()
    {
        _logger = &Poco::Logger::get("subutai");
        _logger->debug("Starting root process");
        Poco::UUIDGenerator u;
        auto uuid = u.create();
        _filename = "/tmp/";
        _filename.append(uuid.toString());
    }

    RootProcess::~RootProcess()
    {
        Poco::File f(_filename);
        if (f.exists()) f.remove();
    }

    void RootProcess::addCommand(const std::string& cmd)
    {
        _logger->debug("Adding root command: %s", cmd);
        _commands.push_back(cmd);
    }

    void RootProcess::execute(const std::string& msg)
    {
        _logger->information("Running gksudo: %s", msg);
        Poco::FileOutputStream of(_filename);
        of << "#!/bin/bash";
        of << std::endl;
        for (auto it = _commands.begin(); it != _commands.end(); it++) 
        {
            of << (*it) << std::endl;
        }
        of.close();

        Poco::File f(_filename);
        f.setExecutable(true);
        Poco::Process p;
        Poco::Process::Args args;
        args.push_back("--message");
        args.push_back(msg);
        args.push_back(_filename);
        auto ph = p.launch("/usr/bin/gksudo", args);
        ph.wait();
    }
};
