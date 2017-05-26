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
	try
	{
		std::vector<std::string> args;
		SubutaiLauncher::Core c(args);
		c.initializeSSL();
		SubutaiLauncher::Downloader d;
		d.setFilename("p2p");
		auto r = d.retrieveFileInfo();
		assert(r);
		auto i = d.info();
		assert(i.name == "p2p");
	}
	catch (Poco::OpenFileException& e)
	{
		std::printf("Openfileexception: %s", e.displayText());
	}
}

void DownloaderTest::testParseFileInfo()
{
    std::string json = "[{\"id\":\"3be143783b6f8a2fa90f09fbb1c02be0\",\"size\":6151936,\"name\":\"p2p\",\"owner\":[\"jenkins\"],\"version\":\"4.0.15-SNAPSHOT-8-g6a04867.\",\"filename\":\"p2p\"}]";
    Poco::JSON::Parser parser;
    Poco::Dynamic::Var result;
    try {
        result = parser.parse(json);
    } catch (Poco::JSON::JSONException e) {
        std::printf("Exception %s\n", e.displayText().c_str());
        assert(false);
    }
    assert(!result.isEmpty());
    assert(result.type() == typeid(Poco::JSON::Array::Ptr));
    Poco::JSON::Array::Ptr arr = result.extract<Poco::JSON::Array::Ptr>();
    assert(arr->size() == 1);
    for (size_t i = 0; i < arr->size(); i++)
    {
        Poco::JSON::Object::Ptr obj = arr->getObject(i);
        std::string id = obj->get("id").toString();
        assert(id == "3be143783b6f8a2fa90f09fbb1c02be0");
        std::string filename = obj->get("filename").toString();
        assert(filename == "p2p");
        int size = obj->get("size").extract<int>();
        assert(size == 6151936);
        Poco::JSON::Array::Ptr owners = obj->get("owner").extract<Poco::JSON::Array::Ptr>();
        for (size_t j = 0; j < owners->size(); j++)
        {
            std::string owner = owners->get(j).extract<std::string>();
            assert(owner == "jenkins");
        }
    }
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
    assert(hash == thash);
}

CppUnit::Test * DownloaderTest::suite()
{
    CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("DownloaderTest");

    CppUnit_addTest(pSuite, DownloaderTest, testRetrieveFileInfo);
    CppUnit_addTest(pSuite, DownloaderTest, testParseFileInfo);
    CppUnit_addTest(pSuite, DownloaderTest, testDownload);
    CppUnit_addTest(pSuite, DownloaderTest, testVerifyDownload);

    return pSuite;
}
