#ifndef __NOTIFICATION_CENTER_H__
#define __NOTIFICATION_CENTER_H__

#include <deque>
#include "Poco/Logger.h"

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
        N_INFO
    };

    struct NotificationMessage
    {
        NotificationType type;
        std::string message;
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

            void notificationRaised(NotificationType t, const std::string& s);
            NotificationMessage dispatchNotification();
            void clear();
        private:
            Poco::Logger* _logger;
            EventPool _pool;
            NotificationPool _npool;
            bool _running;
    };
};

#endif
