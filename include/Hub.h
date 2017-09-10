#ifndef __HUB_H__
#define __HUB_H__

#include <string>
#include <sstream>
#include <cstring>
#include <map>
#include <deque>

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
#include "Poco/URI.h"
#include "Poco/Base64Encoder.h"
#include "Poco/SplitterChannel.h"
#include "Poco/Message.h"
#include "Poco/DigestStream.h"
#include "Poco/MD5Engine.h"
#include "Poco/Timestamp.h"

#include "SubutaiException.h"

namespace SubutaiLauncher 
{

    struct InfoMessage
    {
        std::string key;
        std::string value;
    };

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
            bool isLoggedIn();
            void sendLog(Poco::Message::Priority prio, const std::string& message);
            void sendInfo(const std::string& key, const std::string& value);
            void addInfo(const std::string& key, const std::string& value);
            void flushInfo();
            std::deque<InfoMessage> getInfo();
			std::string encode(const std::string& data);
            const std::string& getId() const;
        private:
            void send(const std::string& ep, const std::string& json);
            void generateID();
            std::string _id;
            Poco::Net::NameValueCollection getCookies();
            Poco::Logger* _logger;
            Poco::Net::HTTPSClientSession _session;
            Poco::Net::NameValueCollection _cookies;
            std::string _login;
            std::string _password;
            std::string _response;
            bool _loggedIn;
            std::deque<InfoMessage> _messages;
    };

}

#endif
