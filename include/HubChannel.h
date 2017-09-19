#ifndef __HUB_CHANNEL_H__
#define __HUB_CHANNEL_H__

#include "Poco/Channel.h"
#include "Poco/Message.h"
#include "Hub.h"
#include "Session.h"

namespace SubutaiLauncher
{

    class HubChannel : public Poco::Channel
    {
        public: 
            HubChannel();
            ~HubChannel();
            virtual void log(const Poco::Message& msg);
            virtual void open();
        private:
            Hub* _hub;
            Poco::FastMutex _mutex;
    };

}

#endif
