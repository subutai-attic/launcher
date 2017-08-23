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
        _logger->trace("Setting user login: %s", login);
        _login = login;
    }

    void Hub::setPassword(std::string password) 
    {
        _logger->trace("Setting user password");
        _password = password;
    }

    bool Hub::auth() 
    {
        _logger->debug("Authenticating at %s%s/tray/login", URL, REST);
        Poco::Net::HTTPRequest pRequest(Poco::Net::HTTPRequest::HTTP_POST, REST+"/tray/login", Poco::Net::HTTPRequest::HTTP_1_0);


        Poco::URI u;
        u.addQueryParameter("password", _password);
        std::string req = "email="+_login+"&"+u.toString().substr(1, u.toString().length() - 1);
        pRequest.setContentLength(req.length());
        pRequest.setContentType("application/x-www-form-urlencoded");
        std::ostream& pStr = _session.sendRequest(pRequest);
        pStr << req;
        
        Poco::Net::HTTPResponse pResponse;
        std::istream& rs = _session.receiveResponse(pResponse);
        std::string pBuffer;
        Poco::StreamCopier::copyToString(rs, pBuffer);
        _logger->trace("Received during login: %s", pBuffer);
        if (pResponse.getStatus() == Poco::Net::HTTPResponse::HTTP_OK) 
        {
            _logger->information("Hub login is OK");
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

    void Hub::addLogLine(HubLogLevel level, const std::string& message)
    {
        HubLog l;
        l.level = level;
        l.message = message;
        _logs.push_back(l);
    }

    void Hub::sendLogs()
    {
        for (auto it = _logs.begin(); it != _logs.end(); it++)
        {
            sendLog((*it).level, (*it).message);
        }
        _logs.clear();
    }

    void Hub::sendLog(HubLogLevel level, const std::string& message)
    {
        std::string status = "info";
        if (level == HL_WARNING) status = "warning";
        else if (level == HL_ERROR) status = "error";
        else if (level == HL_FATAL) status = "fatal";
        std::string json("{");
        json.append("\"uuid\": \"bd8a1439-ee45-4b6f-b3cd-ca60a0f8d61b\",");
        json.append("\"step\": \"install\",");
        json.append("\"problem\": \""+message+"\",");
        json.append("\"info\": \""+_login+"\",");
        json.append("\"status\": \""+status+"\"");
        json.append("}");
        
        _logger->trace("Sending log %s", json);
        Poco::Net::HTTPRequest pRequest(Poco::Net::HTTPRequest::HTTP_POST, REST+"/launcher/status");
        pRequest.setCookies(_cookies);
        pRequest.setContentLength(json.length());
        std::ostream& pStr = _session.sendRequest(pRequest);
        pStr << json;

        Poco::Net::HTTPResponse pResponse;
        std::istream& rs = _session.receiveResponse(pResponse);
        std::string pBuffer;
        Poco::StreamCopier::copyToString(rs, pBuffer);
        _logger->trace("Received during log sending: %s", pBuffer);
        if (pResponse.getStatus() == Poco::Net::HTTPResponse::HTTP_OK) 
        {
            _logger->debug("Log sent ok");
            return;
        }
        _logger->error("Log sent failed");
    }

}
