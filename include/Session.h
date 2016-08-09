#ifndef __SESSION_H__
#define __SESSION_H__

#include <iostream>
#include "VirtualBox.h"

namespace SubutaiLauncher {

    class Session {
        public:
            static Session* instance();
            void destroyInstance();
        private:
            VirtualBox* _virtualBox;
        protected:
            static Session *_instance;
            Session();
            ~Session();
    }; 

};

#endif
