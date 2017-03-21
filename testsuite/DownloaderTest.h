#ifndef __DOWNLOADER_TEST_H__
#define __DOWNLOADER_TEST_H__

#include "Poco/CppUnit/TestCase.h"

class DownloaderTest : public CppUnit::TestCase
{
public:
	DownloaderTest(const std::string& name);
	~DownloaderTest();

	void setUp();
	void tearDown();

    void testRetrieveFileInfo();
    void testDownload();

	static CppUnit::Test* suite();
};

#endif
