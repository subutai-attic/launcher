#ifndef __SESSION_H__
#define __SESSION_H__

#include "Vars.h"

#include <iostream>

#include "Poco/Logger.h"

#include "VirtualBox.h"
#include "Downloader.h"
#include "Settings.h"
#include "NotificationCenter.h"
#include "Hub.h"

namespace SubutaiLauncher 
{

    class Session 
    {
        public:
            static Session* instance();
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
            std::string getStatus();
            Poco::Logger& logger();
			bool isTerminating();
			void terminate();
			void start();
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
        protected:
            static Session *_instance;
            Session();
            ~Session();
    }; 

}

#endif
