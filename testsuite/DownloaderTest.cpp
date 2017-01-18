#include "DownloaderTest.h"
#include "SubutaiLauncher.h"
#include "Poco/CppUnit/TestCaller.h"
#include "Poco/CppUnit/TestSuite.h"

DownloaderTest::DownloaderTest(const std::string& name) : CppUnit::TestCase(name)
{

}

DownloaderTest::~DownloaderTest()
{

}

void DownloaderTest::setUp()
{
}

void DownloaderTest::tearDown()
{
}

void DownloaderTest::testRetrieveFileInfo()
{
    SubutaiLauncher::Downloader d;
    d.setFilename("p2p");
    auto r = d.retrieveFileInfo();
    assert(r);
    auto i = d.info();
    assert(i.name == "p2p");
}

void DownloaderTest::testDownload()
{
    SubutaiLauncher::Downloader d;
    d.setFilename("p2p");
    auto t = d.download();
    t.join();
}

CppUnit::Test * DownloaderTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("DownloaderTest");

    CppUnit_addTest(pSuite, DownloaderTest, testRetrieveFileInfo);
    CppUnit_addTest(pSuite, DownloaderTest, testDownload);

	return pSuite;
}
