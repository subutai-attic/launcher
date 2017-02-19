#include "NotificationCenter.h"

namespace SubutaiLauncher
{
    NotificationCenter::NotificationCenter() : _running(false)
    {
        Log::instance()->logger()->info() << "Starting notification center" << std::endl;
    }

    NotificationCenter::~NotificationCenter()
    {

    }

    void NotificationCenter::add(ScriptAction action)
    {
        Log::instance()->logger()->debug() << "Notification received " << action << std::endl;
        _pool.push_back(action);
    }

    ScriptAction NotificationCenter::dispatch()
    {
        if (_pool.empty()) return NO_EVENT;
        auto e = _pool.front();
        _pool.pop_front();
        return e;
    }

    EventPool NotificationCenter::pool() const
    {
        return _pool;
    }

    void NotificationCenter::start()
    {
        Log::instance()->logger()->debug() << "Notification Center Locked" << std::endl;
        _running = true;
    }

    void NotificationCenter::stop()
    {
        Log::instance()->logger()->debug() << "Notification Center Unlocked" << std::endl;
        _running = false;
    }

    bool NotificationCenter::isRunning()
    {
        return _running;
    }

    void NotificationCenter::notificationRaised(NotificationType t, const std::string& s)
    {
        Log::instance()->logger()->debug() << "Notification Raised: " << s << std::endl;
        NotificationMessage m;
        m.type = t;
        m.message = s;
        _npool.push_back(m);
    }

    NotificationMessage NotificationCenter::dispatchNotification()
    {
        auto m = _npool.front();
        _npool.pop_front();
        return m;
    }

    void NotificationCenter::clear()
    {
        _pool.clear();
        _npool.clear();
    }

};

