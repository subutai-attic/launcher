#ifndef __NOTIFICATION_CENTER_H__
#define __NOTIFICATION_CENTER_H__

#include "Vars.h"

#include <deque>
#include "Poco/Logger.h"
#include "Poco/DynamicAny.h"

namespace SubutaiLauncher
{

    enum ScriptAction
    {
        NO_EVENT = 0,
        DOWNLOAD_STARTED,
        DOWNLOAD_FINISHED,
        INSTALL_STARTED,
        INSTALL_FINISHED,
        SCRIPT_FINISHED,
        SHOW_MESSAGE
    };

    enum NotificationType
    {
        N_EMPTY = 0,
        N_ERROR,
        N_WARNING,
        N_INFO,
        N_DOUBLE_DATA
    };

    struct NotificationMessage
    {
        NotificationType type;
        Poco::DynamicAny message;
    };

    typedef std::deque<ScriptAction> EventPool;
    typedef std::deque<NotificationMessage> NotificationPool;

    class NotificationCenter
    {
        public:
            NotificationCenter();
            ~NotificationCenter();
            void add(ScriptAction action);
            ScriptAction dispatch();
            EventPool pool() const;

            // Lock controls
            void start();
            void stop();
            bool isRunning();

            // Specific events 

            void notificationRaised(NotificationType t, Poco::DynamicAny v);
            NotificationMessage dispatchNotification();
            bool notificationEmpty();
            void clear();
        private:
            Poco::Logger* _logger;
            EventPool _pool;
            NotificationPool _npool;
            bool _running;
    };
};

#endif
