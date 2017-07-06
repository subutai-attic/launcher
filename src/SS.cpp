#include "SS.h"

SubutaiLauncher::SS::SS(const std::string & url) : _url(url)
{
	_logger = &Poco::Logger::get("subutai");
	_logger->information("Starting Subutai Session [%s]", url);
}

SubutaiLauncher::SS::~SS()
{
}

bool SubutaiLauncher::SS::checkPeerInstall()
{
	Poco::Net::HTTPRequest pRequest(Poco::Net::HTTPRequest::HTTP_POST, _url + "/rest/v1/peer/ready");
	_session.sendRequest(pRequest);

	Poco::Net::HTTPResponse pResponse;
	std::istream& rs = _session.receiveResponse(pResponse);
	std::string pBuffer;
	Poco::StreamCopier::copyToString(rs, pBuffer);
	_logger->trace("Received during peer install check: %s", pBuffer);
	if (pResponse.getStatus() == Poco::Net::HTTPResponse::HTTP_OK)
	{
		std::vector<Poco::Net::HTTPCookie> pCookies;
		pResponse.getCookies(pCookies);
		for (auto it = pCookies.begin(); it != pCookies.end(); it++)
		{
			
			_cookies.add((*it).getName(), (*it).getValue());
		}
		return true;
	}
	return false;
}
