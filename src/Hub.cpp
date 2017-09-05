#include "Hub.h"
#include "Session.h"

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

    Hub::Hub() : 
        _loggedIn(false),
        _session(URL, 443),
        _id("")
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
        Poco::Net::HTTPRequest pRequest(Poco::Net::HTTPRequest::HTTP_POST, REST+"/tray/login", Poco::Net::HTTPRequest::HTTP_1_0);
        _logger->debug("Authenticating at %s%s/tray/login", URL, REST);

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
            try 
            {
                generateID();
            }
            catch (SubutaiException& e)
            {
                _logger->critical("Failed to generate installation ID");
                _id = "000abc";
            }
            _loggedIn = true;
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

    bool Hub::isLoggedIn()
    {
        return _loggedIn;
    }

    void Hub::sendLog(Poco::Message::Priority prio, const std::string& message)
    {
        std::string pPrio = "i";
        if (prio == Poco::Message::Priority::PRIO_NOTICE)
        {
            pPrio = "n";
        }
        else if (prio == Poco::Message::Priority::PRIO_WARNING)
        {
            pPrio = "w";
        }
        else if (prio == Poco::Message::Priority::PRIO_ERROR)
        {
            pPrio = "e";
        }
        else if (prio == Poco::Message::Priority::PRIO_CRITICAL)
        {
            pPrio = "c";
        }
        else if (prio == Poco::Message::Priority::PRIO_FATAL)
        {
            pPrio = "f";
        }
        std::string pStep = SubutaiLauncher::Session::instance()->getStep();
        std::string pAction = SubutaiLauncher::Session::instance()->getAction();
        std::string json("{");
        json.append("\"id\": \""+_id+"\",");
        json.append("\"step\": \""+pStep+"\",");
        json.append("\"act\": \""+pAction+"\",");
        json.append("\"log\": \""+message+"\",");
        json.append("\"lvl\": \""+pPrio+"\"");
        json.append("}");
        send("status", json);
    }

    void Hub::sendInfo(const std::string& key, const std::string& value)
    {
        std::string json("{");
        json.append("\"id\": \""+_id+"\",");
        json.append("\"key\": \""+key+"\",");
        json.append("\"value\": \""+value+"\"");
        json.append("}");
        send("info", json);
    }

    void Hub::send(const std::string& ep, const std::string& json)
    {
        Poco::Net::HTTPRequest pRequest(Poco::Net::HTTPRequest::HTTP_POST, REST+"/launcher/"+ep);
        pRequest.setCookies(_cookies);
        pRequest.setContentLength(json.length());
        std::ostream& pStr = _session.sendRequest(pRequest);
        pStr << json;

        Poco::Net::HTTPResponse pResponse;
        std::istream& rs = _session.receiveResponse(pResponse);
        std::string pBuffer;
        Poco::StreamCopier::copyToString(rs, pBuffer);
        if (pResponse.getStatus() == Poco::Net::HTTPResponse::HTTP_OK) 
        {
            return;
        }
    }

    void Hub::generateID()
    {
        if (_id != "") return;

        Poco::MD5Engine md5;
        Poco::DigestOutputStream ostr(md5);
        std::stringstream str;
        Poco::Timestamp t;
        str << t.epochTime();
        Poco::StreamCopier::copyStream(str, ostr);
        ostr.close();
        _id = Poco::DigestEngine::digestToHex(md5.digest()).substr(0, 6);
        if (_id.length() != 6)
        {
            throw SubutaiException("Failed to generate session ID");
        }
    }

    void Hub::addInfo(const std::string& key, const std::string& value)
    {
        if (key.empty() || value.empty()) return;
        InfoMessage m;
        m.key = key;
        m.value = Poco::replace(value, "\n", "\\n");
        _messages.push_back(m);
    }

    void Hub::flushInfo()
    {
        for (auto it = _messages.begin(); it != _messages.end();)
        {
            sendInfo((*it).key, (*it).value);
            it = _messages.erase(it);
        }
    }

    std::deque<InfoMessage> Hub::getInfo()
    {
        return _messages;
    }

}
