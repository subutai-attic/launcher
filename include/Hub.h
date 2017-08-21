#ifndef __HUB_H__
#define __HUB_H__

#include <string>
#include <sstream>
#include <cstring>
#include <map>
#include <vector>

#include "Vars.h"
#include "Poco/Logger.h"
#include "Poco/StreamCopier.h"
#include "Poco/Net/NetException.h"
#include "Poco/Net/SSLException.h"
#include "Poco/Net/SSLManager.h"
#include "Poco/Net/HTTPStreamFactory.h"
#include "Poco/Net/FTPStreamFactory.h"
#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/HTTPSClientSession.h"
#include "Poco/Net/HTTPStreamFactory.h"
#include "Poco/Net/HTTPSStreamFactory.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTMLForm.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/AcceptCertificateHandler.h"
#include "Poco/Net/HTTPCookie.h"
#include "Poco/Net/NameValueCollection.h"
#include "Poco/Base64Encoder.h"

namespace SubutaiLauncher 
{
    enum HubLogLevel
    {
        HL_INFO = 0,
        HL_WARNING,
        HL_ERROR,
        HL_FATAL,
    };

    struct HubLog
    {
        HubLogLevel level;
        std::string message;
    };

    typedef std::vector<HubLog> HubLogs;

    class Hub 
    {
        public:
            static const std::string URL;
            static const std::string REST;
            Hub();
            ~Hub();
            void setLogin(std::string login);
            void setPassword(std::string password);
            bool auth();
            bool balance();
            void addLogLine(HubLogLevel level, const std::string& message);
            void sendLogs();
            void sendLog(HubLogLevel level, const std::string& message);
        private:
            Poco::Net::NameValueCollection getCookies();
            Poco::Logger* _logger;
            Poco::Net::HTTPSClientSession _session;
            Poco::Net::NameValueCollection _cookies;
            std::string _login;
            std::string _password;
            std::string _response;
            HubLogs _logs;
    };

}

#endif
