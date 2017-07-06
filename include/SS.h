#ifndef __SS_H__
#define __SS_H__

#include <string>

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

namespace SubutaiLauncher
{
	class SS
	{
	public:
		SS(const std::string& url);
		~SS();
		bool checkPeerInstall();
	private:
		std::string _url;
		Poco::Logger* _logger;
		Poco::Net::HTTPSClientSession _session;
		Poco::Net::NameValueCollection _cookies;
	};
}

#endif