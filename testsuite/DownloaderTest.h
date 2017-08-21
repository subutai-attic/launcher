#ifndef __DOWNLOADER_TEST_H__
#define __DOWNLOADER_TEST_H__

#include "Poco/CppUnit/TestCase.h"
#include "Poco/MD5Engine.h"
#include "Poco/DigestStream.h"
#include "Poco/DigestEngine.h"
#include "Poco/StreamCopier.h"
#include "Poco/FileStream.h"
#include "Poco/File.h"
#include "Poco/String.h"

#include <Poco/Net/NetException.h>
#include <Poco/Net/HTTPStreamFactory.h>
#include <Poco/Net/FTPStreamFactory.h>
#include <Poco/Net/HTTPClientSession.h>
#include <Poco/Net/HTTPSClientSession.h>
#include <Poco/Net/HTTPStreamFactory.h>
#include <Poco/Net/HTTPSStreamFactory.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTMLForm.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/URI.h>
#include <Poco/URIStreamOpener.h>
#include "Poco/JSON/Parser.h"
#include "Poco/JSON/ParseHandler.h"
#include "Poco/JSON/JSONException.h"
#include "Poco/Dynamic/Var.h"

#include "SubutaiLauncher.h"

class SlDownloaderThreadWorker {
private:
	SubutaiLauncher::Downloader* _downloader;
public:
	SlDownloaderThreadWorker(SubutaiLauncher::Downloader* downloader) : _downloader(downloader) {
	}
	~SlDownloaderThreadWorker() {/*do nothing*/ }

	void Run() {
		_downloader->downloadImpl();
	}
};

class DownloaderTest : public CppUnit::TestCase
{
public:
	DownloaderTest(const std::string& name);
	~DownloaderTest();

	void setUp();
	void tearDown();

    void testRetrieveFileInfo();
    void testParseFileInfo();
    void testDownload();
    void testVerifyDownload();

	static CppUnit::Test* suite();
};

#endif
