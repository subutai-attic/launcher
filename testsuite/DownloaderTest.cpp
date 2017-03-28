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

void DownloaderTest::testVerifyDownload()
{
    std::string thash("6976394f40880db9d36d504336d49f36");

    std::ofstream tfile("md5-test-file", std::ios::binary);
    tfile << "md5-test-file contents";
    tfile.flush();
    tfile.close();

    Poco::MD5Engine md5;
    Poco::DigestOutputStream ostr(md5);

    Poco::FileInputStream fs("md5-test-file");
    Poco::StreamCopier::copyStream(fs, ostr);
    fs.close();
    ostr.close();

    Poco::File f("md5-test-file");
    f.remove();

    std::string hash = Poco::DigestEngine::digestToHex(md5.digest());
    std::printf("hash: %s\n", hash.c_str());
    assert(hash == thash);

    
}

CppUnit::Test * DownloaderTest::suite()
{
    CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("DownloaderTest");

    CppUnit_addTest(pSuite, DownloaderTest, testRetrieveFileInfo);
    CppUnit_addTest(pSuite, DownloaderTest, testDownload);
    CppUnit_addTest(pSuite, DownloaderTest, testVerifyDownload);

    return pSuite;
}
