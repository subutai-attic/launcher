#include "SS.h"

SubutaiLauncher::SS::SS(const std::string & url) : _url(url)
{
	_logger = &Poco::Logger::get("subutai");
	_logger->information("Starting Subutai Session [%s]", url);
}

SubutaiLauncher::SS::~SS()
{
}
