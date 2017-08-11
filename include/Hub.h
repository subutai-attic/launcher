#ifndef __HUB_H__
#define __HUB_H__

#include <string>
#include <sstream>
#include <cstring>
#include <map>

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
        private:
            Poco::Net::NameValueCollection getCookies();
            std::string _login;
            std::string _password;
            std::string _response;
            Poco::Logger* _logger;
            Poco::Net::HTTPSClientSession _session;
            Poco::Net::NameValueCollection _cookies;
    };

}

#endif
