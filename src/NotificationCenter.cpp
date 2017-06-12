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
        _logger->debug("Notification received: %d", (int)action);
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

    void NotificationCenter::notificationRaised(NotificationType t, Poco::Dynamic::Var v)
	{
		_logger->debug("Notification raised");
        //_logger->debug("Notification raised: %s", v.convert<std::string>());
		/*if (t == N_DOUBLE_DATA)
		{
			_logger->debug("Notification: N_DOUBLE_DATA");
		}
		else
		{
			_logger->debug("Notification: OTHER_DATA");
		}*/
        NotificationMessage m;
        m.type = t;
        m.message = v;
        _npool.push_back(m);
    }

    NotificationMessage NotificationCenter::dispatchNotification()
    {
        if (_npool.empty()) return NotificationMessage{N_EMPTY, Poco::Dynamic::Var(0)};
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

