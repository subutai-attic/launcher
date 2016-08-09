#ifndef __SESSION_H__
#define __SESSION_H__

#include <iostream>
#include "VirtualBox.h"
#include "Downloader.h"

namespace SubutaiLauncher {

    class Session {
        public:
            static Session* instance();
            void destroyInstance();
            Downloader* getDownloader();
        private:
            //SubutaiLauncher::VirtualBox* _virtualBox;
            Downloader* _downloader;
        protected:
            static Session *_instance;
            Session();
            ~Session();
    }; 

};

#endif
