#include "NotificationCenter.h"

namespace SubutaiLauncher
{
    NotificationCenter::NotificationCenter() : _running(false)
    {
        _logger = &Poco::Logger::get("subutai");
        _logger->information("Starting notification center");
    }

    NotificationCenter::~NotificationCenter()
    {

    }

    void NotificationCenter::add(ScriptAction action)
    {
        _logger->debug("Notification received: %d", action);
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
        _logger->debug("Notification Center locked");
        _running = true;
    }

    void NotificationCenter::stop()
    {
        _logger->debug("Notification Center unlocked");
        _running = false;
    }

    bool NotificationCenter::isRunning()
    {
        return _running;
    }

    void NotificationCenter::notificationRaised(NotificationType t, Poco::DynamicAny v)
    {
        //_logger->debug("Notification raised: %s", v.convert<std::string>());
        NotificationMessage m;
        m.type = t;
        m.message = v;
        _npool.push_back(m);
    }

    NotificationMessage NotificationCenter::dispatchNotification()
    {
        if (_npool.empty()) return NotificationMessage{N_EMPTY, Poco::DynamicAny(0)};
        auto m = _npool.front();
        _npool.pop_front();
        return m;
    }

    bool NotificationCenter::notificationEmpty()
    {
        return _npool.empty();
    }

    void NotificationCenter::clear()
    {
        _pool.clear();
        _npool.clear();
    }

};

