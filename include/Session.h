#ifndef __SESSION_H__
#define __SESSION_H__

#include "Vars.h"

#include <iostream>

#include "Poco/Logger.h"

#include "VirtualBox.h"
#include "Downloader.h"
#include "Settings.h"
#include "Hub.h"
#include "NotificationCenter.h"

namespace SubutaiLauncher 
{

    class SSH;

    struct SSHSession
    {
        std::string name;
        SSH* session;
    };

    class Session 
    {
        public:
            static Session* instance();
            static bool isInstanced();
            void destroyInstance();
            Downloader* getDownloader();
            Settings* getSettings();
            NotificationCenter* getNotificationCenter();
            Hub* getHub();
            void setSSHCredentials(const std::string& user, const std::string& pass, const std::string& hostname, long port);
            std::string getSSHUser();
            std::string getSSHPass();
            std::string getSSHHostname();
            long getSSHPort();
            void addStatus(const std::string& text);
            void replaceStatus(const std::string& text);
            std::string getStatus();
            Poco::Logger& logger();
			bool isTerminating();
			void terminate();
			void start();
            // SSH Session
            SSH* makeSSHSession(const std::string& name, bool empty = false);
            SSH* getSSHSession(const std::string& name);
            void finalizeSSHSession(const std::string& name);
            void setStep(const std::string& step);
            void setAction(const std::string& action);
            std::string getStep();
            std::string getAction();
        private:
			bool _terminate;
            //SubutaiLauncher::VirtualBox* _virtualBox;
            Downloader* _downloader;
            Settings* _settings;
            NotificationCenter* _notificationCenter;
            Hub* _hub;
            std::string _sshUser;
            std::string _sshPass;
            std::string _sshHostname;
            long _sshPort;
            std::vector<std::string> _statusPool;
            std::vector<SSHSession> _sshSessions;
            std::string _step;
            std::string _action;
        protected:
            static Session *_instance;
            Session();
            ~Session();
    }; 

}

#endif
