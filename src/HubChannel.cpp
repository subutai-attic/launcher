#include "HubChannel.h"

namespace SubutaiLauncher
{

    HubChannel::HubChannel()
    {

    }

    HubChannel::~HubChannel()
    {

    }

    void HubChannel::log(const Poco::Message& msg)
    {
        Poco::FastMutex::ScopedLock lock(_mutex);
        auto pPrio = msg.getPriority();
        if (pPrio == Poco::Message::PRIO_DEBUG || pPrio == Poco::Message::PRIO_TRACE)
        {
            return;
        }
        if ((int)msg.getPriority() < (int)Poco::Message::PRIO_INFORMATION) return;
        if (!Session::isInstanced()) return;
        if (!Session::instance()->getHub()->isLoggedIn()) return;
        Session::instance()->getHub()->sendLog(msg.getPriority(), msg.getText());
    }

    void HubChannel::open()
    {

    }

}
