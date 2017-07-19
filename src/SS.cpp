#include "SS.h"

namespace SubutaiLauncher
{

	SS::SS(const std::string & url) : _url(url),
		_session(_url, 9999)
	{
		_logger = &Poco::Logger::get("subutai");
		_logger->information("Starting Subutai Session [%s]", url);
	}

	SS::~SS()
	{
	}

	bool SS::checkPeerInstall()
	{
		_logger->trace("SS::checkPeerInstall");
		Poco::Net::HTTPRequest pRequest(Poco::Net::HTTPRequest::HTTP_GET, "/rest/v1/peer/ready");
		_logger->debug("Sending request: %s", _url + "/rest/v1/peer/ready");
		_session.sendRequest(pRequest);

		_logger->trace("Getting response");
		Poco::Net::HTTPResponse pResponse;
		std::istream& rs = _session.receiveResponse(pResponse);
		std::string pBuffer;
		Poco::StreamCopier::copyToString(rs, pBuffer);
		_logger->trace("Received during peer install check: %s", pBuffer);
		if (pResponse.getStatus() == Poco::Net::HTTPResponse::HTTP_OK)
		{
			_logger->trace("Peer readiness check: OK");
			return true;
		}
		_logger->trace("Peer readiness check: Fail");
		return false;
	}

}