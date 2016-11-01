#ifndef __SESSION_H__
#define __SESSION_H__

#include <iostream>
#include "VirtualBox.h"
#include "Downloader.h"
#include "Settings.h"
#include "ConfigurationManager.h"

namespace SubutaiLauncher 
{

    class Session 
    {
        public:
            static Session* instance();
            void destroyInstance();
            Downloader* getDownloader();
            Settings* getSettings();
            ConfigurationManager* getConfManager();
        private:
            //SubutaiLauncher::VirtualBox* _virtualBox;
            Downloader* _downloader;
            Settings* _settings;
            ConfigurationManager* _confManager;
        protected:
            static Session *_instance;
            Session();
            ~Session();
    }; 

}

#endif
