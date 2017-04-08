#include "Hub.h"

namespace SubutaiLauncher 
{

#if BUILD_SCHEME_DEV
    const std::string Hub::URL = "dev.subut.ai";
#elif BUILD_SCHEME_MASTER
    const std::string Hub::URL = "stage.subut.ai";
#elif BUILD_SCHEME_PRODUCTION
    const std::string Hub::URL = "hub.subut.ai";
#endif
    const std::string Hub::REST = "/rest/v1";

    Hub::Hub() : _session(URL, 443)
    {
        _logger = &Poco::Logger::get("subutai");
        _logger->information("Starting Hub Session [%s]", URL);
    }

    Hub::~Hub() 
    {
    }

    void Hub::setLogin(std::string login) 
    {
        _login = login;
    }

    void Hub::setPassword(std::string password) 
    {
        _password = password;
    }

    bool Hub::auth() 
    {
        _logger->debug("Authenticating at %s%s/tray/login", URL, REST);
        Poco::Net::HTTPRequest pRequest(Poco::Net::HTTPRequest::HTTP_POST, REST+"/tray/login");
        Poco::Net::HTMLForm pForm;
        pForm.add("email", _login);
        pForm.add("password", _password);
        pForm.prepareSubmit(pRequest);
        _session.sendRequest(pRequest);

        Poco::Net::HTTPResponse pResponse;
        std::istream& rs = _session.receiveResponse(pResponse);
        std::string pBuffer;
        Poco::StreamCopier::copyToString(rs, pBuffer);
        _logger->trace("Received during login: %s", pBuffer);
        if (pResponse.getStatus() == Poco::Net::HTTPResponse::HTTP_OK) 
        {
            std::vector<Poco::Net::HTTPCookie> pCookies;
            pResponse.getCookies(pCookies);
            for (auto it = pCookies.begin(); it != pCookies.end(); it++)
            {
                _logger->trace("Saving Hub cookie: %s => %s", (*it).getName(), (*it).getValue());
                _cookies.add((*it).getName(), (*it).getValue());
            }
            return true;
        }
        return false;
    }

    bool Hub::balance() 
    {
        _logger->debug("Requesting user balance at %s%s/tray/balance", URL, REST);
        Poco::Net::HTTPRequest pRequest(Poco::Net::HTTPRequest::HTTP_GET, REST+"/tray/balance");
        pRequest.setCookies(_cookies);
        _session.sendRequest(pRequest);

        Poco::Net::HTTPResponse pResponse;
        std::istream& rs = _session.receiveResponse(pResponse);
        std::string pBuffer;
        Poco::StreamCopier::copyToString(rs, pBuffer);
        _logger->trace("Received during balance: %s", pBuffer);
        if (pResponse.getStatus() == Poco::Net::HTTPResponse::HTTP_OK) 
        {
            return true;
        }
        return false;
    }

}
