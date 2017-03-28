#ifndef __DOWNLOADER_TEST_H__
#define __DOWNLOADER_TEST_H__

#include "Poco/CppUnit/TestCase.h"
#include "Poco/MD5Engine.h"
#include "Poco/DigestStream.h"
#include "Poco/DigestEngine.h"
#include "Poco/StreamCopier.h"
#include "Poco/FileStream.h"
#include "Poco/File.h"

class DownloaderTest : public CppUnit::TestCase
{
public:
	DownloaderTest(const std::string& name);
	~DownloaderTest();

	void setUp();
	void tearDown();

    void testRetrieveFileInfo();
    void testDownload();
    void testVerifyDownload();

	static CppUnit::Test* suite();
};

#endif
